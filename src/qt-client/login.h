#ifndef LOGIN_H
#define LOGIN_H

#include <QMainWindow>
#include "todolist.h"

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void on_pushButton_logIn_clicked();

private:
    Ui::MainWindow *ui;
    ToDoList *todoList;
};
#endif // LOGIN_H
