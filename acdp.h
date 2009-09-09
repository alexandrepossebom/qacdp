#ifndef ACDP_H
#define ACDP_H

#include <QObject>
#include <QHttp>
#include <QDomDocument>
#include <QComboBox>
#include <QLabel>
#include <QCalendarWidget>
#include <QWebView>


class ACDP : public QObject
{
    Q_OBJECT
    QString passwd;
    QString user;
    QHttp httpLogin;
    QHttp httpProjetos;
    QHttp httpSend;
    QString session;
    QDomDocument dom;
    QComboBox *projectBox;
    QString nome;
    QString id;
    QLabel *nomeLabel;
    QWebView *webview;
    QCalendarWidget *calendar;

public:
    ACDP();
    void login(QString user, QString passwd, QLabel *nomeLabel, QComboBox *projectBox, QWebView *webview,QCalendarWidget *calendar);
    void send(QString project_id,QString horas,QString description);
    void webRefresh();

private slots:
    bool loginDone(bool);
    void readResponseHeader(const QHttpResponseHeader&);
    void projetosDone(bool);
    void sendDone(bool);

};

#endif // ACDP_H
