#ifndef CYGCOMM_CONF_H
#define CYGCOMM_CONF_H

#include <QDialog>

namespace Ui {
class CygcommConf;
}

class CygcommConf : public QDialog
{
    Q_OBJECT

public:
    explicit CygcommConf(QWidget *parent = nullptr);
    ~CygcommConf();

private:
    Ui::CygcommConf *ui;
};

#endif // CYGCOMM_CONF_H
