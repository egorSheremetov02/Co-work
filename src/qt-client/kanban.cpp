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
    ("QListView { font-size: 20pt; font-weight: bold; }"
     "QListView::item { background-color: #E74C3C; padding: 10%;"
             "border: 1px solid #C0392B; }"
             "QListView::item::hover { background-color: #C0392B }");


            list_in_progress->setStyleSheet
            ("QListView { font-size: 20pt; font-weight: bold; }"
             "QListView::item { background-color: #E74C3C; padding: 10%;"
             "border: 1px solid #C0392B; }"
             "QListView::item::hover { background-color: #FFC618 }");

            list_completed->setStyleSheet
            ("QListView { font-size: 20pt; font-weight: bold; }"
             "QListView::item { background-color: #2ECC71; padding: 10%;"
             "border: 1px solid #27AE60; }"
             "QListView::item::hover { background-color: #27AE60 }");

    //создаем события в тулбаре(добавление и удаление)

    QToolBar* tool_bar = new QToolBar(this);
    addToolBar(tool_bar);

    action_add = new QAction(this);
    //ActAdd->setIcon(QIcon(":/resources/add.png"));
    connect(action_add, &QAction::triggered, this, &kanban::on_add);

    tool_bar->addAction(action_add);
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

