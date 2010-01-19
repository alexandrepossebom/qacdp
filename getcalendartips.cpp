#include "getcalendartips.h"
#include <QTextCharFormat>

GetCalendarTips::GetCalendarTips()
{
    interval = 50;
    done = true;
    connect(&timer, SIGNAL(timeout()), this, SLOT(slotProcessQueue()));
    connect(&http, SIGNAL(done(bool)), this, SLOT(update(bool)));
    connect(&http, SIGNAL(sslErrors(QList<QSslError>)),&http, SLOT(ignoreSslErrors()));
}
void GetCalendarTips::setCalendar(QCalendarWidget *cal, QString &ses, QString &i)
{
    calendar = cal;
    session = ses;
    id = i;
}

void GetCalendarTips::slotProcessQueue()
{
    if(jobs.isEmpty()){
        timer.stop();
        return;
    }
    if (done)
    {
        done = false;
        currentDate = jobs.dequeue();

        http.setHost("acdp.mandriva.com.br",QHttp::ConnectionModeHttps);
        QString url("/acdp/relatorio.php?action=day&person_id=");
        url.append(id);
        url.append("&report_day=").append(QString::number(currentDate.day()));
        url.append("&report_month=").append(QString::number(currentDate.month()));
        url.append("&report_year=").append(QString::number(currentDate.year()));
        url.append("&").append(session);

        http.get(url);
    }
}

void GetCalendarTips::update(bool error){
    if(!error){
        QString text;
        text.append(http.readAll());
        QTextCharFormat fmt;
        QBrush brush;
        text = text.remove(QRegExp(".*<table width=\"85%\" class=\"list\" align=\"center\">"));
        text = text.remove(QRegExp("<br><br>.*"));
        QRegExp regexHoras("<td align=\"left\">Total</td>.*<td align=\"left\"><b>([0-9]*)</b></td>");
        if(text.contains(regexHoras))
        {
            int horas = regexHoras.cap(1).toInt();
            if(horas < 8)
                brush.setColor(QColor(255, 255, 0, 140));
            else if(horas == 8)
                brush.setColor(QColor(124, 252, 0, 140));
            else
                brush.setColor(QColor(0, 0, 255, 140));
        }
        else
        {
            if(currentDate.dayOfWeek() == 6 || currentDate.dayOfWeek() == 7 )
                brush.setColor(QColor(84, 84, 84, 40));
            else
                brush.setColor(QColor(255, 0, 0, 40));
            text = "No entries";
        }
        fmt.setBackground( brush );
        fmt.setToolTip( text );
        calendar->setDateTextFormat( currentDate, fmt );
        jobsDone.append(currentDate);
    }
    done = true;
}

void GetCalendarTips::enqueueJob(const QDate &date)
{
    if(!jobsDone.contains(date))
    {
        if(!jobs.contains(date))
        {
            jobs.enqueue(date);
            if(!timer.isActive())
                timer.start(interval);
        }
    }
}
