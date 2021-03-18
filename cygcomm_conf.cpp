#include "cygcomm_conf.h"
#include "ui_cygcomm_conf.h"

CygcommConf::CygcommConf(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::CygcommConf)
{
    ui->setupUi(this);
}

CygcommConf::~CygcommConf()
{
    delete ui;
}
