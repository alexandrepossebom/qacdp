#ifndef CALENDARTHREAD_H
#define CALENDARTHREAD_H

#include <QThread>
#include <QDate>
#include <QHttp>
#include <QUrl>
#include <QQueue>

class GetCalendarTips;

class CalendarThread : public QThread
{
    Q_OBJECT
    QUrl m_url;
    QString m_session;
    int m_id;
    bool gotResult;
    GetCalendarTips *m_tips;
    QDate m_date; // just to know the date being processed
public:
    CalendarThread(GetCalendarTips *tips);
    void setVars(int id,QString session);
    void run();
public slots:
    void process(bool error);
signals:
    void update(QString text,QDate date);
};

#endif // CALENDARTHREAD_H
