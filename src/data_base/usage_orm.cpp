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

TEST_CASE("Task update") {
  DataBase databasa;
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
  std::vector<uint32_t> users{2};
  databasa.add_users_to_task(2, users);
  std::vector<User> users2 = databasa.get_all_users_of_task(1);
  std::vector<Task> task2 = databasa.get_all_tasks_of_user(9);
  CHECK(users2.size() == 1);
  databasa.delete_users_from_task(1, users);
  std::vector<User> users3 = databasa.get_all_users_of_task(1);
  CHECK(users3.size() == 0);
}

TEST_CASE("Add/delete users in project") {
  DataBase databasa;
}

TEST_CASE("Actions in tasks") {
  DataBase databasa;
  databasa.add_comment(1, 1, "Hello world! мяу мяу");
  ActionGetAllDTO tmp{1};
  std::vector<Action> actss = databasa.get_all_actions_of_task(tmp);
  databasa.test();
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
