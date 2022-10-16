#include "simcond_dialog.h"
#include "ui_simcond_dialog.h"
#include "../../Simulator.h"

Simcond_Dialog::Simcond_Dialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::Simcond_Dialog)
{
    ui->setupUi(this);
}

Simcond_Dialog::~Simcond_Dialog()
{
    delete ui;
}

void Simcond_Dialog::Info_ini_Delegate(){
    ui->NowSet->setDateTime(QDateTime::fromString(QString::fromStdString(rc.now.ToString()),"yyyy/MM/dd/hh/mm/ss"));
    ui->IntervalSet->setTime(QTime(0,0,0,0).addSecs(rc.step));
    ui->spinBox->setValue(sim_interval);
}
void Simcond_Dialog::on_buttonBox_accepted()
{
    QDate nowd = ui->NowSet->date();
    QTime now = ui->NowSet->time();
    rc.step = ui->IntervalSet->time().msecsSinceStartOfDay()/1000;
    rc.now = DateTime(nowd.year(),nowd.month(),nowd.day(),now.hour(),now.minute(),now.second());
    sim_interval = ui->spinBox->value();
}
