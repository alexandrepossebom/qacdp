#include "login.h"
#include "ui_login.h"

Login::Login(QWidget *parent) :
        QDialog(parent),
        m_ui(new Ui::Login)
{
    m_ui->setupUi(this);
    m_ui->passwdLineEdit->setFocus();
}

Login::~Login()
{
    delete m_ui;
}

QString Login::userName()
{
    return m_ui->loginLineEdit->text();
}

void Login::setUserName(QString username)
{
    m_ui->loginLineEdit->setText(username);
}

QString Login::pass()
{
    return m_ui->passwdLineEdit->text();
}

void Login::changeEvent(QEvent *e)
{
    QDialog::changeEvent(e);
    switch (e->type()) {
    case QEvent::LanguageChange:
        m_ui->retranslateUi(this);
        break;
    default:
        break;
    }
}
