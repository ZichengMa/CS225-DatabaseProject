#ifndef SIMCOND_DIALOG_H
#define SIMCOND_DIALOG_H

#include <QDialog>
#include "../../Simulator.h"
namespace Ui {
class Simcond_Dialog;
}

class Simcond_Dialog : public QDialog
{
    Q_OBJECT

public:
    explicit Simcond_Dialog(QWidget *parent = nullptr);
    ~Simcond_Dialog();
    Sim_Condition rc;
    int sim_interval;
    void Info_ini_Delegate();
private slots:
    void on_buttonBox_accepted();

private:
    Ui::Simcond_Dialog *ui;
};

#endif // SIMCOND_DIALOG_H
