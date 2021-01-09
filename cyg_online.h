#ifndef CYG_ONLINE_H
#define CYG_ONLINE_H

#include <QObject>
#include <QRunnable>
#include <QProcess>
#include <QTcpSocket>

//注意！要使用信号，采用QObejct 和 QRunnable多继承，记得QObject要放在前面
class CygOnline: public QObject, public QRunnable {
    Q_OBJECT
public:
    CygOnline(QString s_ip, QString s_sn, QString s_location);

protected:
    void run() override;

private:
    QString S_cygIP, S_cygSN, S_cygLocation;


signals:
    void sendResult(QStringList result);
};

#endif // CYG_ONLINE_H
