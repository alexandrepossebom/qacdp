#include "acdp.h"
#include <QDebug>
#include <QUrl>
#include <QDate>
#include <QTimer>
#include <QRegExp>

ACDP::ACDP()
{
    projectBox = NULL;
    connect(&httpLogin,SIGNAL(done(bool)),this,SLOT(loginDone(bool)));
    connect(&httpLogin, SIGNAL(responseHeaderReceived(const QHttpResponseHeader &)),
            this, SLOT(readResponseHeader(const QHttpResponseHeader &)));
    connect(&httpProjetos,SIGNAL(done(bool)),this,SLOT(projetosDone(bool)));
    connect(&httpSend,SIGNAL(done(bool)),this,SLOT(sendDone(bool)));
}

void ACDP::login(QString user,QString passwd,QLabel *nomeLabel, QComboBox *projectBox, QWebView *webview, QCalendarWidget *calendar,QMessageBox *msgBox)
{
    this->user = user;
    this->passwd = passwd;
    this->projectBox = projectBox;
    this->nomeLabel = nomeLabel;
    this->webview = webview;
    this->calendar = calendar;
    this->msgBox = msgBox;

    QByteArray content = "";
    content.append("action=login&GoAheadAndLogIn=Login&user=");
    content.append(user);
    content.append("&passwd=");
    content.append(passwd);


    QHttpRequestHeader header("POST", "/acdp/login.php");
    header.setValue("Host", "ep.conectiva");
    header.setContentType("application/x-www-form-urlencoded");
    header.setContentLength(content.length());

    httpLogin.setHost("ep.conectiva");
    httpLogin.request(header, content);
}

bool ACDP::loginDone(bool error)
{
    if(!error){
        QByteArray content = "";
        content.append("xml=true");
        content.append("&");
        content.append(session);

        QHttpRequestHeader header("POST", "/acdp/horas_projeto.php");
        header.setValue("Host", "ep.conectiva");
        header.setContentType("application/x-www-form-urlencoded");
        header.setContentLength(content.length());

        httpProjetos.setHost("ep.conectiva");
        httpProjetos.request(header, content);

        webRefresh();
        return true;
    }
    return false;
}

void ACDP::projetosDone(bool error)
{
    if(!error){
        dom.setContent(httpProjetos.readAll());
        QDomElement element = dom.documentElement();
        
        QVariant projetoId("9");
        QString projetoString = "Internal Hours";
        projectBox->addItem(projetoString,projetoId);

        for(QDomNode n = element.firstChild(); !n.isNull(); n = n.nextSibling())
        {
            QVariant projetoId(n.toElement().firstChildElement("code").text());
            QString projetoString = n.toElement().firstChildElement("cliente").text();
            projetoString.append(" - ");
            projetoString.append(n.toElement().firstChildElement("title").text());
            projectBox->addItem(projetoString,projetoId);
            id = n.toElement().firstChildElement("person_id").text();
            nome = n.toElement().firstChildElement("person_name").text();
        }
        if(nome.size() > 0)
            nomeLabel->setText(nome);
    }
}

void ACDP::readResponseHeader(const QHttpResponseHeader &responseHeader)
{
    session = responseHeader.value("set-cookie").section(";",0,0);
    if( responseHeader.toString().indexOf("location") == -1 )
    {
        msgBox->setIcon(QMessageBox::Warning);
        msgBox->setText("Invalid user or password");
        msgBox->setStandardButtons(QMessageBox::Ok);
        msgBox->exec();
    }
}

void ACDP::send(QString project_id,QString horas,QString description)
{
    QDate date = calendar->selectedDate();
    QString tmp;
    QByteArray content = "";
    content.append("find_single=1");
    content.append("action=add");
    content.append("&");
    content.append("first_action=add");
    content.append("&");
    content.append("do_action=1");
    content.append("&");
    content.append("person_id=");
    content.append(id);
    content.append("&");
    content.append("person_name");
    content.append(QUrl::toPercentEncoding(nome));
    content.append("&");
    content.append("horas=");
    content.append(horas);
    content.append("&");
    content.append("hours_desc=");
    content.append(QUrl::toPercentEncoding(description));
    content.append("&");
    content.append("date_day=");

    if(QString::number(date.day()).size() == 1)
    {
        content.append("0");
        content.append(QString::number(date.day()));
    }else{
        content.append(QString::number(date.day()));
    }

    content.append("&");
    content.append("date_month=");

     if(QString::number(date.month()).size() == 1)
    {
        content.append("0");
        content.append(QString::number(date.month()));
    }else{
        content.append(QString::number(date.month()));
    }

    content.append("&");
    content.append("date_year=");
    content.append(QString::number(date.year()));
    content.append("&");
    content.append(session);

    QString url = "/acdp/horas_projeto.php?proj_id=";
    url.append(project_id);

    QHttpRequestHeader header("POST", url);
    header.setValue("Host", "ep.conectiva");
    header.setContentType("application/x-www-form-urlencoded");
    header.setContentLength(content.length());

    httpSend.setHost("ep.conectiva");
    httpSend.request(header, content);
}

void ACDP::webRefresh()
{

    QDate date = calendar->selectedDate();

    QString urlString = "http://ep.conectiva/acdp/";

    urlString.append("relatorio.php?action=personal_month2&detailed=&person_id=");
    urlString.append(id);
    urlString.append("&year=2009&month=10");

    urlString.append("&");
    urlString.append("year=");
    urlString.append(QString::number(date.year()));

    urlString.append("&");
    urlString.append("month=");

    if(QString::number(date.month()).size() == 1)
    {
        urlString.append("0");
        urlString.append(QString::number(date.month()));
    }else{
        urlString.append(QString::number(date.month()));
    }

    urlString.append("&");
    urlString.append(session);
    QUrl url = QUrl(urlString);
    webview->setUrl(url);
    webview->setDisabled(true);
}

void ACDP::sendDone(bool error)
{
    QString data = httpSend.readAll();
    QString errorStr;
    if(!error && data.indexOf("errormini") > 0 )
    {
        QRegExp rx;
        QString company, web, country;
        rx.setPattern("errormini.>(.*)</span>");
        if (rx.indexIn(data) != -1) {
            errorStr = rx.cap(1);
        }else{
            errorStr = "Unknow Error";
        }
        msgBox->setIcon(QMessageBox::Warning);
        msgBox->setText(errorStr);
        msgBox->setStandardButtons(QMessageBox::Ok);
        msgBox->exec();
    } else {
        webRefresh();
    }
}
