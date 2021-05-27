#include <QApplication>
#include <QMetaType>
#include "kanban.h"
#include "login.h"
#include "task.h"

int main(int argc, char *argv[]) {
  QApplication a(argc, argv);
  kanban w;
  login log_in;

  log_in.show();
  return a.exec();
}
