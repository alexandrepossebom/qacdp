#include "getcalendartips.h"
#include <QTextCharFormat>


#define NUMTHREADS 5

GetCalendarTips::GetCalendarTips()
{
    settings = new QSettings("Mandriva","qacdp");
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
    if(text == settings->value( QString("Cache/%1").arg(date.toString(Qt::ISODate)) ).toString())
    {
        jobsDone.append(date);
        return;
    }
    settings->setValue(QString("Cache/%1").arg(date.toString(Qt::ISODate)), text);
    updateColors(date,text);
    jobsDone.append(date);
}

void GetCalendarTips::enqueueJob(const QDate &date,bool force)
{
    QString text = settings->value( QString("Cache/%1").arg(date.toString(Qt::ISODate)) ).toString();
    if(text.length() > 0)
        updateColors(date,text);

    if(!jobsDone.contains(date) || force)
    {
        if(!jobs.contains(date) || force)
        {
            jobs.enqueue(date);
        }
    }
}

void  GetCalendarTips::updateColors(const QDate &date,QString &text)
{
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
        if(date.dayOfWeek() == Qt::Saturday || date.dayOfWeek() == Qt::Sunday )
            brush.setColor(QColor(84, 84, 84, 40));
        else
            brush.setColor(QColor(255, 0, 0, 40));
        text = "No entries";
    }

    fmt.setBackground( brush );
    fmt.setToolTip( text );
    calendar->setDateTextFormat( date, fmt );
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
