/********************************************************************************
** Form generated from reading UI file 'simcond_dialog.ui'
**
** Created by: Qt User Interface Compiler version 5.12.8
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_SIMCOND_DIALOG_H
#define UI_SIMCOND_DIALOG_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QDateTimeEdit>
#include <QtWidgets/QDialog>
#include <QtWidgets/QDialogButtonBox>
#include <QtWidgets/QLabel>
#include <QtWidgets/QSpinBox>
#include <QtWidgets/QTimeEdit>

QT_BEGIN_NAMESPACE

class Ui_Simcond_Dialog
{
public:
    QDialogButtonBox *buttonBox;
    QDateTimeEdit *NowSet;
    QLabel *label;
    QTimeEdit *IntervalSet;
    QLabel *label_2;
    QSpinBox *spinBox;
    QLabel *label_3;

    void setupUi(QDialog *Simcond_Dialog)
    {
        if (Simcond_Dialog->objectName().isEmpty())
            Simcond_Dialog->setObjectName(QString::fromUtf8("Simcond_Dialog"));
        Simcond_Dialog->resize(400, 300);
        buttonBox = new QDialogButtonBox(Simcond_Dialog);
        buttonBox->setObjectName(QString::fromUtf8("buttonBox"));
        buttonBox->setGeometry(QRect(30, 240, 341, 32));
        buttonBox->setOrientation(Qt::Horizontal);
        buttonBox->setStandardButtons(QDialogButtonBox::Cancel|QDialogButtonBox::Ok);
        NowSet = new QDateTimeEdit(Simcond_Dialog);
        NowSet->setObjectName(QString::fromUtf8("NowSet"));
        NowSet->setGeometry(QRect(130, 60, 194, 24));
        NowSet->setDateTime(QDateTime(QDate(1970, 1, 1), QTime(0, 0, 0)));
        label = new QLabel(Simcond_Dialog);
        label->setObjectName(QString::fromUtf8("label"));
        label->setGeometry(QRect(40, 60, 71, 21));
        IntervalSet = new QTimeEdit(Simcond_Dialog);
        IntervalSet->setObjectName(QString::fromUtf8("IntervalSet"));
        IntervalSet->setGeometry(QRect(130, 120, 118, 24));
        IntervalSet->setDateTime(QDateTime(QDate(2000, 1, 1), QTime(0, 30, 0)));
        label_2 = new QLabel(Simcond_Dialog);
        label_2->setObjectName(QString::fromUtf8("label_2"));
        label_2->setGeometry(QRect(40, 120, 71, 21));
        spinBox = new QSpinBox(Simcond_Dialog);
        spinBox->setObjectName(QString::fromUtf8("spinBox"));
        spinBox->setGeometry(QRect(130, 170, 121, 24));
        spinBox->setFocusPolicy(Qt::NoFocus);
        spinBox->setMinimum(10);
        spinBox->setMaximum(200);
        spinBox->setSingleStep(10);
        label_3 = new QLabel(Simcond_Dialog);
        label_3->setObjectName(QString::fromUtf8("label_3"));
        label_3->setGeometry(QRect(40, 170, 71, 21));

        retranslateUi(Simcond_Dialog);
        QObject::connect(buttonBox, SIGNAL(accepted()), Simcond_Dialog, SLOT(accept()));
        QObject::connect(buttonBox, SIGNAL(rejected()), Simcond_Dialog, SLOT(reject()));

        QMetaObject::connectSlotsByName(Simcond_Dialog);
    } // setupUi

    void retranslateUi(QDialog *Simcond_Dialog)
    {
        Simcond_Dialog->setWindowTitle(QApplication::translate("Simcond_Dialog", "Setting", nullptr));
        label->setText(QApplication::translate("Simcond_Dialog", "Time Now:", nullptr));
        label_2->setText(QApplication::translate("Simcond_Dialog", "Step:", nullptr));
        label_3->setText(QApplication::translate("Simcond_Dialog", "Speed:", nullptr));
    } // retranslateUi

};

namespace Ui {
    class Simcond_Dialog: public Ui_Simcond_Dialog {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_SIMCOND_DIALOG_H
