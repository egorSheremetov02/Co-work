#include "login.h"
#include "ui_login.h"

login::login(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::login)
{
    ui->setupUi(this);
}

login::~login()
{
    delete ui;
}

void login::on_pushButton_logIn_clicked()
{
    QString username = ui ->lineEdit_username -> text();
    QString password = ui ->lineEdit_password -> text();

    _kanban = new kanban();
    _kanban->show();
}
