#ifndef MONTHLYREPORT_H
#define MONTHLYREPORT_H

#include <QDialog>
#include <QLabel>
namespace Ui {
class MonthlyReport;
}

class MonthlyReport : public QDialog
{
    Q_OBJECT

public:
    explicit MonthlyReport(QWidget *parent = nullptr);
    ~MonthlyReport();
    void label_delegate(QString label, int x);

private slots:
    void on_pushButton_clicked();

private:
    Ui::MonthlyReport *ui;
    QLabel* la[7];
};

#endif // MONTHLYREPORT_H
