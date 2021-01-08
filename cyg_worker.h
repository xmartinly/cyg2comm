#ifndef CYGWORKER_H
#define CYGWORKER_H

#include <QObject>
#include <QThread>
#include <QDateTime>
#include <QTimer>
#include <QTcpSocket>

#include "data_compute.h"
#include "errmsg.h"

class CygWorker: public QThread
{
    Q_OBJECT

public:
    explicit CygWorker(QObject *parent = nullptr, QString s_ip = "", QString s_sn = "");

    ~CygWorker() override;


public slots:

    void tfcAquireStop();

private:

    QTcpSocket *tfc_Socket;
    QTimer *acquire_Timer, *opt_Timer;
    QByteArray cmd;
    bool b_stopAcquire = false;
    QString cyg_ip, cyg_sn, cyg_location;

    DataCompute data_compute;

private slots:

    void tfcAquireStart();

    void recData();

protected:

    void run() override;

signals:
    void resultReady(const QStringList &strlist);

};

#endif // CYGWORKER_H
