#include "db_connection.h"
#include <iostream>
#include <optional>
#include <pqxx/pqxx>
#include <string>
#include <vector>
#include "../shared/structures.h"

#define GET_RES_STR(token) #token

// clang-format off

#define CONNECTION_STRING(USER) GET_RES_STR(dbname = co-work_db user = USER)

// clang-format on

DataBase::DataBase()
    : C(new pqxx::connection(CONNECTION_STRING(DBUSER))),
      users("users", C),
      tasks("tasks", C),
      projs("projects", C) {
  users.add_relations("tasks", "dependence_task_user");
  tasks.add_relations("users", "dependence_task_user");
  projs.add_relations("users", "dependence_project_user");
}

int DataBase::create_task(Task const &t) {
  return tasks.insert(t);
}

int DataBase::create_project(Project const &t) {
  return projs.insert(t);
}

int DataBase::create_user(User const &t) {
  return users.insert(t);
}

std::optional<User> DataBase::auth(std::string const &login,
                                   std::string const &password) {
  std::cout << std::hash<std::string>{}(password) << '\n';
  std::vector<User> tmp = users(select(users).where(
      users.account_name == login and users.password == password));
  if (tmp.size() == 1) {
    return tmp[0];
  } else {
    return std::nullopt;
  }
}

std::optional<Task> DataBase::get_task(int id) {
  std::vector<Task> tmp = tasks(select(tasks).where(tasks.id == id));
  if (tmp.size() == 1) {
    return tmp[0];
  } else {
    return std::nullopt;
  }
}
std::optional<Project> DataBase::get_project(int id) {
  std::vector<Project> tmp = projs(select(projs).where(projs.id == id));
  if (tmp.size() == 1) {
    return tmp[0];
  } else {
    return std::nullopt;
  }
}
std::optional<User> DataBase::get_user(int id) {
  std::vector<User> tmp = users(select(users).where(users.id == id));
  if (tmp.size() == 1) {
    return tmp[0];
  } else {
    return std::nullopt;
  }
}

std::vector<Task> DataBase::get_all_tasks_of_proj(TaskGetAllDTO &dto) {
  int amount_of_tasks =
      dto.tasks_per_page.has_value() ? dto.tasks_per_page.get() : 5;
  int skip = amount_of_tasks *
             (dto.page_number.has_value() ? dto.page_number.get() : 0);
  return tasks(select(tasks)
                   .where(tasks.project_id == dto.project_id)
                   .offset(skip)
                   .limit(amount_of_tasks));
}
