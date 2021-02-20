#include "login.h"
#include "ui_login.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

   /* QWidget *LogInWidget = new QWidget(this);
        LogInWidget->setStyleSheet("background-color: #E6D1D1");
        setCentralWidget(LogInWidget);*/
}

MainWindow::~MainWindow()
{
    delete ui;
}


void MainWindow::on_pushButton_logIn_clicked()
{
    QString username = ui ->lineEdit_username -> text();
    QString password = ui ->lineEdit_password -> text();

    /*if (password == "password" && username == "user") {
        ui -> label_HelloUser -> setText("Ура! Вы вошли");
        todoList = new ToDoList();
        todoList -> show();
        close();
    } else {
        ui -> label_HelloUser -> setText("Неверный логин или пароль. Попробуйте снова");
    }*/
    todoList = new ToDoList();
    todoList->show();
}
