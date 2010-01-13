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

class MainWindow;


class ACDP : public QObject
{
    Q_OBJECT
    QString passwd;
    QString user;
    QHttp httpLogin;
    QHttp httpProjetos;
    QHttp httpSend;
    QHttp httpClear;
    QDomDocument dom;
    QComboBox *projectBox;
    QString nome;
    QString id;
    QLabel *nomeLabel;
    QWebView *webview;
    QCalendarWidget *calendar;
    QMessageBox *msgBox;

public:
    ACDP(MainWindow *win);
    void login(QString user, QString passwd, QLabel *nomeLabel, QComboBox *projectBox,
               QWebView *webview,QCalendarWidget *calendar, QMessageBox *msgBox);
    void send(QString project_id,QString horas,QString description);
    void webRefresh();
    void clearDay();
    bool loginFinished;
    QString session;

private slots:
    bool loginDone(bool);
    void readResponseHeader(const QHttpResponseHeader&);
    void projetosDone(bool);
    void sendDone(bool);
    void clearDone(bool);
private:
    MainWindow *m_win;

};

#endif // ACDP_H
