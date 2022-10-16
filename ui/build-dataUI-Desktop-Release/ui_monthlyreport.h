/********************************************************************************
** Form generated from reading UI file 'monthlyreport.ui'
**
** Created by: Qt User Interface Compiler version 5.12.8
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_MONTHLYREPORT_H
#define UI_MONTHLYREPORT_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QDialog>
#include <QtWidgets/QLabel>
#include <QtWidgets/QPushButton>

QT_BEGIN_NAMESPACE

class Ui_MonthlyReport
{
public:
    QLabel *label;
    QLabel *label_2;
    QLabel *label_3;
    QLabel *label_4;
    QLabel *label_5;
    QLabel *label_6;
    QPushButton *pushButton;

    void setupUi(QDialog *MonthlyReport)
    {
        if (MonthlyReport->objectName().isEmpty())
            MonthlyReport->setObjectName(QString::fromUtf8("MonthlyReport"));
        MonthlyReport->resize(436, 324);
        label = new QLabel(MonthlyReport);
        label->setObjectName(QString::fromUtf8("label"));
        label->setGeometry(QRect(40, 20, 121, 31));
        label_2 = new QLabel(MonthlyReport);
        label_2->setObjectName(QString::fromUtf8("label_2"));
        label_2->setGeometry(QRect(40, 60, 121, 31));
        label_3 = new QLabel(MonthlyReport);
        label_3->setObjectName(QString::fromUtf8("label_3"));
        label_3->setGeometry(QRect(40, 100, 121, 31));
        label_4 = new QLabel(MonthlyReport);
        label_4->setObjectName(QString::fromUtf8("label_4"));
        label_4->setGeometry(QRect(40, 140, 121, 31));
        label_5 = new QLabel(MonthlyReport);
        label_5->setObjectName(QString::fromUtf8("label_5"));
        label_5->setGeometry(QRect(40, 180, 121, 31));
        label_6 = new QLabel(MonthlyReport);
        label_6->setObjectName(QString::fromUtf8("label_6"));
        label_6->setGeometry(QRect(40, 220, 121, 31));
        pushButton = new QPushButton(MonthlyReport);
        pushButton->setObjectName(QString::fromUtf8("pushButton"));
        pushButton->setGeometry(QRect(320, 270, 80, 23));

        retranslateUi(MonthlyReport);

        QMetaObject::connectSlotsByName(MonthlyReport);
    } // setupUi

    void retranslateUi(QDialog *MonthlyReport)
    {
        MonthlyReport->setWindowTitle(QApplication::translate("MonthlyReport", "Monthly Report", nullptr));
        label->setText(QApplication::translate("MonthlyReport", "TextLabel", nullptr));
        label_2->setText(QApplication::translate("MonthlyReport", "TextLabel", nullptr));
        label_3->setText(QApplication::translate("MonthlyReport", "TextLabel", nullptr));
        label_4->setText(QApplication::translate("MonthlyReport", "TextLabel", nullptr));
        label_5->setText(QApplication::translate("MonthlyReport", "TextLabel", nullptr));
        label_6->setText(QApplication::translate("MonthlyReport", "TextLabel", nullptr));
        pushButton->setText(QApplication::translate("MonthlyReport", "Export", nullptr));
    } // retranslateUi

};

namespace Ui {
    class MonthlyReport: public Ui_MonthlyReport {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_MONTHLYREPORT_H
