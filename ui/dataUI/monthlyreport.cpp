#include "monthlyreport.h"
#include "ui_monthlyreport.h"
#include <QFile>
#include <QFileDialog>
#include <QTextStream>
#include <QMessageBox>
MonthlyReport::MonthlyReport(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::MonthlyReport)
{
    ui->setupUi(this);
    la[0] = ui->label;
    la[1] = ui->label_2;
    la[2] = ui->label_3;
    la[3] = ui->label_4;
    la[4] = ui->label_5;
    la[5] = ui->label_6;
}

MonthlyReport::~MonthlyReport()
{
    delete ui;
}

void MonthlyReport::label_delegate(QString label, int x){
    la[x]->setText(label);
    la[x]->adjustSize();
}

void MonthlyReport::on_pushButton_clicked()
{
    QString fname = QFileDialog::getSaveFileName(this,tr("Export"),"",tr("Report Files (*.csv)"));
    if (fname == "") return;
    QFile fs(fname);
    if (!fs.open(QIODevice::WriteOnly|QIODevice::Text)){
        QMessageBox::information(this, "IO","Unable To Write The File!");
        return;
    }
    QTextStream pout(&fs);
    //QTableWidgetItem* k = wtable->horizontalHeaderItem(1);
    for (int i = 0; i < 6; i++){
        pout <<la[i]->text()<<",\n";
    }
    pout.flush();
    fs.close();
}
