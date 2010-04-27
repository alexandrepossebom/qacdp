#ifndef OVERTIMEDIALOG_H
#define OVERTIMEDIALOG_H

#include <QDialog>

namespace Ui {
    class OvertimeDialog;
}

class OvertimeDialog : public QDialog {
    Q_OBJECT
public:
    OvertimeDialog(int overtimefull = 0,int overtimehalf = 0, double salary = 0,QWidget *parent = 0);
    ~OvertimeDialog();
    double getSalary();

protected:
    void changeEvent(QEvent *e);

private:
    Ui::OvertimeDialog *ui;
    int m_half;
    int m_full;
    double m_salary;
private slots:
    void salaryChangedSlot(double salary);
};

#endif // OVERTIMEDIALOG_H
