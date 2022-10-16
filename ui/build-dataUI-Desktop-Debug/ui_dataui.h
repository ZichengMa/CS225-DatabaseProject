/********************************************************************************
** Form generated from reading UI file 'dataui.ui'
**
** Created by: Qt User Interface Compiler version 5.12.8
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_DATAUI_H
#define UI_DATAUI_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QGroupBox>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QLabel>
#include <QtWidgets/QLineEdit>
#include <QtWidgets/QListWidget>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QTableWidget>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_dataUI
{
public:
    QTableWidget *tableWidget;
    QGroupBox *QueryBox;
    QPushButton *IncButton;
    QLineEdit *QueryInput;
    QPushButton *Clear_pb;
    QPushButton *WeekRP_pb;
    QPushButton *Monthrp_pb;
    QPushButton *LoadNew;
    QPushButton *UpdateRec;
    QGroupBox *SimulationBox;
    QPushButton *SimruntButton;
    QLabel *RunLabel;
    QPushButton *Conditionset_button;
    QListWidget *NoteList;
    QPushButton *pushButton;
    QLabel *NowDisp;
    QPushButton *Step_pb;

    void setupUi(QWidget *dataUI)
    {
        if (dataUI->objectName().isEmpty())
            dataUI->setObjectName(QString::fromUtf8("dataUI"));
        dataUI->resize(1298, 879);
        tableWidget = new QTableWidget(dataUI);
        tableWidget->setObjectName(QString::fromUtf8("tableWidget"));
        tableWidget->setGeometry(QRect(10, 20, 891, 781));
        tableWidget->setSortingEnabled(false);
        QueryBox = new QGroupBox(dataUI);
        QueryBox->setObjectName(QString::fromUtf8("QueryBox"));
        QueryBox->setGeometry(QRect(920, 340, 361, 181));
        QueryBox->setAutoFillBackground(false);
        IncButton = new QPushButton(QueryBox);
        IncButton->setObjectName(QString::fromUtf8("IncButton"));
        IncButton->setGeometry(QRect(130, 90, 101, 31));
        QueryInput = new QLineEdit(QueryBox);
        QueryInput->setObjectName(QString::fromUtf8("QueryInput"));
        QueryInput->setGeometry(QRect(10, 40, 341, 31));
        Clear_pb = new QPushButton(QueryBox);
        Clear_pb->setObjectName(QString::fromUtf8("Clear_pb"));
        Clear_pb->setGeometry(QRect(240, 90, 101, 31));
        WeekRP_pb = new QPushButton(QueryBox);
        WeekRP_pb->setObjectName(QString::fromUtf8("WeekRP_pb"));
        WeekRP_pb->setGeometry(QRect(130, 130, 101, 31));
        Monthrp_pb = new QPushButton(QueryBox);
        Monthrp_pb->setObjectName(QString::fromUtf8("Monthrp_pb"));
        Monthrp_pb->setGeometry(QRect(240, 130, 101, 31));
        LoadNew = new QPushButton(QueryBox);
        LoadNew->setObjectName(QString::fromUtf8("LoadNew"));
        LoadNew->setGeometry(QRect(20, 90, 101, 31));
        UpdateRec = new QPushButton(QueryBox);
        UpdateRec->setObjectName(QString::fromUtf8("UpdateRec"));
        UpdateRec->setGeometry(QRect(20, 130, 101, 31));
        SimulationBox = new QGroupBox(dataUI);
        SimulationBox->setObjectName(QString::fromUtf8("SimulationBox"));
        SimulationBox->setGeometry(QRect(920, 10, 361, 321));
        SimulationBox->setAutoFillBackground(false);
        SimruntButton = new QPushButton(SimulationBox);
        SimruntButton->setObjectName(QString::fromUtf8("SimruntButton"));
        SimruntButton->setGeometry(QRect(10, 30, 81, 31));
        RunLabel = new QLabel(SimulationBox);
        RunLabel->setObjectName(QString::fromUtf8("RunLabel"));
        RunLabel->setGeometry(QRect(290, 40, 59, 21));
        Conditionset_button = new QPushButton(SimulationBox);
        Conditionset_button->setObjectName(QString::fromUtf8("Conditionset_button"));
        Conditionset_button->setGeometry(QRect(100, 70, 81, 31));
        NoteList = new QListWidget(SimulationBox);
        NoteList->setObjectName(QString::fromUtf8("NoteList"));
        NoteList->setGeometry(QRect(15, 110, 331, 191));
        pushButton = new QPushButton(SimulationBox);
        pushButton->setObjectName(QString::fromUtf8("pushButton"));
        pushButton->setGeometry(QRect(10, 70, 81, 31));
        NowDisp = new QLabel(SimulationBox);
        NowDisp->setObjectName(QString::fromUtf8("NowDisp"));
        NowDisp->setGeometry(QRect(208, 80, 131, 20));
        Step_pb = new QPushButton(SimulationBox);
        Step_pb->setObjectName(QString::fromUtf8("Step_pb"));
        Step_pb->setGeometry(QRect(100, 30, 81, 31));
        QWidget::setTabOrder(SimruntButton, Step_pb);
        QWidget::setTabOrder(Step_pb, pushButton);
        QWidget::setTabOrder(pushButton, Conditionset_button);
        QWidget::setTabOrder(Conditionset_button, NoteList);
        QWidget::setTabOrder(NoteList, QueryInput);
        QWidget::setTabOrder(QueryInput, LoadNew);
        QWidget::setTabOrder(LoadNew, IncButton);
        QWidget::setTabOrder(IncButton, Clear_pb);
        QWidget::setTabOrder(Clear_pb, UpdateRec);
        QWidget::setTabOrder(UpdateRec, WeekRP_pb);
        QWidget::setTabOrder(WeekRP_pb, Monthrp_pb);
        QWidget::setTabOrder(Monthrp_pb, tableWidget);

        retranslateUi(dataUI);

        QMetaObject::connectSlotsByName(dataUI);
    } // setupUi

    void retranslateUi(QWidget *dataUI)
    {
        dataUI->setWindowTitle(QApplication::translate("dataUI", "dataUI", nullptr));
        QueryBox->setTitle(QApplication::translate("dataUI", "Query", nullptr));
        IncButton->setText(QApplication::translate("dataUI", "Query", nullptr));
        Clear_pb->setText(QApplication::translate("dataUI", "Clear", nullptr));
        WeekRP_pb->setText(QApplication::translate("dataUI", "Weekly Report", nullptr));
        Monthrp_pb->setText(QApplication::translate("dataUI", "Monthly Report", nullptr));
        LoadNew->setText(QApplication::translate("dataUI", "Load New", nullptr));
        UpdateRec->setText(QApplication::translate("dataUI", "Update Record", nullptr));
        SimulationBox->setTitle(QApplication::translate("dataUI", "Simulation Control", nullptr));
        SimruntButton->setText(QApplication::translate("dataUI", "Run", nullptr));
        RunLabel->setText(QApplication::translate("dataUI", "Stopped", nullptr));
        Conditionset_button->setText(QApplication::translate("dataUI", "Set", nullptr));
        pushButton->setText(QApplication::translate("dataUI", "Clear", nullptr));
        NowDisp->setText(QApplication::translate("dataUI", "1970/01/01/00/00/00", nullptr));
        Step_pb->setText(QApplication::translate("dataUI", "Step", nullptr));
    } // retranslateUi

};

namespace Ui {
    class dataUI: public Ui_dataUI {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_DATAUI_H
