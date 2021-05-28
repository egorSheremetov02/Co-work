#include "taskdelegate.h"
#include <task.h>
#include <QLineEdit>
#include <QTextEdit>
#include <iostream>
#include "taskeditor.h"

QWidget *TaskDelegate::createEditor(QWidget *parent,
                                    const QStyleOptionViewItem &,
                                    const QModelIndex &) const {
  auto editor = new TaskEditor(parent);
  connect(editor->ok, SIGNAL(clicked()), this, SLOT(commit()));
  return editor;
}

void TaskDelegate::setEditorData(QWidget *editor,
                                 const QModelIndex &index) const {
  static_cast<TaskEditor *>(editor)->setValue(
      index.data(Qt::EditRole).value<MyTask>());
}

void TaskDelegate::setModelData(QWidget *editor,
                                QAbstractItemModel *model,
                                const QModelIndex &index) const {
  MyTask task =
      MyTask(static_cast<TaskEditor *>(editor)->task_name->text(),
             static_cast<TaskEditor *>(editor)->task_description->toPlainText(),
             static_cast<TaskEditor *>(editor)->task_deadline->text(),
             index.data(Qt::DisplayRole).value<MyTask>().status,
             index.data(Qt::DisplayRole).value<MyTask>().start_date,
             index.data(Qt::DisplayRole).value<MyTask>().task_id,
             index.data(Qt::DisplayRole).value<MyTask>().project_id,
             static_cast<TaskEditor *>(editor)->task_urgency->text().toInt());

  model->setData(index, QVariant::fromValue(task));
}

void TaskDelegate::updateEditorGeometry(QWidget *editor,
                                        const QStyleOptionViewItem &option,
                                        const QModelIndex & /*index*/) const {
  const int hCell = option.rect.height();
  const int hEditor = editor->sizeHint().height();
  const int h = qMax(hCell, hEditor);
  QSize size = option.rect.size();
  size.setHeight(h);
  editor->setGeometry(
      QRect(option.rect.topLeft() - QPoint(0, (h - hCell) / 2), size));
}

void TaskDelegate::commit() {
  TaskEditor *editor = qobject_cast<TaskEditor *>(sender()->parent()->parent());
  emit commitData(editor);
  emit closeEditor(editor);
}
