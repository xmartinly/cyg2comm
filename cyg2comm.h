#ifndef CYG2COMM_H
#define CYG2COMM_H

#include <QMainWindow>

#include <QFile>

#include <QSqlDatabase>
#include <QSqlError>
#include <QSqlQuery>
#include <QSqlRecord>

#include <QDebug>
#include <QTcpSocket>
#include <QThread>
#include <QThreadPool>

#include <QPushButton>
#include <QLabel>

#include <QApplication>
#include <QEventLoop>

#include "cyg_online.h"
#include "cyg_worker.h"


namespace Ui {
    class Cyg2Comm;
}

class Cyg2Comm : public QMainWindow {
    Q_OBJECT

public:
    explicit Cyg2Comm(QWidget *parent = nullptr);
    ~Cyg2Comm();

    QString updateString = "UPDATE cygnus_list SET WHERE cyg_sn = :cyg_sn",
            insertString = "INSERT INTO cyg_data (cyg_sn, cyg_ip, update_time, cyg_version, cyg_trans_msg, cyg_dac_error, cyg_ch1_pwr,cyg_ch1_thk, cyg_ch2_pwr,cyg_ch2_thk, cyg_ch3_pwr,cyg_ch3_thk, cyg_ch4_pwr,cyg_ch4_thk, cyg_ch5_pwr,cyg_ch5_thk, cyg_ch6_pwr,cyg_ch6_thk) VALUES ( ?,?,?,?,?,?,?,?,?,?,?,?,?,?,?,?,?,?)",
            selectString = "SELECT * FROM cyg_list";

public slots:

    void receivResult(QStringList strlist);

    void recivedIPInfo(QStringList ip_info);

private slots:
    void on_search_btn_clicked();

    void on_startall_btn_clicked();

    void on_stopall_btn_clicked();

    void getData();


private:
    Ui::Cyg2Comm *ui;

    typedef struct CygOpts {
        QPushButton *btn;
        int i_btnIndex;
        bool b_btnClicked = false, b_cygConnected = false;
        QString s_sn, s_ip, s_location;
        CygWorker *worker;
    } CygOpt;

    QVector<CygOpt> options;

    int I_cygCount = 0;

    QSqlDatabase database;

    QThreadPool pool;

    QThreadPool *global_pool = QThreadPool::globalInstance();

    QLabel *stat;

    QList<QPushButton> start_btns;

    void Sleep(int msec) {
        QTime dieTime = QTime::currentTime().addMSecs(msec);
        while( QTime::currentTime() < dieTime ) {
            QCoreApplication::processEvents(QEventLoop::AllEvents, 100);
        }
    }

    bool connectDB();

    void disConnectDB();

    void initializeTable();

    void clearCygs();

protected:

};

#endif // CYG2COMM_H
