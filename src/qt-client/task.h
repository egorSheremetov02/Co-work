#ifndef TASK_H
#define TASK_H

#include <QString>
#include <QMetaType>
#include <QApplication>

class Task
{
public:
    QString name;
    QString description;
    QString deadline;

    Task() {}
    Task(QString name_, QString deadline_) : name(name_), deadline(deadline_) {}

    QString toString() const {
        return name + '\n' + deadline;
    }
};

Q_DECLARE_METATYPE(Task);

#endif // TASK_H
