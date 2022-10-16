#include "weeklyreportshow.h"
//#include "ui_weeklyreportshow.h"
#include <QFileDialog>
#include <QFile>
#include <QTextStream>
#include <QMessageBox>
WeeklyReportShow::WeeklyReportShow(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::WeeklyReportShow)
{
    ui->setupUi(this);
    this->registed = ui->tableWidget_2;
    this->treated = ui->tableWidget;
    this->inqueue = ui->tableWidget_3;
    QStringList seleitem = {"Treated","Registed","In Queue"};
    ui->comboBox->addItems(seleitem);
}

WeeklyReportShow::~WeeklyReportShow()
{
    delete ui;
}
void WeeklyReportShow::show_delegate(vector<vector<std::string>*>* querymd,QTableWidget* wg,DateTime now){
    vector<string>* temp = querymd->at(0);
    QStringList titles;
    for (unsigned long j = 0 ;j < temp->size() - 2;j++){
        titles.append(QString::fromStdString(temp->at(j)));
    }
    titles.append("WAITTIME");
    wg->setColumnCount(temp->size()-1);
    wg->setHorizontalHeaderLabels(titles);
    delete temp;
    wg->setRowCount(querymd->size()-1);
    for (unsigned long i = 1; i < querymd->size(); i++){
        vector<string>* temp = querymd->at(i);
        for (unsigned long j = 0; j < temp->size()-2; j++){
            QTableWidgetItem* newItem = new QTableWidgetItem();
            newItem->setText(QString::fromStdString(temp->at(j)));
            wg->setItem(i-1,j,newItem);
        }
        DateTime tret;
        if (wg == ui->tableWidget) {
            tret = DateTime(temp->at(temp->size()-1));
        }
        else{
            tret = now;
        }
        DateTime rt = DateTime(temp->at(temp->size()-2));
        QTableWidgetItem* newItem = new QTableWidgetItem();
        newItem->setText(QString::number(tret.HourLater(rt)));
        wg->setItem(i-1,temp->size()-2,newItem);        //Calc Waiting time
        delete temp;
    }
    wg->horizontalHeader()->setSectionResizeMode(QHeaderView::ResizeToContents);
    wg->setSortingEnabled(true);
    delete querymd;
}

void WeeklyReportShow::on_pushButton_clicked()
{
    QTableWidget* wtable;
    switch (ui->comboBox->currentIndex()) {
    case 0:
        wtable = ui->tableWidget;
        break;
    case 1:
        wtable = ui->tableWidget_2;
        break;
    default:
        wtable = ui->tableWidget_3;
        break;
    }
    QString fname = QFileDialog::getSaveFileName(this,tr("Export"),"",tr("Report Files (*.csv)"));
    if (fname == "") return;
    QFile fs(fname);
    if (!fs.open(QIODevice::WriteOnly|QIODevice::Text)){
        QMessageBox::information(this, "IO","Unable To Write The File!");
        return;
    }
    QTextStream pout(&fs);
    int width = wtable->columnCount();
    int height = wtable ->rowCount();
    //QTableWidgetItem* k = wtable->horizontalHeaderItem(1);
    for (int i = 0; i < width; i ++){
        pout << wtable->horizontalHeaderItem(i)->text()<<',';
    }
    pout << '\n';
    for (int i = 0; i < height; i++){
        for (int j = 0; j < width; j++) {
            pout<<wtable->item(i,j)->text()<<',';
        }
        pout << '\n';
    }
    pout.flush();
    fs.close();
}
