#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QtGui/QMainWindow>
#include <QMessageBox>
#include <QSettings>
#include "acdp.h"

namespace Ui
{
    class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT
    Ui::MainWindow *ui;
    ACDP acdp;
    QMessageBox *msgBox;
public:
    MainWindow(QWidget *parent = 0);
    ~MainWindow();
    void loginOk();

public slots:
    void login();

private slots:
    void send();
    void clear();
    void clearDay();
    void refresh(int year,int month);
private:

    void htmlEncode(QString *text);
};

#endif // MAINWINDOW_H
