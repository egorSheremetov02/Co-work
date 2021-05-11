#ifndef CO_WORK_SERIALIZATION_ORM_H
#define CO_WORK_SERIALIZATION_ORM_H

#include <pqxx/pqxx>
#include "../shared/structures.h"

inline std::string from_enum(Actions act) {
  if (act == Actions::EDIT_TASK)
    return "edit";
  if (act == Actions::CREATE_TASK)
    return "create";
  if (act == Actions::ADD_COMMENT)
    return "comment";
}

inline Actions to_enum(std::string str, Action &act) {
  if (str == "create")
    return Actions::CREATE_TASK;
  /*if(act==Actions::CREATE_TASK)
  return "create";
  if(act==Actions::ADD_COMMENT)
  return "comment";*/
}

inline void from_orm(pqxx::row const &row, User &user) {
  user.id = row["id"].as<int>();
  user.account_name = row["account_name"].c_str();
  user.full_name = row["full_name"].c_str();
  user.email = row["email"].c_str();
  // user.role_in_system=row["role_in_system"].c_str();
}

inline void from_orm(pqxx::row const &row, Project &proj) {
  proj.id = row["id"].as<int>();
  proj.due_date = row["due_date"].c_str();
  proj.name = row["name"].c_str();
  proj.start_date = row["start_date"].c_str();
}

inline void from_orm(pqxx::row const &row, Task &task) {
  task.id = row["id"].as<int>();
  task.name = row["name"].c_str();
  task.description = row["description"].c_str();
  task.due_date = row["due_date"].c_str();
  task.start_date = row["start_date"].c_str();
  task.project_id =
      (row["project_id"].is_null()) ? -1 : row["project_id"].as<int>();
  task.urgency = (row["urgency"].is_null()) ? -1 : row["urgency"].as<int>();
  task.status = row["status"].c_str();
}

inline void from_orm(pqxx::row const &row, Action &act) {
  act.task_id = row["task_id"].as<int>();
  act.user_id = row["user_id"].as<int>();
  act.date_of_action = row["date_of_action"].c_str();
  act.data = row["object_of_action"].c_str();
  act.action_type = to_enum(row["type_of_action"].c_str(), act);
}

inline void from_orm(pqxx::row const &row, AttachedFile &file) {
  file.task_id = row["task_id"].as<int>();
  file.path_to_file = row["path_to_file"].c_str();
  file.filename = row["filename"].c_str();
}

inline void out(Task const &task) {
  std::cout << task.id << " " << task.name << " " << task.description << " "
            << task.due_date << " " << task.project_id << " " << task.start_date
            << std::endl;
}

inline void out(User const &user) {
  std::cout << user.id << " " << user.account_name << std::endl;
}

inline void out(Project const &proj) {
  std::cout << proj.id << " " << proj.name << std::endl;
}

inline void out(Action const &act) {
  std::cout << act.task_id << " " << from_enum(act.action_type) << " "
            << act.data << std::endl;
}

inline void out(AttachedFile const &file) {
  std::cout << file.task_id << " " << file.filename << std::endl;
}

inline std::string to_orm(Project const &proj) {
  std::string sql = " (name,due_date) VALUES ('" + proj.name + "','" +
                    proj.due_date + "') RETURNING id";
  return sql;
}

inline std::string to_orm(Action const &act) {
  std::string sql =
      " (task_id,user_id,type_of_action,object_of_action) VALUES ('" +
      std::to_string(act.task_id) + "','" + std::to_string(act.user_id) +
      "','" + from_enum(act.action_type) + "','" + act.data + "') ";
  std::cout << sql << std::endl;
  return sql;
}

inline std::string to_orm(AttachedFile const &file) {
  std::string sql = " (task_id,path_to_file,filename) VALUES ('" +
                    std::to_string(file.task_id) + "','" + file.path_to_file +
                    "','" + file.filename + "') ";
  return sql;
}

// TODO

inline std::string to_orm(User const &user) {
  std::string sql = " (account_name,full_name,email) VALUES ('" +
                    user.account_name + "','" + user.account_name +
                    /*"','" +user.role_in_system+ */ "','" + user.email +
                    "') RETURNING id";
  return sql;
}

inline std::string to_orm(Task const &task) {
  std::string sql =
      " (name,description,due_date,project_id,urgency,status) VALUES ('" +
      task.name + "','" + task.description + "','" + task.due_date + "','" +
      std::to_string(task.project_id) + "','" + std::to_string(task.urgency) +
      "','" + task.status + "') RETURNING id";
  return sql;
}

inline std::string to_orm(Task const &task, User const &user) {
  std::string sql = " (task_id,user_id) VALUES ('" + std::to_string(task.id) +
                    "','" + std::to_string(user.id) + "')";
  return sql;
}

inline std::string to_orm(User const &user, Task const &task) {
  std::string sql = " (task_id,user_id) VALUES ('" + std::to_string(task.id) +
                    "','" + std::to_string(user.id) + "')";
  return sql;
}

inline std::string to_orm(Project const &proj, User const &user) {
  std::string sql = " (project_id,user_id) VALUES ('" +
                    std::to_string(proj.id) + "','" + std::to_string(user.id) +
                    "')";
  return sql;
}

inline std::string to_orm(User const &user, Project const &proj) {
  std::string sql = " (project_id,user_id) VALUES ('" +
                    std::to_string(proj.id) + "','" + std::to_string(user.id) +
                    "')";
  return sql;
}

#endif  // CO_WORK_SERIALIZATION_ORM_H
