#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "login.h"
#include <QDebug>

#include <QDate>


MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent), ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    connect(ui->sendButton,SIGNAL(clicked()),this,SLOT(send()));
    connect(ui->calendarWidget,SIGNAL(currentPageChanged(int,int)),this,SLOT(refresh(int,int)));
}

void MainWindow::send()
{
    QString horas = ui->hoursLineEdit->text();
    QString projectId = ui->projectBox->itemData(ui->projectBox->currentIndex()).toString();
    QString description = ui->descriptionEdit->toPlainText();
    acdp.send(projectId,horas,description);


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

    if (l.exec() != QDialog::Accepted)
        qApp->quit();

    QString userName = l.userName();
    QString pass = l.pass();

    if(userName.size() > 0 && pass.size() > 0)
        acdp.login(userName,pass,ui->nomeLabel,ui->projectBox,ui->webView,ui->calendarWidget);
}


