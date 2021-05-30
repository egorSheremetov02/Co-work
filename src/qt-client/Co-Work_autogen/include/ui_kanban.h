/********************************************************************************
** Form generated from reading UI file 'kanban.ui'
**
** Created by: Qt User Interface Compiler version 5.12.8
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_KANBAN_H
#define UI_KANBAN_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QMainWindow>
#include <QtWidgets/QMenuBar>
#include <QtWidgets/QStatusBar>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_kanban {
  public:
  QWidget *centralwidget;
  QMenuBar *menubar;
  QStatusBar *statusbar;

  void setupUi(QMainWindow *kanban) {
    if (kanban->objectName().isEmpty())
      kanban->setObjectName(QString::fromUtf8("kanban"));
    kanban->resize(800, 600);
    centralwidget = new QWidget(kanban);
    centralwidget->setObjectName(QString::fromUtf8("centralwidget"));
    kanban->setCentralWidget(centralwidget);
    menubar = new QMenuBar(kanban);
    menubar->setObjectName(QString::fromUtf8("menubar"));
    kanban->setMenuBar(menubar);
    statusbar = new QStatusBar(kanban);
    statusbar->setObjectName(QString::fromUtf8("statusbar"));
    kanban->setStatusBar(statusbar);

    retranslateUi(kanban);

    QMetaObject::connectSlotsByName(kanban);
  }  // setupUi

  void retranslateUi(QMainWindow *kanban) {
    kanban->setWindowTitle(
        QApplication::translate("kanban", "kanban", nullptr));
  }  // retranslateUi
};

namespace Ui {
class kanban : public Ui_kanban {};
}  // namespace Ui

QT_END_NAMESPACE

#endif  // UI_KANBAN_H
