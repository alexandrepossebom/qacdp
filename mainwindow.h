#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QtGui/QMainWindow>
#include <QMessageBox>
#include "acdp.h"

namespace Ui
{
    class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT
    ACDP acdp;
    QMessageBox *msgBox;
public:
    MainWindow(QWidget *parent = 0);
    ~MainWindow();
public slots:
    void login();
private slots:
    void send();
    void clear();
    void clearDay();
    void refresh(int year,int month);
private:
    Ui::MainWindow *ui;
};

#endif // MAINWINDOW_H
