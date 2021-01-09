#include "cyg2comm.h"
#include "ui_cyg2comm.h"

Cyg2Comm::Cyg2Comm(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::Cyg2Comm) {
    ui->setupUi(this);
    database = QSqlDatabase::addDatabase("QPSQL");
    stat = new QLabel;
    stat->setText("Cygnus2 Communication Test Platform v1.01   ");
    stat->setAlignment(Qt::AlignRight);
    statusBar()->addPermanentWidget(stat);
    statusBar()->setSizeGripEnabled(false);
    ui->startall_btn->setEnabled(false);
}

Cyg2Comm::~Cyg2Comm() {
    if(database.isOpen()) {
        database.close();
    }
    delete ui;
}

void Cyg2Comm::receivResult(QStringList str_list) {
    if(database.isOpen()) {
        QSqlQuery sql_query;
        sql_query.prepare(insertString);
        for (int i = 0; i < str_list.size(); i++) {
            sql_query.addBindValue(str_list[i]);
        }
        statusBar()->showMessage(str_list[2] + "\t" + str_list[0] + "  Received.");
        sql_query.exec();
    }
}

bool Cyg2Comm::connectDB() {
    if(!database.isOpen()) {
        database.setHostName("192.168.5.29");
        database.setPort(5432);
        database.setUserName("inficon_service");
        database.setPassword("inficon");
        database.setDatabaseName("cygnus");
    }
    return database.open();
}

void Cyg2Comm::disConnectDB() {
    if(database.isOpen()) {
        database.close();
    }
}

void Cyg2Comm::recivedIPInfo(QStringList datalist) {
    int row_count = ui->unit_tw->rowCount();
    bool b_isAccessAble = datalist.at(3).contains("Cygnus");
    ui->startall_btn->setEnabled(b_isAccessAble);
    QPushButton *btn_start = new QPushButton;
    btn_start->setIcon(QIcon(":/picture/start.png"));
    btn_start->setIconSize(QSize(20, 20));
    ui->unit_tw->insertRow(row_count);
    ui->unit_tw->setItem(row_count, 0, new QTableWidgetItem(datalist.at(0)));
    ui->unit_tw->setItem(row_count, 1, new QTableWidgetItem(datalist.at(1)));
    ui->unit_tw->setItem(row_count, 2, new QTableWidgetItem(datalist.at(2)));
    ui->unit_tw->setItem(row_count, 3, new QTableWidgetItem(datalist.at(3)));
    if(b_isAccessAble) {
        ui->unit_tw->setCellWidget(row_count, 4, btn_start);
        btn_start->setProperty("sn", datalist.at(0));
        btn_start->setProperty("btn_index", row_count);
        btn_start->setProperty("ip", datalist.at(1));
        btn_start->setProperty("location", datalist.at(2));
        connect(btn_start, SIGNAL(clicked()), this, SLOT(getData()));
    } else {
        ui->unit_tw->setItem(row_count, 4, new QTableWidgetItem("Offline"));
    }
    CygOpt opt;
    opt.btn = btn_start;
    opt.s_ip = datalist.at(1);
    opt.s_sn = datalist.at(0);
    opt.s_location = datalist.at(2);
    opt.b_cygConnected = datalist.at(4).toInt();
    opt.worker = new CygWorker(nullptr, datalist.at(1), datalist.at(0), datalist.at(2));
    options.append(opt);
    I_cygCount++;
}

void Cyg2Comm::initializeTable() {
    // initialize table widget
    ui->unit_tw->setColumnCount(5);
    QFont font("Courier New", 20, QFont::Bold);
    QStringList tblHeader;
    tblHeader << tr("SN") << tr("IP") << tr("Location") << tr("Info") << ("Opt");
    ui->unit_tw->setHorizontalHeaderLabels(tblHeader);
    ui->unit_tw->verticalHeader()->setVisible(false);
    ui->unit_tw->setEditTriggers(QAbstractItemView::NoEditTriggers);
    ui->unit_tw->model()->setHeaderData(0, Qt::Horizontal, QFont("Courier New", 14, QFont::Bold), Qt::FontRole);
    ui->unit_tw->model()->setHeaderData(1, Qt::Horizontal, QFont("Courier New", 14, QFont::Bold), Qt::FontRole);
    ui->unit_tw->model()->setHeaderData(2, Qt::Horizontal, QFont("Courier New", 14, QFont::Bold), Qt::FontRole);
    ui->unit_tw->model()->setHeaderData(2, Qt::Horizontal, QFont("Courier New", 14, QFont::Bold), Qt::FontRole);
    ui->unit_tw->model()->setHeaderData(3, Qt::Horizontal, QFont("Courier New", 14, QFont::Bold), Qt::FontRole);
    ui->unit_tw->horizontalHeader()->setSectionResizeMode(0, QHeaderView::ResizeToContents);
    ui->unit_tw->horizontalHeader()->setSectionResizeMode(1, QHeaderView::ResizeToContents);
    ui->unit_tw->horizontalHeader()->setSectionResizeMode(2, QHeaderView::ResizeToContents);
    ui->unit_tw->horizontalHeader()->setSectionResizeMode(3, QHeaderView::Stretch);
    ui->unit_tw->horizontalHeader()->setSectionResizeMode(4, QHeaderView::ResizeToContents);
}


void Cyg2Comm::getData() {
    ui->startall_btn->setEnabled(false);
    QPushButton *btn = dynamic_cast<QPushButton *>(sender());
    int btn_index = btn->property("btn_index").toInt();
    CygOpt opt = options[btn_index];
    btn->setDisabled(true);
    connect(options.at(btn_index).worker, &QThread::finished, options.at(btn_index).worker, &QObject::deleteLater);
    connect(options.at(btn_index).worker, &CygWorker::resultReady, this, &Cyg2Comm::receivResult);
    options.at(btn_index).worker->start();
}

void Cyg2Comm::clearCygs() {
    if(options.count() < 1) {
        return;
    }
    foreach (CygOpt opt, options) {
        opt.worker->tfcAquireStop();
        statusBar()->showMessage("All unit stopped.");
        opt.btn->setEnabled(false);
    }
    options.clear();
    ui->startall_btn->setEnabled(false);
    I_cygCount = 0;
}

void Cyg2Comm::on_search_btn_clicked() {
    clearCygs();
    if(!database.isOpen()) {
        if(!connectDB()) {
            statusBar()->showMessage("Can't connect to Database");
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
            CygOnline *finder = new CygOnline(
                selectQuery.value(2).toString(),
                selectQuery.value(1).toString(),
                selectQuery.value(3).toString()
            );
            connect(finder, &CygOnline::sendResult, this, &Cyg2Comm::recivedIPInfo);
            QThreadPool::globalInstance()->start(finder);
        }
    }
}

void Cyg2Comm::on_stopall_btn_clicked() {
    clearCygs();
}

void Cyg2Comm::on_startall_btn_clicked() {
    if(options.count() < 1) {
        return;
    }
    foreach (CygOpt opt, options) {
        if(opt.b_cygConnected) {
            opt.btn->click();
            Sleep(1000);
        }
    }
}
