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
    QByteArray buffer = tfc_Socket->readAll();
    QStringList sl_buffer, sl_data_list;
    if(buffer.size() == 82) {
        b_read_ok = DataCompute::InficonTFCCheckSum(buffer.mid(2, 79)) == DataCompute::HexTodec(buffer.mid(81, 1).toHex().data());
        if(b_read_ok) {
            s_buffer = buffer.mid(4, 77).toHex();
            sl_buffer = s_buffer.split("06");
            sl_buffer.removeAll(QString(""));
        }
    } else {
        return;
    }
    if(b_read_ok) {
        bool ok;
        sl_data_list << cyg_sn  // cygnus2 sn
                     << cyg_ip  // cygnus2 IP
                     << QDateTime::currentDateTime().toString("yyyy/MM/dd HH:mm:ss:z") // data time
                     << QByteArray::fromHex(sl_buffer[0].toUtf8().data()).toStdString().c_str() // cygnus2 version
                     << errMsg::transMsg(sl_buffer.at(3).toInt(&ok, 16)) // trans message
                     << QString::number(sl_buffer.at(4).toInt(&ok, 16)); // dac error number
        QString s_chs_pwr = sl_buffer.at(1), s_chs_thk = sl_buffer.at(2);
        for(int i = 1; i < 7; i++) {
            QString s_ch_pwr_hex = s_chs_pwr.mid((i - 1) * 8, 8),
                    s_ch_thx_hex = s_chs_thk.mid((i - 1) * 8, 8),
                    s_ch_pwr_value = QString::number(DataCompute::hexStrToFloat(s_ch_pwr_hex)),
                    s_ch_thk_value = QString::number(DataCompute::hexStrToFloat(s_ch_thx_hex));
            sl_data_list << s_ch_pwr_value << s_ch_thk_value;
        }
        emit resultReady(sl_data_list);
    }
}


