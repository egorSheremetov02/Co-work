#include "kanban.h"
#include "./ui_kanban.h"

#include <QLabel>
#include <QToolBar>
#include <QBoxLayout>

kanban::kanban(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::kanban)
{
    ui->setupUi(this);
    QWidget *window = new QWidget(this);
    //window->setStyleSheet("background-color: #E6D1D1");
    setCentralWidget(window);

    QVBoxLayout *win_layout = new QVBoxLayout();
    window->setLayout(win_layout);

    QHBoxLayout *label_layout = new QHBoxLayout();
    win_layout->addLayout(label_layout);

    QLabel *label_to_do = new QLabel("To do", this);
    label_to_do->setStyleSheet("font-size: 15pt;");
    label_layout->addWidget(label_to_do);

    QLabel *label_in_progress = new QLabel("In Progress", this);
    label_in_progress->setStyleSheet("font-size: 15pt;");
    label_layout->addWidget(label_in_progress);

    QLabel *label_completed = new QLabel("Completed", this);
    label_completed->setStyleSheet("font-size: 15pt;");
    label_layout->addWidget(label_completed);

    QHBoxLayout *list_layout = new QHBoxLayout();
    win_layout->addLayout(list_layout);

    //drag-n-drop

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

    list_to_do->setModel(new QStringListModel());
    list_in_progress->setModel(new QStringListModel());
    list_completed->setModel(new QStringListModel());

    list_to_do->setStyleSheet
    ("QListView { font-size: 20pt; background-color: #F3E3FC; border: none;  border-radius: 12px;}"
     "QListView::item { background-color: #d4bee1; padding: 10%; border-radius: 12px;}"
     "QListView::item::hover { background-color: #ddc7ea }");

    list_in_progress->setStyleSheet
    ("QListView { font-size: 20pt; background-color: #F5EDBA; border:none;  border-radius: 12px;}"
     "QListView::item { background-color: #e4dba3; padding: 10%; border-radius: 12px;}"
     "QListView::item::hover { background-color: #e6deab }");

     list_completed->setStyleSheet
     ("QListView { font-size: 20pt; background-color: #C8F6D8; border: none;  border-radius: 12px;}"
      "QListView::item { background-color: #A4C7AF; padding: 10%; border-radius: 12px;}"
      "QListView::item::hover { background-color: #b3dac0 }");

    //создаем события в тулбаре(добавление и удаление)

    QToolBar* tool_bar = new QToolBar(this);
    addToolBar(tool_bar);

    action_add = new QAction(this);
    //ActAdd->setIcon(QIcon(":/resources/add.png"));
    connect(action_add, &QAction::triggered, this, &kanban::on_add);

    //сигналы для отправки сообщений на сервер о перемещении объектов в канбане
    connect(list_completed -> model(), SIGNAL(rowsInserted(QModelIndex,int,int)), this, SLOT(task_completed()));
    connect(list_in_progress -> model(), SIGNAL(rowsInserted(QModelIndex,int,int)), this, SLOT(task_in_progress()));
    connect(list_to_do -> model(), SIGNAL(rowsInserted(QModelIndex,int,int)), this, SLOT(task_in_to_do()));

    connect(list_completed -> model(), SIGNAL(rowsRemoved(QModelIndex,int,int)), this, SLOT(removed_from_completed()));
    connect(list_in_progress -> model(), SIGNAL(rowsRemoved(QModelIndex,int,int)), this, SLOT(removed_from_in_progress()));
    connect(list_to_do -> model(), SIGNAL(rowsRemoved(QModelIndex,int,int)), this, SLOT(removed_from_to_do()));

    tool_bar->addAction(action_add);
}

void kanban::task_completed()
{
//    QLabel *l = new QLabel("task completed");
//    l -> show();
}

void kanban::task_in_progress()
{
//    QLabel *l = new QLabel("task is in progress");
//    l -> show();
}

void kanban::task_in_to_do()
{
//    QLabel *l = new QLabel("task is in to-do");
//    l -> show();
}

void kanban::removed_from_completed()
{
//    QLabel *l = new QLabel("task removed from completed");
//    l -> show();
}

void kanban::removed_from_in_progress()
{
//    QLabel *l = new QLabel("task removed from in progress");
//    l -> show();
}

void kanban::removed_from_to_do()
{
//    QLabel *l = new QLabel("task removed from to-do");
//    l -> show();
}

void kanban::on_add()
{
    list_to_do->model()->insertRow(list_to_do->model()->rowCount());
    QModelIndex oIndex = list_to_do->model()->index(
                list_to_do->model()->rowCount() - 1, 0);

    list_to_do->edit(oIndex);
}


kanban::~kanban()
{
    delete ui;
}

