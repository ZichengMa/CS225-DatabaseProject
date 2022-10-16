#include "dataui.h"
#include "ui_dataui.h"
#include "simcond_dialog.h"
#include <QFileDialog>
#include "../../Acsctrl.h"
#include "../../SupClass.h"
#include "weeklyreportshow.h"
#include "monthlyreport.h"
using namespace std;
dataUI::dataUI(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::dataUI)
{
    ui->setupUi(this);
    this->run_symbol = new QTimer();
    this->sim_drive = new QTimer();
    connect(run_symbol,SIGNAL(timeout()),this,SLOT(pon_run_symbol_timeout()));
    connect(sim_drive,SIGNAL(timeout()),this,SLOT(pon_sim_driven_timeout()));
    ini_table();
    this->cond.now = DateTime(1970,1,1,0,0,0);
    this->cond.step = 3600;
    this->sim_timer = 100;
    this->sim = new Simulator(cond);
    this->sim_drive->setInterval(1000/sim_timer);
    ui->NowDisp->setText(QString::fromStdString(cond.now.ToString()));
}

dataUI::~dataUI()
{
    delete ui;
    delete run_symbol;
    delete sim_drive;
    delete sim;
}
void dataUI::pon_run_symbol_timeout()
{
    static int dotnum = 0;
    if (dotnum == 0) ui->RunLabel->setText("Running.");
    if (dotnum == 1) ui->RunLabel->setText("Running..");
    if (dotnum == 2) ui->RunLabel->setText("Running...");
    dotnum = (dotnum+1)%3;
}
void dataUI::pon_sim_driven_timeout()
{
    Sim_Status u = sim->Tick();
    cond.now.AddSecond(cond.step);
    ui->NowDisp->setText(QString::fromStdString(u.now));
    for (unsigned long i = 0; i < u.status_add->size();i++){
        note(QString::fromStdString(u.status_add->at(i)));
    }
    delete u.status_add;
}
void dataUI::ini_table()
{
//    QStringList titles;
//    titles.append("No.");
//    titles.append("ID");
//    ui->tableWidget->setColumnCount(2);
//    ui->tableWidget->setHorizontalHeaderLabels(titles);
}

void dataUI::on_IncButton_clicked()
{
//    static int p = 0;
//    int rowc = ui->tableWidget->rowCount();
//    QTableWidgetItem* newItem = new QTableWidgetItem();
//    newItem->setText(QString::number(p));
//    ui->tableWidget->setRowCount(rowc+1);
//    ui->tableWidget->setItem(rowc,0,newItem);
//    p++;
    report_ps(ui->QueryInput->text());
}

void dataUI::on_SimruntButton_clicked()
{
    static bool Isrunning = false;
    Isrunning = !Isrunning;
    if (Isrunning) {
        ui->SimruntButton->setText("Stop");
        pon_run_symbol_timeout();
        run_symbol->start(1000);
        sim_drive->start();
    }else{
        ui->SimruntButton->setText("Run");
        ui->RunLabel->setText("Stopped");
        run_symbol->stop();
        sim_drive->stop();
    }
}

void dataUI::on_Conditionset_button_clicked()
{
    Simcond_Dialog *sdp = new Simcond_Dialog(this);
    sdp->sim_interval = sim_timer;
    sdp->rc = cond;
    sdp->Info_ini_Delegate();
    if (sdp->exec() == QDialog::Accepted) {
        cond = sdp->rc;
        sim->UpdateCond(cond);
        ui->NowDisp->setText(QString::fromStdString(cond.now.ToString()));
        sim_timer = sdp->sim_interval;
        this->sim_drive->setInterval(1000/sim_timer);
        note("Time Set:"+QString::fromStdString(cond.now.ToString()));
        note("Time Step Set:"+QString::number(cond.step)+" seconds");
        note("Speed Set:"+QString::number(sim_timer)+" Hz");
    }
    delete sdp;
}

void dataUI::on_pushButton_clicked()
{
    ui->NoteList->clear();
}

void dataUI::on_QueryInput_returnPressed()
{
    on_IncButton_clicked();
}

void dataUI::on_Clear_pb_clicked()
{
    ui->tableWidget->clear();
    ui->tableWidget->setRowCount(0);
    ui->tableWidget->setColumnCount(0);
}

void dataUI::on_LoadNew_clicked()
{
    QString fpath = QFileDialog::getOpenFileName(this,"Open","","New_Records(*.csv)");
    if (fpath != ""){
        switch (Acsctrl::Put(fpath.toStdString())) {
        case 1:
            note("File Not Found: "+fpath);
            return;
            break;
        case 2:
            note("Failed To Load: "+fpath);
            return;
            break;
        default:
            break;
        }
        note("File Loaded: "+fpath);
    }
}

void dataUI::on_UpdateRec_clicked()
{
    QString fpath = QFileDialog::getOpenFileName(this,"Open","","Update_Records(*.csv)");
    if (fpath != ""){
        switch (Acsctrl::UpdateData(fpath.toStdString())) {
        case 1:
            note("File Not Found: "+fpath);
            return;
            break;
        case 2:
            note("Failed To Load: "+fpath);
            return;
            break;
        default:
            break;
        }
        note("File Loaded: "+fpath);
    }
}

void dataUI:: note(QString msg){
    ui->NoteList->addItem(msg);
    ui->NoteList->setCurrentRow(ui->NoteList->count()-1);
    ui->NoteList->clearSelection();
}

void dataUI::on_Step_pb_clicked()
{
    pon_sim_driven_timeout();
}

void dataUI::report_ps(QString query)
{
    on_Clear_pb_clicked();
    vector<vector<string>*>* querymd = Acsctrl::Search(query.toStdString());
    if (querymd != nullptr){
        if (querymd->size() == 1) {
            note("No Results Found!");
            ui->QueryInput->clear();
            return;
        }
        vector<string>* temp = querymd->at(0);
        QStringList titles;
        for (unsigned long j = 0 ;j < temp->size();j++){
            titles.append(QString::fromStdString(temp->at(j)));
        }
        ui->tableWidget->setColumnCount(temp->size());
        ui->tableWidget->setHorizontalHeaderLabels(titles);
        delete temp;
        ui->tableWidget->setRowCount(querymd->size()-1);
        for (unsigned long i = 1; i < querymd->size(); i++){
            vector<string>* temp = querymd->at(i);
            for (unsigned long j = 0; j < temp->size(); j++){
                QTableWidgetItem* newItem = new QTableWidgetItem();
                newItem->setText(QString::fromStdString(temp->at(j)));
                ui->tableWidget->setItem(i-1,j,newItem);
            }
            delete temp;
        }
        ui->tableWidget->horizontalHeader()->setSectionResizeMode(QHeaderView::ResizeToContents);
        note(QString::number(querymd->size()-1)+" Result(s) Found!");
        delete querymd;
    }else{
        note("Invalid Query!");
    }
    ui->QueryInput->clear();

}
void dataUI::on_WeekRP_pb_clicked()
{
    DateTime p = cond.now;
    p.AddSecond(-604800);
    WeeklyReportShow* wshow = new WeeklyReportShow(this);
    wshow->show_delegate(Acsctrl::Search("from record select (name,prof,agegp,riskgp,regtime,tretime) where (status == 3)"),wshow->treated,cond.now);
    wshow->show_delegate(Acsctrl::Search("from record select (name,prof,agegp,riskgp,regtime,tretime) where (status == 0)"),wshow->registed,cond.now);
    wshow->show_delegate(Acsctrl::Search("from record select (name,prof,agegp,riskgp,regtime,tretime) where (status == 1)"),wshow->inqueue,cond.now);
    wshow->exec();
    delete wshow;
    //report_ps("from record select (all) where ()");
}

void dataUI::on_Monthrp_pb_clicked()
{
    MonthlyReport* mr = new MonthlyReport(this);
    DateTime monago = cond.now;
    monago.AddSecond(-2592000);
    vector<vector<string>*>* reged = Acsctrl::Search("from record select (uid) where (\""+ monago.ToString()+"\"<= regtime <= \"" + cond.now.ToString()+"\")");
    mr->label_delegate("Registed in 30 days: "+QString::number(reged->size()-1),0);
    recycle(reged);
    reged = Acsctrl::Search("from record select (uid) where ((\""+ monago.ToString()+"\"<= regtime <= \"" + cond.now.ToString()+"\") and ((status == 1) or (status == 2) or (status == 0)))");
    mr->label_delegate("Waiting (Registed in 30 days): "+QString::number(reged->size()-1),1);
    recycle(reged);
    reged = Acsctrl::Search("from record select (uid) where ((status == 1) or (status == 2) or (status == 0))");
    mr->label_delegate("Waiting (Total): "+QString::number(reged->size()-1),2);
    recycle(reged);
    reged = Acsctrl::Search("from record select (uid) where ((status == 3) or (status == 2))");
    mr->label_delegate("Assigned (Total): "+QString::number(reged->size()-1),3);
    recycle(reged);
    reged = Acsctrl::Search("from record select (regtime,tretime) where (status == 3)");
    int count = reged->size() -1;
    int hournum = 0;
    delete reged->at(0);
    for (unsigned long i = 1; i < reged->size(); i++){
        vector<string>* tp = reged->at(i);
        hournum += DateTime(tp->at(1)).HourLater(DateTime(tp->at(0)));
        delete tp;
    }
    delete reged;
    mr->label_delegate("Average Waiting Time (Hr): "+QString::number((double)hournum/count,'f',2),4);
    reged = Acsctrl::Search("from record select (uid) where (status == 4)");
    mr->label_delegate("Withdrawn (Total): "+QString::number(reged->size()-1),5);
    recycle(reged);
    mr->exec();
    delete mr;
    //from record select (uid) where ("1970/01/01/00/00/00" <=regtime <="1970/01/04/19/00/00")
    //from record select (uid) where (("1970/01/01/00/00/00" <=regtime <="1970/01/04/19/00/00") and ((status == 1) or (status == 2) or (status == 0)))
    //from record select (uid) where ((status == 1) or (status == 2) or (status == 0))
    //from record select (uid) where ((status == 2) or (status == 3))
    //from record select (regtime,tretime) where (status == 3)
    //from record select (uid) where (status == 4)
    //from record select (uid) where ((status == 1) or (status == 2))


}
void dataUI::recycle(vector<vector<string>*>* x) {
    for (unsigned long i = 0; i < x->size(); i++){
        delete x->at(i);
    }
    delete x;
}
