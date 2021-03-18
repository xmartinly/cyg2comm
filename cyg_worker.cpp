#include "cyg_worker.h"

CygWorker::CygWorker(QObject *parent, QString s_ip, QString s_sn, QString s_location) : QThread(parent) {
    cyg_ip = s_ip;
    cyg_sn  = s_sn;
    cyg_location = s_location;
    cmd_acquire.append('\x27');
    cmd_acquire.append('\x00');
    cmd_acquire.append('\x53');
    cmd_acquire.append('\x47');
    cmd_acquire.append('\x0f'); // date
    cmd_acquire.append('\x48');
    cmd_acquire.append('\x01'); //message hello 0
    cmd_acquire.append('\x53');
    cmd_acquire.append('\x47');
    cmd_acquire.append('\x0f'); // date
    cmd_acquire.append('\x53');
    cmd_acquire.append('\x4c');
    cmd_acquire.append('\x03');
    cmd_acquire.append('\x00');  //material pwr 1
    cmd_acquire.append('\x53');
    cmd_acquire.append('\x47');
    cmd_acquire.append('\x0f'); // date
    cmd_acquire.append('\x53');
    cmd_acquire.append('\x4c');
    cmd_acquire.append('\x04');
    cmd_acquire.append('\x00'); //material thk 2
    cmd_acquire.append('\x53');
    cmd_acquire.append('\x47');
    cmd_acquire.append('\x0f'); // date
    cmd_acquire.append('\x53');
    cmd_acquire.append('\x47');
    cmd_acquire.append('\x09'); //trans msg SG9 3
    cmd_acquire.append('\x53');
    cmd_acquire.append('\x47');
    cmd_acquire.append('\x0f'); // date
    cmd_acquire.append('\x53');
    cmd_acquire.append('\x47');
    cmd_acquire.append('\x07');
    cmd_acquire.append('\x00');
    cmd_acquire.append('\x00');
    cmd_acquire.append('\x00');
    cmd_acquire.append('\x02');
    cmd_acquire.append('\x00');
    cmd_acquire.append('\x00');
    cmd_acquire.append('\x00');
    cmd_acquire.append('\x00');// dac error 4
    cmd_acquire.append('\x21');
    cmd_reset.append('\x1e');
    cmd_reset.append('\x00'); // length
    cmd_reset.append('\x52');
    cmd_reset.append('\x47');
    cmd_reset.append('\x01');
    cmd_reset.append('\x52');
    cmd_reset.append('\x47');
    cmd_reset.append('\x02');
    cmd_reset.append('\x52');
    cmd_reset.append('\x4c');
    cmd_reset.append('\x0b');
    cmd_reset.append('\x01');
    cmd_reset.append('\x52');
    cmd_reset.append('\x4C');
    cmd_reset.append('\x0b');
    cmd_reset.append('\x02');
    cmd_reset.append('\x52');
    cmd_reset.append('\x4C');
    cmd_reset.append('\x0b');
    cmd_reset.append('\x03');
    cmd_reset.append('\x52');
    cmd_reset.append('\x4C');
    cmd_reset.append('\x0b');
    cmd_reset.append('\x04');
    cmd_reset.append('\x52');
    cmd_reset.append('\x4C');
    cmd_reset.append('\x0b');
    cmd_reset.append('\x05');
    cmd_reset.append('\x52');
    cmd_reset.append('\x4C');
    cmd_reset.append('\x0b');
    cmd_reset.append('\x06');
    cmd_reset.append('\x40'); // checksum
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
    if(B_firstConnection) {
        tfc_Socket->write(cmd_reset);
    }
    if(tfc_Socket->state() == 3) {
        tfc_Socket->write(cmd_acquire);
        tfc_Socket->flush();
    } else {
        B_firstConnection = true;
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
            acquire_Timer->setInterval(10000);
            connect(acquire_Timer, SIGNAL(timeout()), this, SLOT(tfcAquireStart()), Qt::DirectConnection);
            QTimer::singleShot(0, acquire_Timer, SLOT(start()));
        }
    }
    exec();
}

void CygWorker::recData() {
    if(B_firstConnection) {
        B_firstConnection = false;
        return;
    }
    bool b_read_ok = false;
    QString s_buffer;
    int i_msg_length;
    QByteArray buffer = tfc_Socket->readAll(),
               ba_resp,
               ba_checksum;
    i_msg_length = buffer.size();
    ba_resp = buffer.mid(2, i_msg_length - 3);
    ba_checksum = buffer.mid(i_msg_length - 1, 1);
    b_read_ok = DataCompute::InficonTFCCheckSum(ba_resp) == DataCompute::HexTodec(ba_checksum.toHex().data());
    if(b_read_ok) {
        QStringList sl_resp, sl_data_list;
        QString s_resp_sp = buffer.mid(4, 14).toHex(), s_buffer = buffer.mid(17, i_msg_length - 18).toHex(),
                s_version,
                s_chs_pwr,
                s_chs_thk,
                s_trans,
                s_dac;
        sl_resp = s_buffer.split(s_resp_sp);
        s_version = QByteArray::fromHex(sl_resp.at(0).mid(2, -1).toLocal8Bit()).data();
        s_chs_pwr = sl_resp.at(1);
        s_chs_thk = sl_resp.at(2);
        s_trans = sl_resp.at(3);
        s_dac = sl_resp.at(4);
        int i_dac = DataCompute::hexStrToInt(s_dac.right(2));
        sl_data_list << cyg_sn  // cygnus2 sn
                     << cyg_ip  // cygnus2 IP
                     << cyg_location // cygnus2 location
                     << QString::number(QDateTime::currentMSecsSinceEpoch()) // data time
                     << s_version // cygnus2 version
                     << errMsg::transMsg(DataCompute::hexStrToInt(s_trans)) // trans message
                     << (i_dac > 0 ? "DAC Error " + QString::number(i_dac) : "N/A"); // dac error number
        for(int i = 1; i < 7; i++) {
            QString s_ch_pwr_hex = s_chs_pwr.mid((i - 1) * 8, 8),
                    s_ch_thx_hex = s_chs_thk.mid((i - 1) * 8, 8),
                    s_ch_pwr_value = QString::number(DataCompute::hexStrToFloat(s_ch_pwr_hex), 'd', 2),
                    s_ch_thk_value = QString::number(DataCompute::hexStrToFloat(s_ch_thx_hex), 'd', 3);
            sl_data_list << s_ch_pwr_value << s_ch_thk_value;
        }
        emit resultReady(sl_data_list);
    }
}


