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
  pqxx::connection *C;
  db::Users users;
  db::Tasks tasks;
  db::Projects projs;
  db::Select select;

  DataBase();

  int create_task(Task const &t);

  int create_project(Project const &t);

  int create_user(User const &t);

  std::optional<User> auth(std::string const &login,
                           std::string const &password);

  std::optional<Task> get_task(int id);
  std::optional<Project> get_project(int id);
  std::optional<User> get_user(int id);
};

#endif  // CO_WORK_DB_CONNECTION_H
