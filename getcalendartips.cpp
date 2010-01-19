#include "getcalendartips.h"
#include <QTextCharFormat>

#define NUMTHREADS 5

GetCalendarTips::GetCalendarTips()
{
    for (int i =0; i < NUMTHREADS; ++i)
    {
        CalendarThread *ct = new CalendarThread(this);
        connect(ct,SIGNAL(update(QString,QDate)),this,SLOT(update(QString,QDate)));
        calendarThreads.append(ct);
    }
}

void GetCalendarTips::setCalendar(QCalendarWidget *cal, QString &ses, QString &i)
{
    calendar = cal;
    session = ses;
    id = i;
    foreach(CalendarThread *ct, calendarThreads)
        ct->setVars(id.toInt(),session);
}

void GetCalendarTips::update(QString text,QDate date){
    QBrush brush;
    QTextCharFormat fmt;
    QRegExp regexHoras("<td align=\"left\">Total</td>.*<td align=\"left\"><b>([0-9]*)</b></td>");
    if(text.contains(regexHoras))
    {
        int horas = regexHoras.cap(1).toInt();
        if(horas < 8)
            brush.setColor(QColor(255, 255, 0, 140));
        else if(horas == 8)
            brush.setColor(QColor(124, 252, 0, 140));
        else
            brush.setColor(QColor(124, 252, 0, 240));
    }
    else
    {
        if(date.dayOfWeek() == 6 || date.dayOfWeek() == 7 )
            brush.setColor(QColor(84, 84, 84, 40));
        else
            brush.setColor(QColor(255, 0, 0, 40));
        text = "No entries";
    }

    fmt.setBackground( brush );
    fmt.setToolTip( text );
    calendar->setDateTextFormat( date, fmt );
    jobsDone.append(date);
}

void GetCalendarTips::enqueueJob(const QDate &date)
{
    if(!jobsDone.contains(date))
    {
        if(!jobs.contains(date))
        {
            jobs.enqueue(date);
        }
    }
}

QDate GetCalendarTips::dequeueJob()
{
    QDate date;
    mutex.lock();
    if (!jobs.isEmpty())
        date = jobs.dequeue();
    mutex.unlock();
    return date;
}

void GetCalendarTips::startProcessing()
{
    foreach(CalendarThread *ct, calendarThreads)
        ct->start();
}
