#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "login.h"
#include "overtimedialog.h"
#include <QDebug>

#include <QDate>

#include <QDesktopWidget>
#include <QApplication>



MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent), ui(new Ui::MainWindow), acdp(this)
{
    msgBox = new QMessageBox;
    ui->setupUi(this);

    overtimefull = 0;
    overtimehalf = 0;
    connect(ui->sendButton,SIGNAL(clicked()),this,SLOT(send()));
    connect(ui->clearButton,SIGNAL(clicked()),this,SLOT(clear()));
    connect(ui->clearDayButton,SIGNAL(clicked()),this,SLOT(clearDay()));
    connect(ui->calendarWidget,SIGNAL(currentPageChanged(int,int)),this,SLOT(refresh(int,int)));
    connect(ui->overtimeButton,SIGNAL(clicked()),this,SLOT(calcOvertimeSlot()));
    centralWidget()->setEnabled(false);

    ui->calendarWidget->setMaximumDate(QDate::currentDate());
    ui->calendarWidget->setVerticalHeaderFormat(QCalendarWidget::NoVerticalHeader);

    QPoint center = qApp->desktop()->screenGeometry(this).center();

    QRect r = rect();
    r.moveCenter(center);
    move(r.topLeft());
}

void MainWindow::calcOvertimeSlot()
{
    overtimefull = 0;
    overtimehalf = 0;
    int month = ui->calendarWidget->selectedDate().month();
    int year = ui->calendarWidget->selectedDate().year();

    int horas = 0;

    for(int i=1;i<=31;i++)
    {
        QDate date;
        date.setDate(year,month,i);
        if (date.isValid() && date.operator <=(QDate::currentDate()))
        {
            horas = 0;
            QString data = ui->calendarWidget->dateTextFormat(date).property(8228).toString();

            QRegExp rx("<td align=\"left\"><b>([0-9]+)</b></td>");

            if (rx.indexIn(data) != -1) {
                 horas = rx.cap(1).toInt();
            }
            if(date.dayOfWeek() == Qt::Sunday)
                overtimefull += horas;
            else if(date.dayOfWeek() == Qt::Saturday )
                overtimehalf += horas;
            else
            {
                if (horas > 8)
                    overtimehalf += (horas - 8);
            }
        }

    }

    QSettings settings("Mandriva", "qacdp");
    bool ok = true;
    double salary = settings.value("Acdp/Salary").toDouble(&ok);
    if(!ok)
    {
        salary = 0;
    }
    OvertimeDialog *ovtd = new OvertimeDialog(overtimefull,overtimehalf,salary,this);
    if (ovtd->exec() == QDialog::Accepted)
    {
        settings.setValue("Acdp/Salary", ovtd->getSalary());
    }
}

void MainWindow::send()
{
    bool ok = true;
    QString horas = ui->hoursLineEdit->text();
    int h = horas.toInt(&ok);
    if(!ok || h < 1 || h > 24){
        msgBox->setText("Invalid Hours field");
        msgBox->setIcon(QMessageBox::Warning);
        msgBox->setStandardButtons(QMessageBox::Ok);
        msgBox->exec();
        ui->hoursLineEdit->clear();
    }else{
        QString projectId = ui->projectBox->itemData(ui->projectBox->currentIndex()).toString();
        QString description = ui->descriptionEdit->toPlainText();
        htmlEncode(&description);
        QString shortDescription = description;
        if (description.length() > 30) {
            shortDescription.truncate(30);
            shortDescription += "...";
        }
        QString str = QString("Added %1 hours to %2 (%3)").arg(horas).arg(ui->projectBox->currentText()).arg(shortDescription);
        ui->statusLog->setText(str);
        acdp.send(projectId,horas,description);
    }
}

void MainWindow::refresh(int year,int month)
{
    QDate date;
    date.setDate(year,month,ui->calendarWidget->selectedDate().day());
    ui->calendarWidget->setSelectedDate(date);



    acdp.webRefresh();
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::login()
{
    Login l;
    QSettings settings("Mandriva", "qacdp");
    l.setUserName(settings.value("Acdp/UserName").toString());
    l.setPass(settings.value("Acdp/Password").toString());
    if (l.exec() != QDialog::Accepted)
        qApp->quit();

    QString userName = l.userName();
    QString pass = l.pass();

    if(userName.size() > 0 && pass.size() > 0)
    {
        settings.setValue("Acdp/UserName", userName);
        if(l.savePass())
            settings.setValue("Acdp/Password", pass);
        else
            settings.remove("Acdp/Password");
        acdp.login(userName,pass,ui->nomeLabel,ui->projectBox,ui->calendarWidget,msgBox);
    }


    refresh(ui->calendarWidget->selectedDate().year(),ui->calendarWidget->selectedDate().month());

}

void MainWindow::clearDay()
{

    msgBox->setStandardButtons(QMessageBox::Yes | QMessageBox::No);
    msgBox->setText("Remove all entries for this day ?");
    switch (msgBox->exec()) {
    case QMessageBox::Yes:
        acdp.clearDay();
        break;
    case QMessageBox::No:
        // no was clicked
        break;
    default:
        // should never be reached
        break;
    }

}

void MainWindow::clear()
{
    ui->hoursLineEdit->clear();
    ui->descriptionEdit->clear();
}

void MainWindow::loginOk()
{
    centralWidget()->setEnabled(true);
}

void MainWindow::htmlEncode(QString *text)
{
    text->replace(QString::fromUtf8("á"),"a");
    text->replace(QString::fromUtf8("â"),"a");
    text->replace(QString::fromUtf8("à"),"a");
    text->replace(QString::fromUtf8("ã"),"a");
    text->replace(QString::fromUtf8("ç"),"c");
    text->replace(QString::fromUtf8("é"),"e");
    text->replace(QString::fromUtf8("ê"),"e");
    text->replace(QString::fromUtf8("í"),"i");
    text->replace(QString::fromUtf8("ó"),"o");
    text->replace(QString::fromUtf8("ô"),"o");
    text->replace(QString::fromUtf8("õ"),"o");
    text->replace(QString::fromUtf8("ú"),"u");
    text->replace(QString::fromUtf8("ü"),"u");
    text->replace(QString::fromUtf8("Á"),"A");
    text->replace(QString::fromUtf8("Â"),"A");
    text->replace(QString::fromUtf8("À"),"A");
    text->replace(QString::fromUtf8("Ã"),"A");
    text->replace(QString::fromUtf8("Ç"),"C");
    text->replace(QString::fromUtf8("É"),"E");
    text->replace(QString::fromUtf8("Ê"),"E");
    text->replace(QString::fromUtf8("Í"),"I");
    text->replace(QString::fromUtf8("Ó"),"O");
    text->replace(QString::fromUtf8("Ô"),"O");
    text->replace(QString::fromUtf8("Õ"),"O");
    text->replace(QString::fromUtf8("Ú"),"U");
    text->replace(QString::fromUtf8("Ü"),"U");
}
