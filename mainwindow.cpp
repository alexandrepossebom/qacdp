#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "login.h"
#include <QDebug>

#include <QDate>

#include <QDesktopWidget>
#include <QApplication>



MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent), ui(new Ui::MainWindow), acdp(this)
{
    msgBox = new QMessageBox;
    ui->setupUi(this);
    connect(ui->sendButton,SIGNAL(clicked()),this,SLOT(send()));
    connect(ui->clearButton,SIGNAL(clicked()),this,SLOT(clear()));
    connect(ui->clearDayButton,SIGNAL(clicked()),this,SLOT(clearDay()));
    connect(ui->calendarWidget,SIGNAL(currentPageChanged(int,int)),this,SLOT(refresh(int,int)));
    centralWidget()->setEnabled(false);

    ui->calendarWidget->setMaximumDate(QDate::currentDate());
    ui->calendarWidget->setVerticalHeaderFormat(QCalendarWidget::NoVerticalHeader);

    QPoint center = qApp->desktop()->screenGeometry(this).center();

    QRect r = rect();
    r.moveCenter(center);
    move(r.topLeft());
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
    if (l.exec() != QDialog::Accepted)
        qApp->quit();


    QString userName = l.userName();
    QString pass = l.pass();

    if(userName.size() > 0 && pass.size() > 0)
    {
        settings.setValue("Acdp/UserName", userName);
        acdp.login(userName,pass,ui->nomeLabel,ui->projectBox,ui->webView,ui->calendarWidget,msgBox);
    }
}

void MainWindow::clearDay()
{

    msgBox->setStandardButtons(QMessageBox::Yes | QMessageBox::No);
    msgBox->setText("Remove all entries for thi day ?");
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
