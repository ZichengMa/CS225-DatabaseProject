/********************************************************************************
** Form generated from reading UI file 'weeklyreportshow.ui'
**
** Created by: Qt User Interface Compiler version 5.12.8
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_WEEKLYREPORTSHOW_H
#define UI_WEEKLYREPORTSHOW_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QComboBox>
#include <QtWidgets/QDialog>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QLabel>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QStatusBar>
#include <QtWidgets/QTableWidget>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_WeeklyReportShow
{
public:
    QWidget *centralwidget;
    QTableWidget *tableWidget;
    QTableWidget *tableWidget_2;
    QTableWidget *tableWidget_3;
    QPushButton *pushButton;
    QComboBox *comboBox;
    QLabel *label;
    QLabel *label_2;
    QLabel *label_3;
    QStatusBar *statusbar;

    void setupUi(QDialog *WeeklyReportShow)
    {
        if (WeeklyReportShow->objectName().isEmpty())
            WeeklyReportShow->setObjectName(QString::fromUtf8("WeeklyReportShow"));
        WeeklyReportShow->resize(1234, 540);
        centralwidget = new QWidget(WeeklyReportShow);
        centralwidget->setObjectName(QString::fromUtf8("centralwidget"));
        centralwidget->setGeometry(QRect(0, 0, 1231, 511));
        tableWidget = new QTableWidget(centralwidget);
        tableWidget->setObjectName(QString::fromUtf8("tableWidget"));
        tableWidget->setGeometry(QRect(10, 30, 391, 381));
        tableWidget_2 = new QTableWidget(centralwidget);
        tableWidget_2->setObjectName(QString::fromUtf8("tableWidget_2"));
        tableWidget_2->setGeometry(QRect(420, 30, 391, 381));
        tableWidget_3 = new QTableWidget(centralwidget);
        tableWidget_3->setObjectName(QString::fromUtf8("tableWidget_3"));
        tableWidget_3->setGeometry(QRect(830, 30, 391, 381));
        pushButton = new QPushButton(centralwidget);
        pushButton->setObjectName(QString::fromUtf8("pushButton"));
        pushButton->setGeometry(QRect(1090, 470, 101, 31));
        comboBox = new QComboBox(centralwidget);
        comboBox->setObjectName(QString::fromUtf8("comboBox"));
        comboBox->setGeometry(QRect(930, 470, 131, 23));
        label = new QLabel(centralwidget);
        label->setObjectName(QString::fromUtf8("label"));
        label->setGeometry(QRect(170, 430, 59, 15));
        label_2 = new QLabel(centralwidget);
        label_2->setObjectName(QString::fromUtf8("label_2"));
        label_2->setGeometry(QRect(590, 430, 59, 15));
        label_3 = new QLabel(centralwidget);
        label_3->setObjectName(QString::fromUtf8("label_3"));
        label_3->setGeometry(QRect(1000, 430, 59, 15));
        statusbar = new QStatusBar(WeeklyReportShow);
        statusbar->setObjectName(QString::fromUtf8("statusbar"));
        statusbar->setGeometry(QRect(0, 0, 3, 22));

        retranslateUi(WeeklyReportShow);

        QMetaObject::connectSlotsByName(WeeklyReportShow);
    } // setupUi

    void retranslateUi(QDialog *WeeklyReportShow)
    {
        WeeklyReportShow->setWindowTitle(QApplication::translate("WeeklyReportShow", "Weekly Report", nullptr));
        pushButton->setText(QApplication::translate("WeeklyReportShow", "Export", nullptr));
        label->setText(QApplication::translate("WeeklyReportShow", "Treated", nullptr));
        label_2->setText(QApplication::translate("WeeklyReportShow", "Registed", nullptr));
        label_3->setText(QApplication::translate("WeeklyReportShow", "In Queue", nullptr));
    } // retranslateUi

};

namespace Ui {
    class WeeklyReportShow: public Ui_WeeklyReportShow {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_WEEKLYREPORTSHOW_H
