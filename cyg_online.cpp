#include "cyg_online.h"

CygOnline::CygOnline(QString s_ip, QString s_sn)
{
    s_cygIP = s_ip;
    s_cygSN = s_sn;
}

void CygOnline::run()
{
    bool ip_online = false;
    QString ip_ttl = "N/A", ip_info = "Offline";
    QTcpSocket *tfc_socket = new QTcpSocket;
    QByteArray cmd, buffer;
    QStringList result;
    cmd[0] = '\x02';
    cmd[1] = '\x00';
    cmd[2] = '\x48';
    cmd[3] = '\x01';
    cmd[4] = '\x49';
    tfc_socket->connectToHost(s_cygIP, 2101, QIODevice::ReadWrite);
    ip_online = tfc_socket->waitForConnected(250);
    if(ip_online) {
        tfc_socket->write(cmd);
        tfc_socket->flush();
    }
    if(tfc_socket->waitForReadyRead(250)) {
        buffer = tfc_socket->readAll();
    }
    ip_info = QString(buffer.mid(5, 22));
    if(ip_info.length() < 10) {
        ip_info = "Other In Control";
    }
    buffer.clear();
    if(tfc_socket->isOpen()) {
        tfc_socket->close();
    }
    result << s_cygSN << s_cygIP << ip_info << QString::number(ip_online) ;
    emit sendResult(result);
}

