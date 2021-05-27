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
  CHECK(databasa.registration(tmp1) == 1);
  CHECK(databasa.registration(tmp1) == 0);
}

TEST_CASE("Task creation/delete") {
  /*DataBase databasa;
    Task t{0, "Task_task", "Description", "2021-04-17 06:05:05", 1, 7,
    "status"}; int tt=databasa.create_task(t); std::cout<<tt<<std::endl;
    std::optional<Task> tmp=databasa.get_task(tt);
    CHECK(tmp->id==tt);
    CHECK(tmp->name=="Task_task");
    databasa.delete_task(tt);
    std::optional<Task> is_del=databasa.get_task(tt);
    CHECK(!is_del.has_value());*/
}

TEST_CASE("Task creation/delete with action") {
  DataBase databasa;
  Task t{0, "Task_task_test", "Description", "2021-04-17 06:05:05", 1,
         7, "status"};
  uint32_t tt = databasa.create_task(t, 1);
  ActionGetAllDTO tmp{tt};
  std::vector<Action> acts = databasa.get_all_actions_of_task(tmp);
  // databasa.delete_task(tt);*/
}

TEST_CASE("Task update") {
  DataBase databasa;
  databasa.update_task({21, {"miu"}, {"New_discr"}}, 1);
  std::vector<Action> acts = databasa.get_history(21);
}

TEST_CASE("Project creation/delete") {
  DataBase databasa;
}

TEST_CASE("Project update") {
  DataBase databasa;
}

TEST_CASE("Add/delete users in task") {
  /* DataBase databasa;
   //uint32_t
  id=databasa.create_user({0,"irzuk","Irina",Roles::ADMIN,"irina@hse.ru"});
   std::vector<uint32_t> users{9};
   databasa.add_users_to_task(1,users);
   std::vector<User> users2=databasa.get_all_users_of_task(1);
   std::vector<Task> task2=databasa.get_all_tasks_of_user(9);
  CHECK(users2.size()==1);
   databasa.delete_users_from_task(1,users);
   std::vector<User> users3=databasa.get_all_users_of_task(1);
   CHECK(users3.size()==0);*/
}

TEST_CASE("Add/delete users in project") {
  DataBase databasa;
}

TEST_CASE("Actions in tasks") {
  DataBase databasa;
  // databasa.add_comment(1,1,"Hello world! мяу мяу");
  // std::vector<Action> acts=databasa.get_history(1);
  // ActionGetAllDTO tmp{1};
  // std::vector<Action> acts = databasa.get_all_actions_of_task(tmp);
}

TEST_CASE("Files") {
  DataBase databasa;
  std::vector<AttachedFile> f;
  f.push_back({1, "/home/project", "filename"});
  databasa.add_files_to_task(1, f);
  databasa.get_all_files(1);
}
