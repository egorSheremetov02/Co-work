#ifndef TASK_H
#define TASK_H

#include <QApplication>
#include <QMetaType>
#include <QString>
#include "taskaction.h"

class MyTask {
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
  MyTask(QString name_,
         QString description_,
         QString deadline_,
         QString status_,
         QString start_date_,
         uint32_t task_id_,
         uint32_t project_id_,
         uint32_t urgency_)
      : name(name_),
        deadline(deadline_),
        description(description_),
        status(status_),
        start_date(start_date_),
        task_id(task_id_),
        project_id(project_id_),
        urgency(urgency_) {}

  QString toString() const {
    return name + '\n' + deadline + '\n' + QString::number(urgency);
  }
};

Q_DECLARE_METATYPE(MyTask);
#endif  // TASK_H
