#ifndef TODOLIST_H
#define TODOLIST_H

#include <QMainWindow>
#include <QListView>
#include <QStringListModel>

namespace Ui {
class ToDoList;
}

class ToDoList : public QMainWindow
{
    Q_OBJECT

public:
    explicit ToDoList(QWidget *parent = nullptr);
    ~ToDoList();

protected slots:
    void onAdd();
    //void onRemove();

private:
    Ui::ToDoList *ui;

    QListView* list_to_do = nullptr;
    QListView* list_in_progress = nullptr;
    QListView* list_completed = nullptr;

    QAction* action_add = nullptr;
    //QAction* action_remove = nullptr;
};

#endif // TODOLIST_H
