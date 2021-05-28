#ifndef KANBAN_H
#define KANBAN_H

#include <QListView>
#include <QMainWindow>
#include <QStringListModel>
#include <iostream>
#include <nlohmann/json.hpp>

// QT_BEGIN_NAMESPACE
namespace Ui {
class kanban;
}
// QT_END_NAMESPACE

class kanban : public QMainWindow {
  Q_OBJECT

  public:
  kanban(QWidget *parent = nullptr);
  ~kanban();

  signals:
  void update();

  private slots:
  void on_add();
  void on_remove();
  void show_item_menu_to_do(const QPoint &pos);
  void show_item_menu_in_progress(const QPoint &pos);
  void show_item_menu_completed(const QPoint &pos);
  void show_task();
  void show_history();
  void update_kanban();

  private:
  Ui::kanban *ui;

  QListView *list_to_do = nullptr;
  QListView *list_in_progress = nullptr;
  QListView *list_completed = nullptr;

  QAction *action_add = nullptr;
  QAction *action_remove = nullptr;

  std::string buffer;
  std::size_t len;

  // void get_projects(QMenu *menu);
  void read_server();
  void write_server(nlohmann::json &data);
};
#endif  // KANBAN_H
