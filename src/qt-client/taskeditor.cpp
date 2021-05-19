#include <QHBoxLayout>
#include <QtWidgets>
#include <task.h>
#include "taskeditor.h"

TaskEditor::TaskEditor(QWidget *parent)
    : QWidget(parent)
{
    win = new QDialog(parent);

    auto layout = new QVBoxLayout(win);
    task_name = new QLineEdit;
    task_deadline = new QLineEdit;
    ok = new QPushButton;
    layout->addWidget(task_name);
    layout->addWidget(task_deadline);
    layout->addWidget(ok);
    ok -> setText("ok");

    win->show();
}

void TaskEditor::setValue(const Task &task)
{
    task_name->setText(task.name);
    task_deadline->setText(task.deadline);
}

Task TaskEditor::value() const
{
    return Task(task_name -> text(), task_deadline -> text());
}
