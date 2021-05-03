#ifndef LOGIN_H
#define LOGIN_H

#include <QMainWindow>
#include "kanban.h"

namespace Ui {
class login;
}

class login : public QMainWindow {
  Q_OBJECT

  public:
  explicit login(QWidget *parent = nullptr);
  ~login();

  private slots:
  void on_pushButton_logIn_clicked();

  private:
  Ui::login *ui;
  kanban *_kanban;
};

#endif  // LOGIN_H
