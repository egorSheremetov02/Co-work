/********************************************************************************
** Form generated from reading UI file 'login.ui'
**
** Created by: Qt User Interface Compiler version 5.12.8
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_LOGIN_H
#define UI_LOGIN_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QGroupBox>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QLabel>
#include <QtWidgets/QLineEdit>
#include <QtWidgets/QMainWindow>
#include <QtWidgets/QMenuBar>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QStatusBar>
#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_login {
  public:
  QWidget *centralwidget;
  QGroupBox *groupBox;
  QVBoxLayout *verticalLayout;
  QHBoxLayout *horizontalLayout;
  QLabel *label_username;
  QLineEdit *lineEdit_username;
  QHBoxLayout *horizontalLayout_2;
  QLabel *label_password;
  QLineEdit *lineEdit_password;
  QLabel *label_error;
  QPushButton *pushButton_logIn;
  QMenuBar *menubar;
  QStatusBar *statusbar;

  void setupUi(QMainWindow *login) {
    if (login->objectName().isEmpty())
      login->setObjectName(QString::fromUtf8("login"));
    login->resize(800, 600);
    centralwidget = new QWidget(login);
    centralwidget->setObjectName(QString::fromUtf8("centralwidget"));
    groupBox = new QGroupBox(centralwidget);
    groupBox->setObjectName(QString::fromUtf8("groupBox"));
    groupBox->setGeometry(QRect(140, 150, 511, 211));
    verticalLayout = new QVBoxLayout(groupBox);
    verticalLayout->setObjectName(QString::fromUtf8("verticalLayout"));
    horizontalLayout = new QHBoxLayout();
    horizontalLayout->setObjectName(QString::fromUtf8("horizontalLayout"));
    label_username = new QLabel(groupBox);
    label_username->setObjectName(QString::fromUtf8("label_username"));

    horizontalLayout->addWidget(label_username);

    lineEdit_username = new QLineEdit(groupBox);
    lineEdit_username->setObjectName(QString::fromUtf8("lineEdit_username"));

    horizontalLayout->addWidget(lineEdit_username);

    verticalLayout->addLayout(horizontalLayout);

    horizontalLayout_2 = new QHBoxLayout();
    horizontalLayout_2->setObjectName(QString::fromUtf8("horizontalLayout_2"));
    label_password = new QLabel(groupBox);
    label_password->setObjectName(QString::fromUtf8("label_password"));

    horizontalLayout_2->addWidget(label_password);

    lineEdit_password = new QLineEdit(groupBox);
    lineEdit_password->setObjectName(QString::fromUtf8("lineEdit_password"));
    lineEdit_password->setEchoMode(QLineEdit::Password);

    horizontalLayout_2->addWidget(lineEdit_password);

    verticalLayout->addLayout(horizontalLayout_2);

    label_error = new QLabel(groupBox);
    label_error->setObjectName(QString::fromUtf8("label_error"));

    verticalLayout->addWidget(label_error);

    pushButton_logIn = new QPushButton(groupBox);
    pushButton_logIn->setObjectName(QString::fromUtf8("pushButton_logIn"));

    verticalLayout->addWidget(pushButton_logIn);

    login->setCentralWidget(centralwidget);
    menubar = new QMenuBar(login);
    menubar->setObjectName(QString::fromUtf8("menubar"));
    menubar->setGeometry(QRect(0, 0, 800, 22));
    login->setMenuBar(menubar);
    statusbar = new QStatusBar(login);
    statusbar->setObjectName(QString::fromUtf8("statusbar"));
    login->setStatusBar(statusbar);

    retranslateUi(login);

    QMetaObject::connectSlotsByName(login);
  }  // setupUi

  void retranslateUi(QMainWindow *login) {
    login->setWindowTitle(
        QApplication::translate("login", "MainWindow", nullptr));
    groupBox->setTitle(QString());
    label_username->setText(
        QApplication::translate("login", "Username", nullptr));
    label_password->setText(
        QApplication::translate("login", "Password", nullptr));
    label_error->setText(QString());
    pushButton_logIn->setText(
        QApplication::translate("login", "Log in", nullptr));
  }  // retranslateUi
};

namespace Ui {
class login : public Ui_login {};
}  // namespace Ui

QT_END_NAMESPACE

#endif  // UI_LOGIN_H
