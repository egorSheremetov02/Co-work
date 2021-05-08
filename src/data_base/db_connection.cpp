#include "db_connection.h"
#include <iostream>
#include <optional>
#include <pqxx/pqxx>
#include <string>
#include <vector>
#include "../shared/serialization.h"
#include "../shared/structures.h"
#include "sha/sha256.h"

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
  // std::cout << sha256(password) << '\n';
  std::vector<User> tmp = users(select(users).where(
      users.account_name == login and users.password == sha256(password)));
  if (tmp.size() == 1) {
    std::cout << "Yes" << std::endl;
    return tmp[0];
  } else {
    std::cout << "NOOO" << std::endl;
    return std::nullopt;
  }
}

std::optional<Task> DataBase::get_task(uint32_t id) {
  std::vector<Task> tmp = tasks(select(tasks).where(tasks.id == id));
  if (tmp.size() == 1) {
    return tmp[0];
  } else {
    return std::nullopt;
  }
}
std::optional<Project> DataBase::get_project(uint32_t id) {
  std::vector<Project> tmp = projs(select(projs).where(projs.id == id));
  if (tmp.size() == 1) {
    return tmp[0];
  } else {
    return std::nullopt;
  }
}
std::optional<User> DataBase::get_user(uint32_t id) {
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

bool DataBase::update_task(TaskEditDTO &dto) {
  db::Expression tmp;
  if (dto.name.has_value()) {
    tmp += tasks.name == dto.name.get();
  }
  if (dto.description.has_value()) {
    tmp += tasks.description == dto.description.get();
  }
  if (dto.status.has_value()) {
    tmp += tasks.status == dto.status.get();
  }
  if (dto.due_date.has_value()) {
    tmp += tasks.due_date == dto.due_date.get();
  }
  if (dto.urgency.has_value()) {
    tmp += tasks.urgency == dto.urgency.get();
  }
  // TODO add action update_task

  return tasks.update(dto.task_id, tmp);
}

bool DataBase::update_project(ProjectEditDTO &dto) {
  db::Expression tmp;
  if (dto.name.has_value()) {
    tmp += tasks.name == dto.name.get();
  }
  if (dto.due_date.has_value()) {
    tmp += tasks.due_date == dto.due_date.get();
  }

  return projs.update(dto.project_id, tmp);
}

int DataBase::create_task(TaskCreateDTO &dto) {
  return tasks.insert(from_dto(dto));
}

int DataBase::create_project(ProjectCreateDTO &dto) {
  return projs.insert(from_dto(dto));
}

int DataBase::create_user(UserCreateDTO &dto) {
  return users.insert(from_dto(std::move(dto)));
}
