#include "cyg2comm.h"
#include "ui_cyg2comm.h"

Cyg2Comm::Cyg2Comm(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::Cyg2Comm)
{
    ui->setupUi(this);
    database = QSqlDatabase::addDatabase("QPSQL");
    stat = new QLabel;
    stat->setAlignment(Qt::AlignRight);
    statusBar()->addWidget(stat, 1);
    ui->add_btn->setHidden(true);
}

Cyg2Comm::~Cyg2Comm()
{
    if(database.isOpen()) {
        database.close();
    }
    delete ui;
}

void Cyg2Comm::on_search_btn_clicked()
{
    clearCygs();
    if(!database.isOpen()) {
        if(!connectDB()) {
            stat->setText("Can't connect to Database");
            return;
        }
    }
    if(ui->unit_tw->rowCount() > 0) {
        ui->unit_tw->clearContents();
        ui->unit_tw->setRowCount(0);
    }
    QSqlQuery selectQuery;
    QStringList onlineCygs;
    selectQuery.prepare(selectString);
    if (selectQuery.exec()) {
        initializeTable();
        global_pool->setMaxThreadCount(QThread::idealThreadCount());
        while (selectQuery.next()) {
            CygOnline * finder = new CygOnline( selectQuery.value(2).toString(),
                                                selectQuery.value(1).toString());
            connect(finder, &CygOnline::sendResult, this, &Cyg2Comm::recivedIPInfo);
            QThreadPool::globalInstance()->start(finder);
        }
    }
}

void Cyg2Comm::on_add_btn_clicked()
{
}

void Cyg2Comm::on_stopall_btn_clicked()
{
    clearCygs();
}

void Cyg2Comm::receivResult(QStringList strlist)
{
    if(database.isOpen()) {
        QSqlQuery sql_query;
        sql_query.prepare(insertString);
        for (int i = 0; i < strlist.size(); i++) {
            sql_query.addBindValue(strlist[i]);
        }
        stat->setText(strlist.join("\t | "));
        sql_query.exec();
    }
}

bool Cyg2Comm::connectDB()
{
    if(!database.isOpen()) {
        database.setHostName("192.168.5.29");
        database.setPort(5432);
        database.setUserName("inficon_service");
        database.setPassword("inficon");
        database.setDatabaseName("cygnus");
    }
    return database.open();
}

void Cyg2Comm::disConnectDB()
{
    if(database.isOpen()) {
        database.close();
    }
}

void Cyg2Comm::recivedIPInfo(QStringList datalist)
{
    qDebug() << datalist;
    int row_count = ui->unit_tw->rowCount();
    bool b_isAccessAble = datalist.at(3) != "Offline" && datalist.at(3) != "" && datalist.at(3) != "Other In Control";
    QPushButton * btn_start = new QPushButton;
    btn_start->setIcon(QIcon(":/picture/start.png"));
    btn_start->setIconSize(QSize(20, 20));
    ui->unit_tw->insertRow(row_count);
    ui->unit_tw->setItem(row_count, 0, new QTableWidgetItem(datalist.at(0)));
    ui->unit_tw->setItem(row_count, 1, new QTableWidgetItem(datalist.at(1)));
    ui->unit_tw->setItem(row_count, 2, new QTableWidgetItem(datalist.at(2)));
    if(b_isAccessAble) {
        //i_cygOnlineCount++;
        ui->unit_tw->setCellWidget(row_count, 3, btn_start);
        btn_start->setProperty("sn", datalist.at(0));
        btn_start->setProperty("ip", datalist.at(1));
        connect(btn_start, SIGNAL(clicked()), this, SLOT(getData()));
    }
//    if(database.isOpen()) {
//        QSqlQuery updateQuery;
//        updateQuery.prepare(updateString);
//        updateQuery.addBindValue(datalist.at(6));
//        updateQuery.addBindValue(datalist.at(0));
//        updateQuery.exec();
//    }
}

void Cyg2Comm::initializeTable()
{
    // initialize table widget
    ui->unit_tw->setColumnCount(4);
    QFont font("Courier New", 20, QFont::Bold);
    QStringList tblHeader;
    tblHeader << tr("SN") << tr("IP") << tr("Info") << ("Opt");
    ui->unit_tw->setHorizontalHeaderLabels(tblHeader);
    ui->unit_tw->verticalHeader()->setVisible(false);
    ui->unit_tw->setEditTriggers(QAbstractItemView::NoEditTriggers);
    ui->unit_tw->model()->setHeaderData(0, Qt::Horizontal, QFont("Courier New", 14, QFont::Bold), Qt::FontRole);
    ui->unit_tw->model()->setHeaderData(1, Qt::Horizontal, QFont("Courier New", 14, QFont::Bold), Qt::FontRole);
    ui->unit_tw->model()->setHeaderData(2, Qt::Horizontal, QFont("Courier New", 14, QFont::Bold), Qt::FontRole);
    ui->unit_tw->model()->setHeaderData(3, Qt::Horizontal, QFont("Courier New", 14, QFont::Bold), Qt::FontRole);
    ui->unit_tw->horizontalHeader()->setSectionResizeMode(0, QHeaderView::ResizeToContents);
    ui->unit_tw->horizontalHeader()->setSectionResizeMode(1, QHeaderView::ResizeToContents);
    ui->unit_tw->horizontalHeader()->setSectionResizeMode(2, QHeaderView::Stretch);
    ui->unit_tw->horizontalHeader()->setSectionResizeMode(3, QHeaderView::ResizeToContents);
}


void Cyg2Comm::getData()
{
    QPushButton *btn = dynamic_cast<QPushButton *>(sender());
    CygOpt opt;
    btn->setDisabled(true);
    QString ip = btn->property("ip").toString(),
            sn = btn->property("sn").toString();
    int btn_index = btn->property("btn_index").toInt(),
        i_optionsCurrent = options.count();
    opt.btn = btn;
    opt.s_ip = ip;
    opt.s_sn = sn;
    opt.i_btnIndex = btn_index;
    opt.worker = new CygWorker(nullptr, ip, sn, location);
    options.append(opt);
    connect(options.at(i_optionsCurrent).worker, &QThread::finished, options.at(i_optionsCurrent).worker, &QObject::deleteLater);
    connect(options.at(i_optionsCurrent).worker, &CygWorker::resultReady, this, &Cyg2Comm::receivResult);
    options.at(i_optionsCurrent).worker->start();
}

void Cyg2Comm::clearCygs()
{
    if(options.count() < 1) {
        return;
    }
    foreach (CygOpt opt, options) {
        stopWorker();
        stat->setText("");
        opt.btn->setEnabled(true);
    }
    options.clear();
}


