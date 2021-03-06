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
  db::Files files;
  db::Actions actions;
  db::Select select;

  DataBase();

  int create_task(Task const &t);

  int create_project(Project const &t);

  int create_user(User const &t);

  std::optional<User> auth(std::string const &login,
                           std::string const &password);

  uint32_t registration(RegistrationReqDTO &dto);

  std::optional<Task> get_task(uint32_t id);
  std::optional<Project> get_project(uint32_t id);
  std::optional<User> get_user(uint32_t id);

  std::vector<Task> get_all_tasks_of_proj(TaskGetAllDTO &dto);
  std::vector<Action> get_all_actions_of_task(ActionGetAllDTO const &dto);
  // TODO add pagination for status
  std::vector<Task> get_all_tasks_of_user(uint32_t id);
  std::vector<User> get_all_users_of_task(uint32_t id);

  std::vector<Project> get_all_projects_of_user(uint32_t id);
  std::vector<User> get_all_users_of_project(uint32_t id);

  int create_task(TaskCreateDTO &dto);
  int create_project(ProjectCreateDTO &dto);
  int create_user(RegistrationReqDTO &dto);

  int create_task(TaskCreateDTO const &t, uint32_t user_id);

  bool add_users_to_task(uint32_t id, std::vector<uint32_t> &users_id);
  bool add_users_to_project(uint32_t id, std::vector<uint32_t> &users_id);

  bool delete_users_from_task(uint32_t id, std::vector<uint32_t> &users_id);
  bool delete_users_from_project(uint32_t id, std::vector<uint32_t> &users_id);

  bool update_task(TaskEditDTO const &dto, uint32_t user_id);
  bool update_project(ProjectEditDTO &dto);

  bool delete_task(uint32_t id);
  bool delete_project(uint32_t id);
  bool delete_user(uint32_t id);

  // for history:
  bool add_comment(uint32_t task_id, uint32_t user_id, std::string comment);

  // for files
  std::vector<AttachedFile> get_all_files(uint32_t id);
  bool add_files_to_task(uint32_t id, std::vector<AttachedFile> const &f);
  void test();
  bool update_task(TaskEditDTO const &dto);
  std::vector<Action> get_history(uint32_t id);
  std::vector<Action> get_history(ActionGetAllDTO const &dto);

  private:
  std::vector<Task> get_all_tasks_of_proj(uint32_t id);
};

#endif  // CO_WORK_DB_CONNECTION_H
