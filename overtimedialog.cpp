#include "overtimedialog.h"
#include "ui_overtimedialog.h"

OvertimeDialog::OvertimeDialog(int overtimefull, int overtimehalf, double salary,QWidget *parent) :
    QDialog(parent),
    ui(new Ui::OvertimeDialog)
{
    ui->setupUi(this);

    m_full = overtimefull;
    m_half = overtimehalf;
    m_salary = salary;

    connect(ui->doubleSpinBox,SIGNAL(valueChanged(double)),this,SLOT(salaryChangedSlot(double)));

    ui->overtimeFull->setText(QString::number(overtimefull));
    ui->overtimeHalf->setText(QString::number(overtimehalf));
    ui->total->setText("R$ 0,00");
    if(salary > 0)
    {
        ui->doubleSpinBox->setValue(salary);
        salaryChangedSlot(salary);
    }
}

double OvertimeDialog::getSalary()
{
    return m_salary;
}

void OvertimeDialog::salaryChangedSlot(double salary)
{
    m_salary = salary;
    double half = 0;
    double full = 0;

    double salarybyhour = salary / 200;

    half = (salarybyhour * 1.5) * m_half;
    full = (salarybyhour * 2) * m_full;

    ui->total->setText(QString("R$ %1").arg(QString::number(half+full,'F',2)));

}

OvertimeDialog::~OvertimeDialog()
{
    delete ui;
}

void OvertimeDialog::changeEvent(QEvent *e)
{
    QDialog::changeEvent(e);
    switch (e->type()) {
    case QEvent::LanguageChange:
        ui->retranslateUi(this);
        break;
    default:
        break;
    }
}
