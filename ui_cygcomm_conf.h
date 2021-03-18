/********************************************************************************
** Form generated from reading UI file 'cygcomm_conf.ui'
**
** Created by: Qt User Interface Compiler version 5.12.2
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_CYGCOMM_CONF_H
#define UI_CYGCOMM_CONF_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QComboBox>
#include <QtWidgets/QDialog>

QT_BEGIN_NAMESPACE

class Ui_CygcommConf
{
public:
    QComboBox *regionCb;
    QComboBox *intvCb;

    void setupUi(QDialog *CygcommConf)
    {
        if (CygcommConf->objectName().isEmpty())
            CygcommConf->setObjectName(QString::fromUtf8("CygcommConf"));
        CygcommConf->resize(640, 480);
        regionCb = new QComboBox(CygcommConf);
        regionCb->addItem(QString());
        regionCb->addItem(QString());
        regionCb->addItem(QString());
        regionCb->addItem(QString());
        regionCb->addItem(QString());
        regionCb->setObjectName(QString::fromUtf8("regionCb"));
        regionCb->setGeometry(QRect(30, 40, 69, 22));
        intvCb = new QComboBox(CygcommConf);
        intvCb->addItem(QString());
        intvCb->addItem(QString());
        intvCb->addItem(QString());
        intvCb->addItem(QString());
        intvCb->addItem(QString());
        intvCb->setObjectName(QString::fromUtf8("intvCb"));
        intvCb->setGeometry(QRect(140, 40, 69, 22));

        retranslateUi(CygcommConf);

        QMetaObject::connectSlotsByName(CygcommConf);
    } // setupUi

    void retranslateUi(QDialog *CygcommConf)
    {
        CygcommConf->setWindowTitle(QApplication::translate("CygcommConf", "Dialog", nullptr));
        regionCb->setItemText(0, QApplication::translate("CygcommConf", "CD", nullptr));
        regionCb->setItemText(1, QApplication::translate("CygcommConf", "GZ", nullptr));
        regionCb->setItemText(2, QApplication::translate("CygcommConf", "BJ", nullptr));
        regionCb->setItemText(3, QApplication::translate("CygcommConf", "SH", nullptr));
        regionCb->setItemText(4, QApplication::translate("CygcommConf", "Other", nullptr));

        intvCb->setItemText(0, QApplication::translate("CygcommConf", "2000", nullptr));
        intvCb->setItemText(1, QApplication::translate("CygcommConf", "3000", nullptr));
        intvCb->setItemText(2, QApplication::translate("CygcommConf", "4000", nullptr));
        intvCb->setItemText(3, QApplication::translate("CygcommConf", "5000", nullptr));
        intvCb->setItemText(4, QApplication::translate("CygcommConf", "10000", nullptr));

    } // retranslateUi

};

namespace Ui {
    class CygcommConf: public Ui_CygcommConf {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_CYGCOMM_CONF_H
