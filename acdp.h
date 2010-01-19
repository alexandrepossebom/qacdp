#ifndef ACDP_H
#define ACDP_H

#include <QObject>
#include <QHttp>
#include <QDomDocument>
#include <QComboBox>
#include <QLabel>
#include <QCalendarWidget>
#include <QWebView>
#include <QMessageBox>
#include "getcalendartips.h"

class MainWindow;

class ACDP : public QObject
{
    Q_OBJECT
    QString passwd;
    QString user;
    QHttp httpLogin;
    QHttp httpProjetos;
    QHttp httpSend;
    QHttp httpWebView;
    QHttp httpClear;
    QDomDocument dom;
    QComboBox *projectBox;
    QString nome;

    QLabel *nomeLabel;
    QCalendarWidget *calendar;
    QMessageBox *msgBox;
    GetCalendarTips gct;

public:
    ACDP(MainWindow *win);
    void login(QString user, QString passwd, QLabel *nomeLabel, QComboBox *projectBox,
               QCalendarWidget *calendar, QMessageBox *msgBox);
    void send(QString project_id,QString horas,QString description);
    void webRefresh();
    void clearDay();
    bool loginFinished;
    QString session;
    QString id;
    void updateCalendar(int month,int year);

private slots:
    void loginDone(bool);
    void loginHeader(const QHttpResponseHeader&);
    void projetosDone(bool);
    void sendDone(bool);
    void clearDone(bool);

private:
    MainWindow *m_win;

};


#endif // ACDP_H
