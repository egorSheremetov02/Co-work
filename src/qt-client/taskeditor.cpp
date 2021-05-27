#include <QHBoxLayout>
#include <QtWidgets>
#include <task.h>
#include "taskeditor.h"

TaskEditor::TaskEditor(QWidget *parent)
    : QWidget(parent)
{
    win = new QDialog(this);

    auto layout = new QVBoxLayout(win);
    task_name = new QLineEdit;
    task_deadline = new QLineEdit;
    task_description = new QTextEdit;
    task_urgency = new QLineEdit;
    ok = new QPushButton;
    layout->addWidget(task_name);
    layout->addWidget(task_description);
    layout->addWidget(task_deadline);
    layout->addWidget(task_urgency);
    layout->addWidget(ok);
    ok -> setText("ok");

    win->show();
}

void TaskEditor::setValue(const Task &task)
{
    task_name->setText(task.name);
    task_deadline->setText(task.deadline);
    task_urgency->setText(QString::number(task.urgency));
    task_description->setText(task.description);
}

Task TaskEditor::value() const
{
    return Task(task_name -> text(), task_deadline -> text());
}
