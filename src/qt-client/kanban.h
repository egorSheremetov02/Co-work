#ifndef KANBAN_H
#define KANBAN_H

#include <QMainWindow>
#include <QListView>
#include <QStringListModel>

QT_BEGIN_NAMESPACE
namespace Ui { class kanban; }
QT_END_NAMESPACE

class kanban : public QMainWindow
{
    Q_OBJECT

public:
    kanban(QWidget *parent = nullptr);
    ~kanban();

private slots:
    void on_add();
    void task_completed();
    void task_in_progress();
    void task_in_to_do();
    void removed_from_completed();
    void removed_from_in_progress();
    void removed_from_to_do();
    //void onRemove();
private:
    Ui::kanban *ui;

    QListView* list_to_do = nullptr;
    QListView* list_in_progress = nullptr;
    QListView* list_completed = nullptr;

    QAction* action_add = nullptr;
    //QAction* action_remove = nullptr;
};
#endif // KANBAN_H
