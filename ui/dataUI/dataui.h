#ifndef DATAUI_H
#define DATAUI_H

#include <QWidget>
#include <QTimer>
#include "../../SupClass.h"
#include "../../Simulator.h"
QT_BEGIN_NAMESPACE
namespace Ui { class dataUI; }
QT_END_NAMESPACE

class dataUI : public QWidget
{
    Q_OBJECT

public:
    dataUI(QWidget *parent = nullptr);
    ~dataUI();
private slots:
    void on_IncButton_clicked();

    void on_SimruntButton_clicked();

    void pon_run_symbol_timeout();

    void pon_sim_driven_timeout();

    void on_Conditionset_button_clicked();

    void on_pushButton_clicked();

    void on_QueryInput_returnPressed();

    void on_Clear_pb_clicked();

    void on_LoadNew_clicked();

    void on_UpdateRec_clicked();

    void on_Step_pb_clicked();

    void on_WeekRP_pb_clicked();

    void on_Monthrp_pb_clicked();

private:
    Ui::dataUI *ui;
    QTimer *run_symbol;
    QTimer *sim_drive;
    void ini_table();
    Sim_Condition cond;
    Simulator* sim;
    void note(QString msg);
    void report_ps(QString query);
    int sim_timer;
    void recycle(vector<vector<string>*>*);
};
#endif // DATAUI_H
