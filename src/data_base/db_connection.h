#ifndef CO_WORK_DB_CONNECTION_H
#define CO_WORK_DB_CONNECTION_H
#include "src/shared/structures.h"
#include <iostream>
#include <optional>
#include <pqxx/pqxx>
#include <string>
#include <vector>
#include "orm.h"

struct DataBase{
  db::Users users;
  db::Tasks tasks;
  db::Projects projs;
  DataBase();

  int create_task(Task t);
};

#endif //CO_WORK_DB_CONNECTION_H
