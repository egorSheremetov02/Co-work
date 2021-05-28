#ifndef TASKACTION_H
#define TASKACTION_H
#include <QMetaType>
#include <QString>

class TaskAction {
  public:
  TaskAction();
  QString action_type;
  QString author;
  QString description;
  QString date;

  explicit TaskAction(QString action_type_,
                      QString author_,
                      QString description_,
                      QString date_)
      : action_type(action_type_),
        author(author_),
        description(description_),
        date(date_) {}
};

#endif  // TASKACTION_H
