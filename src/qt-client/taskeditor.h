#ifndef TASKEDITOR_H
#define TASKEDITOR_H


#include <QComboBox>
#include <QDoubleSpinBox>
#include <QWidget>
#include <QPushButton>
#include <QTextEdit>
#include "taskdelegate.h"

class MyTask;

class TaskEditor : public QWidget
{
    Q_OBJECT

public:
    TaskEditor(QWidget *parent = nullptr);
    void setValue(const MyTask &task);
    MyTask value() const;
    QPushButton *ok;

private:
    QDialog *win;
    QLineEdit *task_name;
    QLineEdit *task_deadline;
    QLineEdit *task_urgency;
    QTextEdit *task_description;
};

#endif // TASKEDITOR_H
