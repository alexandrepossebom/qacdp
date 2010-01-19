#ifndef GETCALENDARTIPS_H
#define GETCALENDARTIPS_H

#include <QObject>
#include <QHttp>
#include <QUrl>
#include <QDebug>
#include <QQueue>
#include <QTimer>
#include <QDate>
#include <QCalendarWidget>

class GetCalendarTips : public QObject
{
    Q_OBJECT
    QHttp http;
    QTimer timer;
    QDate currentDate;
    QCalendarWidget *calendar;
    QString session;
    QString id;
    int interval;
    bool done;
    QList<QDate> jobsDone;
    QQueue<QDate> jobs;
public slots:
    void slotProcessQueue();
    void update(bool error);

public:
    GetCalendarTips();
    void enqueueJob(const QDate &date);
    void setCalendar(QCalendarWidget *cal,QString &session,QString &id);
};

#endif // GETCALENDARTIPS_H
