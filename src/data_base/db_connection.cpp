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
      projs("projects", C),
      files("attached_files", C),
      actions("actions", C) {
  users.add_relations("tasks", "dependence_task_user");
  users.add_relations("projs", "dependence_project_user");
  tasks.add_relations("users", "dependence_task_user");
  projs.add_relations("users", "dependence_project_user");
}

int DataBase::create_task(Task const &t) {
  return tasks.insert(t);
}

int DataBase::create_task(Task const &t, uint32_t user_id) {
  Create c{"A"};
  nlohmann::json j;
  to_json(j, c);
  uint32_t id = 2;  // tasks.insert(t);
  // std::cout << j.dump() << std::endl;
  Action act{id, user_id, Actions::CREATE_TASK, j.dump()};
  actions.insert(act);
  return id;
}

int DataBase::create_project(Project const &t) {
  return projs.insert(t);
}

int DataBase::create_user(User const &t) {
  return users.insert(t);
}

std::optional<User> DataBase::auth(std::string const &login,
                                   std::string const &password) {
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
  uint32_t amount_of_tasks =
      dto.tasks_per_page.has_value() ? dto.tasks_per_page.get() : 5;
  uint32_t skip = amount_of_tasks *
                  (dto.page_number.has_value() ? dto.page_number.get() : 0);
  return tasks(select(tasks)
                   .where(tasks.project_id == dto.project_id)
                   .offset(skip)
                   .limit(amount_of_tasks));
}

std::vector<Action> DataBase::get_all_actions_of_task(ActionGetAllDTO &dto) {
  uint32_t amount_of_tasks =
      dto.actions_per_page.has_value() ? dto.actions_per_page.get() : 5;
  uint32_t skip = amount_of_tasks *
                  (dto.page_number.has_value() ? dto.page_number.get() : 0);
  std::cout << "a" << std::endl;
  return actions(select(actions)
                     .where(actions.task_id == dto.task_id)
                     .offset(skip)
                     .limit(amount_of_tasks));
}

std::vector<Task> DataBase::get_all_tasks_of_proj(uint32_t id) {
  return tasks(select(tasks).where(tasks.project_id == id));
}

std::vector<User> DataBase::get_all_users_of_task(uint32_t id) {
  return users(select(users).join(tasks).where(tasks.task_id == id));
}

std::vector<Task> DataBase::get_all_tasks_of_user(uint32_t id) {
  return tasks(select(tasks).join(users).where(users.user_id == id));
}

bool DataBase::update_task(TaskEditDTO const &dto) {
  db::Expression update_query;

  if (dto.name) {
    update_query += tasks.name == dto.name.get();
  }
  if (dto.description) {
    update_query += tasks.description == dto.description.get();
  }
  if (dto.status) {
    update_query += tasks.status == dto.status.get();
  }
  if (dto.due_date) {
    update_query += tasks.due_date == dto.due_date.get();
  }
  if (dto.urgency) {
    update_query += tasks.urgency == dto.urgency.get();
  }
  // TODO add action update_task

  return tasks.update(dto.task_id, update_query);
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

// TODO delete ALL data(users,files,etc)

bool DataBase::delete_task(uint32_t id) {
  tasks.del_join("users", tasks.task_id == id);
  return tasks.del(tasks.id == id);
}
bool DataBase::delete_project(uint32_t id) {
  std::vector<Task> all_tasks = get_all_tasks_of_proj(id);
  for (auto &z : all_tasks) {
    delete_task(z.id);
  }
  tasks.del(tasks.project_id == id);
  // delete_users_from_project();
  return projs.del(projs.id == id);
}

bool DataBase::delete_user(uint32_t id) {
  return users.del(users.id == id);
}

bool DataBase::add_users_to_task(uint32_t id, std::vector<uint32_t> &users_id) {
  for (auto &id_of_user : users_id) {
    if (!tasks.insert_join("users", get_task(id).value(),
                           get_user(id_of_user).value()))
      ;
    return false;
  }
  return true;
}

bool DataBase::delete_users_from_task(uint32_t id,
                                      std::vector<uint32_t> &users_id) {
  for (auto &id_of_user : users_id) {
    tasks.del_join("users", tasks.task_id == id);
  }
  return true;
}
