#ifndef TASK_H
#define TASK_H

#include <QString>
#include <QMetaType>
#include <QApplication>

#include "taskaction.h"

class MyTask
{
public:
    QString name;
    QString description;
    QString deadline;
    QString status;
    QString start_date;
    uint32_t task_id;
    uint32_t project_id;
    uint8_t urgency;

    MyTask() {}
    MyTask(QString name_, QString deadline_) : name(name_), deadline(deadline_) {}

    QString toString() const {
        return name + '\n' + deadline;
    }
};

Q_DECLARE_METATYPE(MyTask);
#endif // TASK_H
