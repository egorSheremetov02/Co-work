#include "kanban.h"
#include "login.h"

#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    kanban w;
    //w.show();
    login log_in;
    log_in.show();
    return a.exec();
}
