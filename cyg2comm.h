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

#include "cyg_online.h"
#include "cyg_worker.h"


namespace Ui
{
    class Cyg2Comm;
}

class Cyg2Comm : public QMainWindow
{
    Q_OBJECT

public:
    explicit Cyg2Comm(QWidget *parent = nullptr);
    ~Cyg2Comm();

    QString updateString = "UPDATE cygnus_list SET WHERE cyg_sn = :cyg_sn",
            insertString = "INSERT INTO cygnus_comm (update_time, comm_sn, unit_ip, unit_version, unit_location, frequency, activity, dac_error, trans_msg) VALUES (?,?,?,?,?,?,?,?,?)",
            selectString = "SELECT * FROM cyg_list";

public slots:
    void receivResult(QStringList strlist);
    void recivedIPInfo(QStringList ip_info);

private slots:
    void on_search_btn_clicked();
    void on_add_btn_clicked();
    void on_stopall_btn_clicked();
    void getData();

private:
    Ui::Cyg2Comm *ui;

    bool connectDB();
    void disConnectDB();
    void initializeTable();
    void clearCygs();

    typedef struct CygOpts {
        QPushButton * btn;
        int i_btnIndex;
        bool b_btnClicked = false, b_cygConnected = false;
        QString s_sn, s_ip, s_location;
        CygWorker *worker;
    } CygOpt;

    QVector<CygOpt> options;

    QSqlDatabase database;

    QThreadPool pool;
    QThreadPool *global_pool = QThreadPool::globalInstance();

    QLabel *stat;
protected:

signals:
    void stopWorker();
};

#endif // CYG2COMM_H
