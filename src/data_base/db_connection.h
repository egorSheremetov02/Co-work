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
  // TODO
  // bool registration();

  std::optional<Task> get_task(uint32_t id);
  std::optional<Project> get_project(uint32_t id);
  std::optional<User> get_user(uint32_t id);

  std::vector<Task> get_all_tasks_of_proj(TaskGetAllDTO &dto);

  int create_task(TaskCreateDTO &dto);
  int create_project(ProjectCreateDTO &dto);
  int create_user(UserCreateDTO &dto);

  bool add_users_to_task(uint32_t id, std::vector<uint32_t> &users_id);
  bool add_users_to_project(uint32_t id, std::vector<uint32_t> &users_id);

  bool delete_users_from_task(uint32_t id, std::vector<uint32_t> &users_id);
  bool delete_users_from_project(uint32_t id, std::vector<uint32_t> &users_id);

  bool update_task(TaskEditDTO const &dto);
  bool update_project(ProjectEditDTO &dto);

  bool delete_task();
  bool delete_project();
};

#endif  // CO_WORK_DB_CONNECTION_H
