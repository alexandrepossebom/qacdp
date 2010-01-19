#ifndef GETCALENDARTIPS_H
#define GETCALENDARTIPS_H

#include <QObject>
#include "calendarthread.h"
#include <QHttp>
#include <QUrl>
#include <QDebug>
#include <QQueue>
#include <QTimer>
#include <QDate>
#include <QCalendarWidget>
#include <QMutex>

class GetCalendarTips : public QObject
{
    Q_OBJECT
    QList<CalendarThread*> calendarThreads;
    QCalendarWidget *calendar;
    QString session;
    QString id;
    QList<QDate> jobsDone;
    QQueue<QDate> jobs;
    QMutex mutex;
public slots:
    void update(QString text,QDate date);

public:
    GetCalendarTips();
    void enqueueJob(const QDate &date);
    void setCalendar(QCalendarWidget *cal,QString &session,QString &id);
    QDate dequeueJob();
    void startProcessing();
};

#endif // GETCALENDARTIPS_H
