#include "cyg_worker.h"

CygWorker::CygWorker(QObject *parent, QString s_ip, QString s_sn) : QThread(parent)
{
    cyg_ip = s_ip;
    cyg_sn  = s_sn;
    cmd[0] = '\x18';
    cmd[1] = '\x00';
    //message hello
    cmd[2] = '\x48';
    cmd[3] = '\x01';
    //sensor1 activity SS51
    cmd[4] = '\x53';
    cmd[5] = '\x53';
    cmd[6] = '\x05';
    cmd[7] = '\x01';
    //sensor1 frequence SS41
    cmd[8] = '\x53';
    cmd[9] = '\x53';
    cmd[10] = '\x04';
    cmd[11] = '\x01';
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
    cmd[26] = '\xe6';
}

CygWorker::~CygWorker()
{
    if(tfc_Socket->isOpen()) {
        tfc_Socket->abort();
    }
    quit();
    wait(500);
}



void CygWorker::tfcAquireStop()
{
    b_stopAcquire = true;
}

void CygWorker::tfcAquireStart()
{
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


void CygWorker::run()
{
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

void CygWorker::recData()
{
    int msgTrans, msgDAC;
    bool ok = true;
    QString dateTime = QDateTime::currentDateTime().toString("yyyy/MM/dd HH:mm:ss:z"), _msgTrans, _msgDAC;
    QByteArray buffer, msgH1, msgACT, msgFreq;
    QStringList dataList;
    buffer = tfc_Socket->readAll();
    QByteArray ba_reponse = buffer.mid(2, 43),  ba_checksum = buffer.mid(45, 1);
    QString _ba_checksum = ba_checksum.toHex();
    if(DataCompute::InficonTFCCheckSum(ba_reponse) == DataCompute::HexTodec(_ba_checksum.toLatin1().data())) {
        msgH1 = buffer.mid(5, 22);
        msgACT = buffer.mid(28, 4);
        msgFreq = buffer.mid(33, 8);
        msgTrans = buffer.mid(42, 1).toHex().toInt(&ok, 16);
        msgDAC = buffer.mid(44, 1).toHex().toInt(&ok, 16);
        _msgTrans = msgTrans == 0 ? "N/A" : errMsg::transMsg(msgTrans);
        _msgDAC = msgDAC == 0 ? "N/A" : QString::number(msgDAC);
        dataList << dateTime
                 << cyg_sn
                 << cyg_ip
                 << QString(msgH1)
                 << cyg_location
                 << QString::number(DataCompute::ReadFreq(msgFreq), 'f', 3)
                 << QString::number(DataCompute::ReadLong(msgACT))
                 << _msgDAC
                 << _msgTrans;
        emit resultReady(dataList);
        dataList.clear();
    }
}

