#include <QtGui/QApplication>
#include <QTimer>
#include "mainwindow.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MainWindow w;
    w.show();
    QTimer::singleShot(0, &w, SLOT(login()));
    return a.exec();
}
