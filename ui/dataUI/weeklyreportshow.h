#ifndef WEEKLYREPORTSHOW_H
#define WEEKLYREPORTSHOW_H

#include <QDialog>
#include "ui_weeklyreportshow.h"
#include "../../SupClass.h"
namespace Ui {
class WeeklyReportShow;
}

class WeeklyReportShow : public QDialog
{
    Q_OBJECT

public:
    explicit WeeklyReportShow(QWidget *parent = nullptr);
    void show_delegate(vector<vector<std::string>*>*,QTableWidget* wg,DateTime now);
    ~WeeklyReportShow();
    QTableWidget* treated;
    QTableWidget* registed;
    QTableWidget* inqueue;

private slots:
    void on_pushButton_clicked();

private:
    Ui::WeeklyReportShow *ui;
};

#endif // WEEKLYREPORTSHOW_H
