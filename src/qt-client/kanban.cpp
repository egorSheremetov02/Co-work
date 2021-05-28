#include "kanban.h"
#include <task.h>
#include <QBoxLayout>
#include <QDialog>
#include <QLabel>
#include <QLineEdit>
#include <QPlainTextEdit>
#include <QPushButton>
#include <QToolBar>
#include <iostream>
#include <nlohmann/json.hpp>
#include "./ui_kanban.h"
#include "asio.hpp"
#include "globals.h"
#include "serialization.h"
#include "structures.h"
#include "taskdelegate.h"
#include "taskeditor.h"
#include "tasklist.h"

using asio::ip::tcp;
using nlohmann::json;

tcp::socket &get_socket();

kanban::kanban(QWidget *parent) : QMainWindow(parent), ui(new Ui::kanban) {
  buffer.resize(defaults::server::default_buffer_size);

  ui->setupUi(this);
  QWidget *window = new QWidget(this);
  window->setStyleSheet("background-color: #F9F9FA");
  setCentralWidget(window);

  QVBoxLayout *win_layout = new QVBoxLayout();
  window->setLayout(win_layout);

  QMenuBar *menu_bar = new QMenuBar(this);
  win_layout->addWidget(menu_bar);

  QHBoxLayout *label_layout = new QHBoxLayout();
  win_layout->addLayout(label_layout);

  QLabel *label_to_do = new QLabel("To do", this);
  label_layout->addWidget(label_to_do);

  QLabel *label_in_progress = new QLabel("In Progress", this);
  label_layout->addWidget(label_in_progress);

  QLabel *label_completed = new QLabel("Completed", this);
  label_layout->addWidget(label_completed);

  QHBoxLayout *list_layout = new QHBoxLayout();
  win_layout->addLayout(list_layout);

  label_to_do->setAlignment(Qt::AlignHCenter);
  label_in_progress->setAlignment(Qt::AlignHCenter);
  label_completed->setAlignment(Qt::AlignHCenter);

  label_to_do->setStyleSheet("font-size: 20pt;");
  label_in_progress->setStyleSheet("font-size: 20pt;");
  label_completed->setStyleSheet("font-size: 20pt;");

  // drag-n-drop

  QMetaType::registerConverter(&MyTask::toString);

  list_to_do = new QListView(this);
  list_to_do->setDragEnabled(true);
  list_to_do->setAcceptDrops(true);
  list_to_do->setDropIndicatorShown(true);
  list_to_do->setDefaultDropAction(Qt::MoveAction);
  list_layout->addWidget(list_to_do);

  list_in_progress = new QListView(this);
  list_in_progress->setDragEnabled(true);
  list_in_progress->setAcceptDrops(true);
  list_in_progress->setDropIndicatorShown(true);
  list_in_progress->setDefaultDropAction(Qt::MoveAction);
  list_layout->addWidget(list_in_progress);

  list_completed = new QListView(this);
  list_completed->setDragEnabled(true);
  list_completed->setAcceptDrops(true);
  list_completed->setDropIndicatorShown(true);
  list_completed->setDefaultDropAction(Qt::MoveAction);
  list_layout->addWidget(list_completed);

  list_to_do->setModel(new TaskList(list_to_do));
  list_in_progress->setModel(new TaskList(list_in_progress));
  list_completed->setModel(new TaskList(list_completed));

  list_to_do->setItemDelegate(new TaskDelegate(list_to_do));
  list_in_progress->setItemDelegate(new TaskDelegate(list_in_progress));
  list_completed->setItemDelegate(new TaskDelegate(list_completed));

  list_to_do->setStyleSheet(
      "QListView { font-size: 20pt; background-color: #F3E3FC; border: none;  "
      "border-radius: 12px;}"
      "QListView::item { background-color: #d4bee1; padding: 10%; "
      "border-radius: 12px;}"
      "QListView::item::hover { background-color: #ddc7ea }");

  list_in_progress->setStyleSheet(
      "QListView { font-size: 20pt; background-color: #F5EDBA; border:none;  "
      "border-radius: 12px;}"
      "QListView::item { background-color: #e4dba3; padding: 10%; "
      "border-radius: 12px;}"
      "QListView::item::hover { background-color: #e6deab }");

  list_completed->setStyleSheet(
      "QListView { font-size: 20pt; background-color: #C8F6D8; border: none;  "
      "border-radius: 12px;}"
      "QListView::item { background-color: #A4C7AF; padding: 10%; "
      "border-radius: 12px;}"
      "QListView::item::hover { background-color: #b3dac0 }");

  //меню

  QMenu *task_menu = new QMenu("Tasks");

  action_add = new QAction(this);
  action_add->setText("Add task");
  connect(action_add, &QAction::triggered, this, &kanban::on_add);

  action_remove = new QAction(this);
  action_remove->setText("Remove task");
  connect(action_remove, &QAction::triggered, this, &kanban::on_remove);

  task_menu->addAction(action_add);
  task_menu->addAction(action_remove);

  menu_bar->addMenu(task_menu);

  QMenu *project_menu = new QMenu("Projects");
  QMenu *my_projects_menu = new QMenu("My projects");
  QAction *add_project = new QAction(this);
  add_project->setText("Add project");
  //тут нужно коннектить с слотом добавления проекта

  project_menu->addMenu(my_projects_menu);
  project_menu->addAction(add_project);

  menu_bar->addMenu(project_menu);

  //в меню нужно добавить еще фильтр по проектам(логика кажется примерно такая
  //же как в отображении проектов)

  list_to_do->setContextMenuPolicy(Qt::CustomContextMenu);

  connect(list_to_do, SIGNAL(customContextMenuRequested(const QPoint &)), this,
          SLOT(show_item_menu_to_do(const QPoint &)));

  list_in_progress->setContextMenuPolicy(Qt::CustomContextMenu);

  connect(list_in_progress, SIGNAL(customContextMenuRequested(const QPoint &)),
          this, SLOT(show_item_menu_in_progress(const QPoint &)));

  list_completed->setContextMenuPolicy(Qt::CustomContextMenu);

  connect(list_completed, SIGNAL(customContextMenuRequested(const QPoint &)),
          this, SLOT(show_item_menu_completed(const QPoint &)));

  connect(this, SIGNAL(update()), this, SLOT(update_kanban()));
}

void kanban::on_add() {
  list_to_do->model()->insertRow(list_to_do->model()->rowCount());
  QModelIndex ind =
      list_to_do->model()->index(list_to_do->model()->rowCount() - 1, 0);

  list_to_do->edit(ind);
}

void kanban::on_remove() {
  QModelIndex ind = list_completed->currentIndex();
  list_completed->model()->removeRows(ind.row(), 1, ind);
}

void kanban::show_item_menu_to_do(const QPoint &pos) {
  QMenu *menu = new QMenu(this);
  menu->addAction("Show task", this, SLOT(show_task()));
  menu->addAction("History", this, SLOT(show_history()));
  menu->addAction("Сomments", this, SLOT(show_comments()));

  menu->popup(list_to_do->viewport()->mapToGlobal(pos));
}

void kanban::show_item_menu_in_progress(const QPoint &pos) {
  QMenu *menu = new QMenu(this);
  menu->addAction("Show task", this, SLOT(show_task()));
  menu->addAction("History", this, SLOT(show_history()));
  menu->addAction("Сomments", this, SLOT(show_comments()));
  menu->popup(list_in_progress->viewport()->mapToGlobal(pos));
}

void kanban::show_item_menu_completed(const QPoint &pos) {
  QMenu *menu = new QMenu(this);
  menu->addAction("Show task", this, SLOT(show_task()));
  menu->addAction("History", this, SLOT(show_history()));
  menu->addAction("Сomments", this, SLOT(show_comments()));
  menu->popup(list_completed->viewport()->mapToGlobal(pos));
}

void kanban::show_task() {
  QDialog *window = new QDialog;
  window->setWindowTitle("Task info");
  MyTask task = list_to_do->selectionModel()
                    ->selectedIndexes()
                    .at(0)
                    .data()
                    .value<MyTask>();

  auto layout = new QVBoxLayout(window);
  layout->addWidget(new QLabel("Name: " + task.name));
  layout->addWidget(new QLabel("Urgency: " + QString::number(task.urgency)));
  QPlainTextEdit *desc = new QPlainTextEdit("Description: " + task.description);
  desc->setReadOnly(true);
  layout->addWidget(desc);
  layout->addWidget(new QLabel("Created: " + task.start_date));
  layout->addWidget(new QLabel("Deadline: " + task.deadline));
  window->show();
}

void kanban::show_history() {
  QDialog *window = new QDialog(this);
  window->setWindowTitle("History");
}

void kanban::update_kanban() {
  std::cout << "update_kanban" << std::endl;
  auto server_response =
      json::parse(buffer.substr(0, len)).get<ResponseFormat<json>>();

  if (server_response.metadata == "heartbeat") {
  }

  if (server_response.metadata == defaults::server::get_all_tasks_metadata) {
    auto todos = server_response.data.get<std::vector<Task>>();
    list_to_do->model()->removeRows(0, list_to_do->model()->rowCount(),
                                    QModelIndex());
    list_in_progress->model()->removeRows(
        0, list_in_progress->model()->rowCount(), QModelIndex());
    list_completed->model()->removeRows(0, list_completed->model()->rowCount(),
                                        QModelIndex());
    for (auto &todo : todos) {
      MyTask task = MyTask(QString::fromStdString(todo.name),
                           QString::fromStdString(todo.description),
                           QString::fromStdString(todo.due_date),
                           QString::fromStdString(todo.status),
                           QString::fromStdString(todo.start_date), todo.id,
                           todo.project_id, todo.urgency);
      if (todo.status == "TODO") {
        static_cast<TaskList *>(list_to_do->model())
            ->addTask(list_to_do->model()->rowCount(), task);
      } else if (todo.status == "IN_PROGRESS") {
        static_cast<TaskList *>(list_in_progress->model())
            ->addTask(list_in_progress->model()->rowCount(), task);
      } else {
        static_cast<TaskList *>(list_completed->model())
            ->addTask(list_completed->model()->rowCount(), task);
      }
    }
  } else if (server_response.metadata ==
             defaults::server::create_task_metadata) {
    auto todo = server_response.data.get<Task>();
    MyTask task = MyTask(QString::fromStdString(todo.name),
                         QString::fromStdString(todo.description),
                         QString::fromStdString(todo.due_date),
                         QString::fromStdString(todo.status),
                         QString::fromStdString(todo.start_date), todo.id,
                         todo.project_id, todo.urgency);
    if (todo.status == "TODO") {
      static_cast<TaskList *>(list_to_do->model())
          ->addTask(list_to_do->model()->rowCount(), task);
    } else if (todo.status == "IN_PROGRESS") {
      static_cast<TaskList *>(list_in_progress->model())
          ->addTask(list_in_progress->model()->rowCount(), task);
    } else {
      static_cast<TaskList *>(list_completed->model())
          ->addTask(list_completed->model()->rowCount(), task);
    }
  }
}

void kanban::read_server() {
  get_socket().async_read_some(asio::buffer(buffer.data(), buffer.size()),
                               [&](const asio::error_code &err, std::size_t l) {
                                 emit update();
                                 std::cout << "read_server_callback"
                                           << std::endl;
                               });
}

// void kanban::get_projects() {
//  //взять что-то с сервера
//}

// void kanban::on_button_pushed() {
//  QLabel *l = new QLabel("task added in todo: " + input);
//  l->show();
//  TaskCreateDTO taskDTO;
//  taskDTO.description = input.toStdString();
//  taskDTO.name = "From Qt-client";
//  taskDTO.urgency = 10;
//  taskDTO.status = "TODO";
//  RequestFormat<TaskCreateDTO> task_create_request = {"task create", taskDTO};
//  nlohmann::json json_task_create = task_create_request;
//  std::string request = json_task_create.dump();

//  get_socket().write_some(asio::buffer(request.data(), request.size()));
//}

kanban::~kanban() {
  delete ui;
}
