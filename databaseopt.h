#ifndef DATABASEOPT_H
#define DATABASEOPT_H

#include <QObject>
#include <QSqlDatabase>
#include <QSqlError>
#include <QSqlQuery>
#include <QSqlRecord>
#include <QThread>


class DatabaseOpt: public QThread
{
    Q_OBJECT

public:
    explicit DatabaseOpt(QObject *parent = nullptr);

public slots:

private:

    QString updateString = "UPDATE cygnus_list SET online = :online WHERE list_sn = :list_sn",
            insertString = "INSERT INTO cygnus_comm (comm_sn, dac_error, trans_msg, frequency, activity, update_time, unit_ip, unit_version, unit_location) VALUES (?,?,?,?,?,?,?,?,?)",
            selectString = "SELECT * FROM cygnus_list";

    QSqlDatabase database;


private slots:

protected:
};

#endif // DATABASEOPT_H
