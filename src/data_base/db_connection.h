#ifndef CO_WORK_DB_CONNECTION_H
#define CO_WORK_DB_CONNECTION_H

#include <iostream>
#include <optional>
#include <pqxx/pqxx>
#include <string>
#include <vector>
#include "orm.h"
#include "src/shared/structures.h"

struct DataBase {
  db::Users users;
  db::Tasks tasks;
  db::Projects projs;

  DataBase();

  int create_task(Task const &t);

  int create_project(Project const &t);

  std::optional<User> auth(std::string const &login,
                           std::string const &password);
};

#endif  // CO_WORK_DB_CONNECTION_H
