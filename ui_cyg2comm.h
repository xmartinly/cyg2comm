/********************************************************************************
** Form generated from reading UI file 'cyg2comm.ui'
**
** Created by: Qt User Interface Compiler version 5.12.2
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_CYG2COMM_H
#define UI_CYG2COMM_H

#include <QtCore/QVariant>
#include <QtGui/QIcon>
#include <QtWidgets/QApplication>
#include <QtWidgets/QGridLayout>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QLabel>
#include <QtWidgets/QMainWindow>
#include <QtWidgets/QMenuBar>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QSpacerItem>
#include <QtWidgets/QStatusBar>
#include <QtWidgets/QTableWidget>
#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_Cyg2Comm
{
public:
    QWidget *centralWidget;
    QGridLayout *gridLayout;
    QHBoxLayout *horizontalLayout;
    QSpacerItem *horizontalSpacer_4;
    QLabel *label;
    QSpacerItem *horizontalSpacer_5;
    QPushButton *search_btn;
    QSpacerItem *horizontalSpacer;
    QPushButton *startall_btn;
    QSpacerItem *horizontalSpacer_3;
    QPushButton *stopall_btn;
    QSpacerItem *horizontalSpacer_2;
    QVBoxLayout *verticalLayout;
    QTableWidget *unit_tw;
    QMenuBar *menuBar;
    QStatusBar *statusBar;

    void setupUi(QMainWindow *Cyg2Comm)
    {
        if (Cyg2Comm->objectName().isEmpty())
            Cyg2Comm->setObjectName(QString::fromUtf8("Cyg2Comm"));
        Cyg2Comm->resize(1024, 768);
        QSizePolicy sizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);
        sizePolicy.setHorizontalStretch(0);
        sizePolicy.setVerticalStretch(0);
        sizePolicy.setHeightForWidth(Cyg2Comm->sizePolicy().hasHeightForWidth());
        Cyg2Comm->setSizePolicy(sizePolicy);
        Cyg2Comm->setMinimumSize(QSize(1024, 768));
        Cyg2Comm->setMaximumSize(QSize(1024, 768));
        centralWidget = new QWidget(Cyg2Comm);
        centralWidget->setObjectName(QString::fromUtf8("centralWidget"));
        gridLayout = new QGridLayout(centralWidget);
        gridLayout->setSpacing(6);
        gridLayout->setContentsMargins(11, 11, 11, 11);
        gridLayout->setObjectName(QString::fromUtf8("gridLayout"));
        horizontalLayout = new QHBoxLayout();
        horizontalLayout->setSpacing(6);
        horizontalLayout->setObjectName(QString::fromUtf8("horizontalLayout"));
        horizontalSpacer_4 = new QSpacerItem(100, 20, QSizePolicy::Fixed, QSizePolicy::Minimum);

        horizontalLayout->addItem(horizontalSpacer_4);

        label = new QLabel(centralWidget);
        label->setObjectName(QString::fromUtf8("label"));
        QFont font;
        font.setFamily(QString::fromUtf8("Courier New"));
        font.setPointSize(24);
        font.setBold(true);
        font.setWeight(75);
        label->setFont(font);

        horizontalLayout->addWidget(label);

        horizontalSpacer_5 = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout->addItem(horizontalSpacer_5);

        search_btn = new QPushButton(centralWidget);
        search_btn->setObjectName(QString::fromUtf8("search_btn"));
        search_btn->setMinimumSize(QSize(0, 30));
        search_btn->setMaximumSize(QSize(75, 16777215));
        QIcon icon;
        icon.addFile(QString::fromUtf8(":/picture/search.png"), QSize(), QIcon::Normal, QIcon::Off);
        search_btn->setIcon(icon);
        search_btn->setIconSize(QSize(55, 55));

        horizontalLayout->addWidget(search_btn);

        horizontalSpacer = new QSpacerItem(40, 20, QSizePolicy::Fixed, QSizePolicy::Minimum);

        horizontalLayout->addItem(horizontalSpacer);

        startall_btn = new QPushButton(centralWidget);
        startall_btn->setObjectName(QString::fromUtf8("startall_btn"));
        startall_btn->setMinimumSize(QSize(0, 30));
        startall_btn->setMaximumSize(QSize(75, 16777215));
        QIcon icon1;
        icon1.addFile(QString::fromUtf8(":/picture/add.png"), QSize(), QIcon::Normal, QIcon::Off);
        startall_btn->setIcon(icon1);
        startall_btn->setIconSize(QSize(55, 55));

        horizontalLayout->addWidget(startall_btn);

        horizontalSpacer_3 = new QSpacerItem(40, 20, QSizePolicy::Fixed, QSizePolicy::Minimum);

        horizontalLayout->addItem(horizontalSpacer_3);

        stopall_btn = new QPushButton(centralWidget);
        stopall_btn->setObjectName(QString::fromUtf8("stopall_btn"));
        QSizePolicy sizePolicy1(QSizePolicy::Minimum, QSizePolicy::Fixed);
        sizePolicy1.setHorizontalStretch(0);
        sizePolicy1.setVerticalStretch(0);
        sizePolicy1.setHeightForWidth(stopall_btn->sizePolicy().hasHeightForWidth());
        stopall_btn->setSizePolicy(sizePolicy1);
        stopall_btn->setMinimumSize(QSize(0, 30));
        stopall_btn->setMaximumSize(QSize(75, 16777215));
        QIcon icon2;
        icon2.addFile(QString::fromUtf8(":/picture/stopall.png"), QSize(), QIcon::Normal, QIcon::Off);
        stopall_btn->setIcon(icon2);
        stopall_btn->setIconSize(QSize(55, 55));

        horizontalLayout->addWidget(stopall_btn);

        horizontalSpacer_2 = new QSpacerItem(40, 20, QSizePolicy::Fixed, QSizePolicy::Minimum);

        horizontalLayout->addItem(horizontalSpacer_2);


        gridLayout->addLayout(horizontalLayout, 0, 0, 1, 1);

        verticalLayout = new QVBoxLayout();
        verticalLayout->setSpacing(6);
        verticalLayout->setObjectName(QString::fromUtf8("verticalLayout"));
        unit_tw = new QTableWidget(centralWidget);
        unit_tw->setObjectName(QString::fromUtf8("unit_tw"));

        verticalLayout->addWidget(unit_tw);

        verticalLayout->setStretch(0, 6);

        gridLayout->addLayout(verticalLayout, 1, 0, 1, 1);

        Cyg2Comm->setCentralWidget(centralWidget);
        menuBar = new QMenuBar(Cyg2Comm);
        menuBar->setObjectName(QString::fromUtf8("menuBar"));
        menuBar->setGeometry(QRect(0, 0, 1024, 23));
        Cyg2Comm->setMenuBar(menuBar);
        statusBar = new QStatusBar(Cyg2Comm);
        statusBar->setObjectName(QString::fromUtf8("statusBar"));
        Cyg2Comm->setStatusBar(statusBar);

        retranslateUi(Cyg2Comm);

        QMetaObject::connectSlotsByName(Cyg2Comm);
    } // setupUi

    void retranslateUi(QMainWindow *Cyg2Comm)
    {
        Cyg2Comm->setWindowTitle(QApplication::translate("Cyg2Comm", "Cyg2Comm", nullptr));
        label->setText(QApplication::translate("Cyg2Comm", "Cygnus2 Test", nullptr));
#ifndef QT_NO_TOOLTIP
        search_btn->setToolTip(QApplication::translate("Cyg2Comm", "Search", nullptr));
#endif // QT_NO_TOOLTIP
        search_btn->setText(QString());
#ifndef QT_NO_TOOLTIP
        startall_btn->setToolTip(QApplication::translate("Cyg2Comm", "Start All", nullptr));
#endif // QT_NO_TOOLTIP
        startall_btn->setText(QString());
#ifndef QT_NO_TOOLTIP
        stopall_btn->setToolTip(QApplication::translate("Cyg2Comm", "Stop All", nullptr));
#endif // QT_NO_TOOLTIP
        stopall_btn->setText(QString());
    } // retranslateUi

};

namespace Ui {
    class Cyg2Comm: public Ui_Cyg2Comm {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_CYG2COMM_H
