#ifndef TASKEDITOR_H
#define TASKEDITOR_H


#include <QComboBox>
#include <QDoubleSpinBox>
#include <QWidget>
#include <QPushButton>
#include "taskdelegate.h"

class Task;

class TaskEditor : public QWidget
{
    Q_OBJECT

public:
    TaskEditor(QWidget *parent = nullptr);
    void setValue(const Task &task);
    Task value() const;
    QPushButton *ok;

private:
    QDialog *win;
    QLineEdit *task_name;
    QLineEdit *task_deadline;
};

#endif // TASKEDITOR_H
