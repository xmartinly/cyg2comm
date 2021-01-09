#include "cyg_worker.h"

CygWorker::CygWorker(QObject *parent, QString s_ip, QString s_sn, QString s_location) : QThread(parent) {
    cyg_ip = s_ip;
    cyg_sn  = s_sn;
    cyg_location = s_location;
//    b_stopAcquire = false;
    cmd[0] = '\x18';
    cmd[1] = '\x00';
    //message hello
    cmd[2] = '\x48';
    cmd[3] = '\x01';
    //material pwr
    cmd[4] = '\x53';
    cmd[5] = '\x4c';
    cmd[6] = '\x03';
    cmd[7] = '\x00';
    //material thk
    cmd[8] = '\x53';
    cmd[9] = '\x4c';
    cmd[10] = '\x04';
    cmd[11] = '\x00';
    //trans msg SG9
    cmd[12] = '\x53';
    cmd[13] = '\x47';
    cmd[14] = '\x09';
    //DAC error message SG7
    cmd[15] = '\x53';
    cmd[16] = '\x47';
    cmd[17] = '\x07';
    cmd[18] = '\x00';
    cmd[19] = '\x00';
    cmd[20] = '\x00';
    cmd[21] = '\x02';
    cmd[22] = '\x00';
    cmd[23] = '\x00';
    cmd[24] = '\x00';
    cmd[25] = '\x00';
    //checksum
    cmd[26] = '\xD4';
}

CygWorker::~CygWorker() {
    if(tfc_Socket->isOpen()) {
        tfc_Socket->abort();
    }
    quit();
    wait(500);
}



void CygWorker::tfcAquireStop() {
    b_stopAcquire = true;
}

void CygWorker::tfcAquireStart() {
    if(b_stopAcquire && acquire_Timer->isActive()) {
        tfc_Socket->abort();
        acquire_Timer->stop();
        quit();
        return;
    }
    if(tfc_Socket->state() == 3) {
        tfc_Socket->write(cmd);
        tfc_Socket->flush();
    } else {
        tfc_Socket->abort();
        tfc_Socket->connectToHost(cyg_ip, 2101, QIODevice::ReadWrite);
        tfc_Socket->waitForConnected(2000);
    }
}


void CygWorker::run() {
    if(cyg_ip != "") {
        tfc_Socket = new QTcpSocket();
        tfc_Socket->connectToHost(cyg_ip, 2101, QIODevice::ReadWrite);
        if(tfc_Socket->waitForConnected(250)) {
            connect(tfc_Socket, SIGNAL(readyRead()), this, SLOT(recData()));
            acquire_Timer = new QTimer;
            acquire_Timer->setInterval(5000);
            connect(acquire_Timer, SIGNAL(timeout()), this, SLOT(tfcAquireStart()), Qt::DirectConnection);
            QTimer::singleShot(0, acquire_Timer, SLOT(start()));
        }
    }
    exec();
}

void CygWorker::recData() {
    bool b_read_ok = false;
    QString s_buffer;
    QByteArray buffer = tfc_Socket->readAll(),
               ba_version,
               ba_pwr,
               ba_trans,
               ba_dac,
               ba_thk;
    QStringList sl_buffer, sl_data_list;
    if(buffer.size() == 82) {
        b_read_ok = DataCompute::InficonTFCCheckSum(buffer.mid(2, 79)) == DataCompute::HexTodec(buffer.mid(81, 1).toHex().data());
        if(b_read_ok) {
            bool ok;
            ba_version = buffer.mid(5, 22);
            ba_pwr = buffer.mid(28, 24);
            ba_thk = buffer.mid(53, 24);
            ba_trans = buffer.mid(78, 1);
            ba_dac = buffer.mid(80, 1);
            sl_data_list << cyg_sn  // cygnus2 sn
                         << cyg_ip  // cygnus2 IP
                         << QDateTime::currentDateTime().toString("yyyy/MM/dd HH:mm:ss:z") // data time
                         << ba_version.toStdString().c_str() // cygnus2 version
                         << errMsg::transMsg(ba_trans.toInt(&ok, 16)) // trans message
                         << QString::number(ba_dac.toInt(&ok, 16)); // dac error number
            QString s_chs_pwr = ba_pwr.toHex(), s_chs_thk = ba_thk.toHex();
            for(int i = 1; i < 7; i++) {
                QString s_ch_pwr_hex = s_chs_pwr.mid((i - 1) * 8, 8),
                        s_ch_thx_hex = s_chs_thk.mid((i - 1) * 8, 8),
                        s_ch_pwr_value = QString::number(DataCompute::hexStrToFloat(s_ch_pwr_hex), 'd', 2),
                        s_ch_thk_value = QString::number(DataCompute::hexStrToFloat(s_ch_thx_hex), 'd', 3);
                sl_data_list << s_ch_pwr_value << s_ch_thk_value;
            }
            emit resultReady(sl_data_list);
        }
    } else {
        return;
    }
}


