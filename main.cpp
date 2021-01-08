#include "cyg2comm.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    Cyg2Comm w;
    QFile qss(":qss/Aqua.qss");
    if( qss.open(QFile::ReadOnly)) {
        qApp->setStyleSheet(qss.readAll());
        qss.close();
    }
    w.show();
    return a.exec();
}
