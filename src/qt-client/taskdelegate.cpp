#include "taskdelegate.h"
#include "taskeditor.h"
#include <iostream>
#include <task.h>

QWidget *TaskDelegate::createEditor(QWidget *parent, const QStyleOptionViewItem &,
                                    const QModelIndex &) const
{
    std::cout << "createEditor" << std::endl;
    auto editor = new TaskEditor(parent);
    connect(editor->ok, SIGNAL(clicked()),this, SLOT(commit()));
    return editor;
}

void TaskDelegate::setEditorData(QWidget *editor, const QModelIndex &index) const
{
    static_cast<TaskEditor *>(editor)->setValue(index.data(Qt::EditRole).value<Task>());
}

void TaskDelegate::setModelData(QWidget *editor, QAbstractItemModel *model,
                                 const QModelIndex &index) const
{
    model->setData(index, QVariant::fromValue(static_cast<TaskEditor *>(editor)->value()));
}

void TaskDelegate::updateEditorGeometry(QWidget *editor, const QStyleOptionViewItem &option,
                                         const QModelIndex & /*index*/) const
{
    const int hCell = option.rect.height();
    const int hEditor = editor->sizeHint().height();
    const int h = qMax(hCell, hEditor);
    QSize size = option.rect.size();
    size.setHeight(h);
    editor->setGeometry(QRect(option.rect.topLeft() - QPoint(0, (h - hCell) / 2), size));
}

void TaskDelegate::commit()
 {
     std::cout << "commit" << std::endl;
     TaskEditor *editor = qobject_cast<TaskEditor *>(sender());
     emit commitData(editor);
     emit closeEditor(editor);
 }
