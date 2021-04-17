#include "../shared/structures.h"
#include <iostream>
#include <optional>
#include <pqxx/pqxx>
#include <string>
#include <vector>
#include "db_connection.h"

DataBase::DataBase():users("users"),tasks("tasks"),projs("projects"){
  users.add_relations("tasks","dependence_task_user");
  tasks.add_relations("users","dependence_task_user");
  projs.add_relations("users","dependence_project_user");
}

int DataBase::create_task(Task t){
 return tasks.insert(t);
}
