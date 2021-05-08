#ifndef CO_WORK_SERIALIZATION_ORM_H
#define CO_WORK_SERIALIZATION_ORM_H

#include <pqxx/pqxx>
#include "../shared/structures.h"

inline void from_orm(pqxx::row const &row, User &user) {
  user.id = row["id"].as<int>();
  user.account_name = row["account_name"].c_str();
  user.full_name = row["full_name"].c_str();
  // user.email = row["email"].c_str();

  // user.role_in_system=row["role_in_system"].c_str();
}

inline void from_orm(pqxx::row const &row, Project &proj) {
  proj.id = row["id"].as<int>();
  proj.date = row["due_date"].c_str();
  proj.name = row["name"].c_str();
}

inline void from_orm(pqxx::row const &row, Task &task) {
  task.id = row["id"].as<int>();
  task.name = row["name"].c_str();
  task.description = row["description"].c_str();
  task.date = row["due_date"].c_str();
  task.project_id =
      (row["project_id"].is_null()) ? -1 : row["project_id"].as<int>();
  task.urgency = (row["urgency"].is_null()) ? -1 : row["urgency"].as<int>();
  task.status = row["status"].c_str();
}

inline void out(Task const &task) {
  std::cout << task.id << " " << task.name << " " << task.description << " "
            << task.date << " " << task.project_id << std::endl;
}

inline void out(User const &user) {
  std::cout << user.id << " " << user.account_name << std::endl;
}

inline void out(Project const &proj) {
  std::cout << proj.id << " " << proj.name << std::endl;
}

inline std::string to_orm(Project const &proj) {
  std::string sql = " (name,due_date) VALUES ('" + proj.name + "','" +
                    proj.date + "') RETURNING id";
  return sql;
}

// TODO

inline std::string to_orm(User const &user) {
  std::string sql =
      " (account_name,full_name,email) VALUES ('" + user.account_name + "','" +
      user.account_name +
      /*"','" +user.role_in_system+  "','" +user.email+*/ "') RETURNING id";
  return sql;
}

inline std::string to_orm(Task const &task) {
  std::string sql =
      " (name,description,due_date,project_id,urgency,status) VALUES ('" +
      task.name + "','" + task.description + "','" + task.date + "','" +
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
