#include <iostream>
#include <optional>
#include <pqxx/pqxx>
#include <string>
#include <vector>
#include "../doctest/doctest.h"
#include "../shared/structures.h"
#include "db_connection.h"
#include "orm.h"

TEST_CASE("Registration and Authentification new user") {
  DataBase databasa;
  RegistrationReqDTO tmp1{"Acc", "Name", Roles::USER, "email", "password"};
  uint32_t id = databasa.registration(tmp1);
  CHECK(id != 0);
  CHECK(databasa.registration(tmp1) == 0);
  CHECK(databasa.registration(tmp1) == 0);
  CHECK(databasa.auth("Acc", "password").has_value() == 1);
  CHECK(databasa.auth("admin", "qwerty").has_value() == 1);
  CHECK(databasa.auth(tmp1.account_name, "passsword").has_value() == 0);
  databasa.delete_user(id);
}

TEST_CASE("Task creation/delete") {
  DataBase databasa;
  TaskCreateDTO t{"Task_task", "Description",        7, "status",
                  1,           "2021-04-17 06:05:05"};
  uint32_t id = databasa.create_task(t, 1);
  ActionGetAllDTO dto{id};
  std::vector<Action> acts = databasa.get_all_actions_of_task(dto);
  CHECK(acts.size() == 1);
  CHECK(acts[0].action_type == Actions::CREATE_TASK);
  std::optional<Task> tmp = databasa.get_task(id);
  CHECK(tmp->id == id);
  CHECK(tmp->name == "Task_task");
  databasa.delete_task(id);
  std::optional<Task> is_del = databasa.get_task(id);
  CHECK(!is_del.has_value());
}

TEST_CASE("Task edit") {
  DataBase databasa;
  TaskCreateDTO t{"Task_task", "Description",        7, "status",
                  1,           "2021-04-17 06:05:05"};
  uint32_t id = databasa.create_task(t, 1);
  TaskEditDTO edit{id, 1, {"Edited_task"}, {}, {"new_status"}, {}, 3};
  CHECK(databasa.update_task(edit, 1) == 1);
  ActionGetAllDTO tmp{id};
  databasa.add_comment(id, 1, "Hello world! мяу мяу2");
  std::vector<Action> actss = databasa.get_all_actions_of_task(tmp);
  CHECK(actss.size() == 3);
  CHECK(actss[0].action_type == Actions::CREATE_TASK);
  CHECK(actss[1].action_type == Actions::EDIT_TASK);
  CHECK(actss[2].action_type == Actions::ADD_COMMENT);
}

TEST_CASE("Project creation/delete") {
  DataBase databasa;
  Project proj{};
}

TEST_CASE("Project creation/delete") {
  DataBase databasa;
  Project proj{};
}

TEST_CASE("Project update") {
  DataBase databasa;
}

TEST_CASE("Add/delete users in task") {
  DataBase databasa;
  std::vector<uint32_t> users{1, 2};
  TaskCreateDTO t{"Task_Users",         "Description", 7, "status", 1,
                  "2021-04-17 06:05:05"};
  uint32_t id = databasa.create_task(t, 1);
  databasa.add_users_to_task(id, users);
  std::vector<User> users2 = databasa.get_all_users_of_task(id);
  std::vector<Task> task2 = databasa.get_all_tasks_of_user(1);
  CHECK(users2.size() == 2);
  users.pop_back();
  databasa.delete_users_from_task(id, users);
  std::vector<Task> task3 = databasa.get_all_tasks_of_user(1);
  CHECK(task2.size() - 1 == task3.size());
  std::vector<User> users3 = databasa.get_all_users_of_task(id);
  CHECK(users3.size() == 1);
}

TEST_CASE("Add/delete users in project") {
  DataBase databasa;
  std::vector<uint32_t> users{1, 2};
  ProjectCreateDTO t{"Project", "2021-04-17 06:05:05"};
  uint32_t id = databasa.create_project(t);
  databasa.add_users_to_project(id, users);
  std::vector<User> users2 = databasa.get_all_users_of_project(id);
  std::vector<Project> task2 = databasa.get_all_projects_of_user(1);
  CHECK(users2.size() == 2);
  users.pop_back();
  databasa.delete_users_from_project(id, users);
  std::vector<User> users3 = databasa.get_all_users_of_project(id);
  CHECK(users3.size() == 1);
}

TEST_CASE("Actions in tasks") {
  DataBase databasa;
  TaskCreateDTO t{"Task_task", "Description",        7, "status",
                  1,           "2021-04-17 06:05:05"};
  uint32_t id = databasa.create_task(t, 1);
  databasa.add_comment(id, 1, "Hello world! мяу мяу");
  ActionGetAllDTO tmp{id};
  std::vector<Action> actss = databasa.get_all_actions_of_task(tmp);
  CHECK(actss.size() == 2);
  CHECK(actss[0].action_type == Actions::CREATE_TASK);
  CHECK(actss[1].action_type == Actions::ADD_COMMENT);
  databasa.add_comment(id, 1, "Hello world! мяу мяу2");
  ActionGetAllDTO tmp2{id};
  std::vector<Action> actss2 = databasa.get_all_actions_of_task(tmp2);
  CHECK(actss2.size() == 3);
  CHECK(actss2[0].action_type == Actions::CREATE_TASK);
  CHECK(actss2[1].action_type == Actions::ADD_COMMENT);
  CHECK(actss2[2].action_type == Actions::ADD_COMMENT);
}

TEST_CASE("Files") {
  /*
  DataBase databasa;
  std::vector<AttachedFile> f;
  f.push_back({1, "/home/project", "filename"});
  databasa.add_files_to_task(1, f);
  databasa.get_all_files(1);
  */
}
