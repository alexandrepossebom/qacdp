#include "calendarthread.h"
#include "getcalendartips.h"
#include <QDebug>

CalendarThread::CalendarThread(GetCalendarTips *tips)
    : m_tips(tips)
{   
}

void CalendarThread::setVars(int id,QString session)
{
    m_session = session;
    m_id = id;
}

void CalendarThread::process(bool error)
{
    QHttp *http = qobject_cast<QHttp*>(sender());
    if (!http)
        return;
    if(!error){
        QString text;
        text.append(http->readAll());
        text = text.remove(QRegExp(".*<table width=\"85%\" class=\"list\" align=\"center\">"));
        text = text.remove(QRegExp("<br><br>.*"));
        emit update(text,m_date);
    }
    QDate date = m_tips->dequeueJob();
    if (!date.isValid())
        exit();
    m_date = date;
    QString m_url("/acdp/relatorio.php?action=day&person_id=");
    m_url.append(QString::number(m_id));
    m_url.append("&report_day=").append(QString::number(date.day()));
    m_url.append("&report_month=").append(QString::number(date.month()));
    m_url.append("&report_year=").append(QString::number(date.year()));
    m_url.append("&").append(m_session);
    http->get(m_url);
}

void CalendarThread::run()
{
    QDate date = m_tips->dequeueJob();
    if (!date.isValid())
        return;
    m_date = date;
    QHttp m_http;
    connect(&m_http,SIGNAL(done(bool)),this,SLOT(process(bool)),Qt::BlockingQueuedConnection);
    connect(&m_http, SIGNAL(sslErrors(QList<QSslError>)),&m_http, SLOT(ignoreSslErrors()));
    gotResult = false;
    m_http.setHost("acdp.mandriva.com.br",QHttp::ConnectionModeHttps);

    QString m_url("/acdp/relatorio.php?action=day&person_id=");
    m_url.append(QString::number(m_id));
    m_url.append("&report_day=").append(QString::number(date.day()));
    m_url.append("&report_month=").append(QString::number(date.month()));
    m_url.append("&report_year=").append(QString::number(date.year()));
    m_url.append("&").append(m_session);
    m_http.get(m_url);

    exec();
}
