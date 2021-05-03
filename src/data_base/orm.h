#ifndef CO_WORK_ORM_H
#define CO_WORK_ORM_H

#include <iostream>
#include <optional>
#include <pqxx/pqxx>
#include <sstream>
#include <string>
#include <vector>
#include "../shared/structures.h"
#include "serialization_orm.h"

#define GET_RES_STR(token) #token

// clang-format off
#define CONNECTION_STRING(USER) GET_RES_STR(dbname=co-work_db user=USER)
//clang-format on

namespace db {

struct Expression {
  std::string expr_;

  Expression operator&&(Expression const &other) const {
    return Expression{expr_ + " AND " + other.expr_};
  }

  Expression operator==(std::string const &data) const {
    return Expression{expr_ + "=" + "\'" + data + "\'"};
  }

  Expression operator==(int data) const {
    return Expression{expr_ + "=" + "\'" + std::to_string(data) + "\'"};
  }
};

template <typename T>
struct Table {
  std::string table_;

  Table(std::string t) : table_(std::move(t)){};
  std::map<std::string, std::string> relations_;

  virtual std::string on_tables(std::string const &str) = 0;

  void add_relations(std::string const &relate_to, std::string const &table) {
    relations_[relate_to] = table;
  }

  std::vector<T> convert_to_vector(const pqxx::result &R) {
    if (R.empty()) {
      return {};
    }
    std::vector<T> data;
    for (const auto &row : R) {
      T tmp;
      from_orm(row, tmp);
      data.push_back(tmp);
      //  out(tmp);
    }

    return data;
  }

  std::vector<T> select_all() {
    try {
      pqxx::connection C(CONNECTION_STRING(DBUSER));
      pqxx::work W{C};
      C.prepare("select", "SELECT * FROM " + table_);
      pqxx::result R{W.exec_prepared("select")};
      return convert_to_vector(R);
    } catch (std::exception const &e) {
      std::cerr << e.what() << std::endl;
    }
    return {};
  }

  std::vector<T> select_where(Expression const &request) {
    try {
      pqxx::connection C(CONNECTION_STRING(DBUSER));
      pqxx::work W{C};
      C.prepare("select",
                "SELECT * FROM " + table_ + " WHERE " + request.expr_);
      pqxx::result R{W.exec_prepared("select")};
      return convert_to_vector(R);
    } catch (std::exception const &e) {
      std::cerr << e.what() << std::endl;
    }
    return {};
  }

  std::vector<T> select_join_where(std::string const &table_to_join,
                                   Expression const &request) {
    try {
      pqxx::connection C(CONNECTION_STRING(DBUSER));
      pqxx::work W{C};
      C.prepare("select", "SELECT * FROM " + relations_[table_to_join] +
                              " INNER JOIN " + table_ + " ON " +
                              on_tables(relations_[table_to_join]) + " WHERE " +
                              request.expr_);
      pqxx::result R{W.exec_prepared("select")};
      return convert_to_vector(R);
    } catch (std::exception const &e) {
      std::cerr << e.what() << std::endl;
    }
    return {};
  }

  int insert(T const &object) {  // return id in db
    try {
      pqxx::connection C(CONNECTION_STRING(DBUSER));
      pqxx::work W{C};
      pqxx::result R = W.exec("INSERT INTO " + table_ + to_orm(object));
      W.commit();
      return R[0][0].as<int>();
    } catch (std::exception const &e) {
      std::cerr << e.what() << std::endl;
    }
    return -1;
  }

  template <typename Z>
  bool insert_join(std::string const &table_to_join,
                   T const &object1,
                   Z const &object2) {
    try {
      pqxx::connection C(CONNECTION_STRING(DBUSER));
      pqxx::work W{C};
      pqxx::result R = W.exec("INSERT INTO " + relations_[table_to_join] +
                              to_orm(object1, object2));
      W.commit();
      return true;
    } catch (std::exception const &e) {
      std::cerr << e.what() << std::endl;
    }
    return false;
  }

  void update(int id, Expression const &request) {
    try {
      pqxx::connection C(CONNECTION_STRING(DBUSER));
      pqxx::work W{C};
      pqxx::result R = W.exec("UPDATE " + table_ + " SET " + request.expr_ +
                              "WHERE id='" + std::to_string(id) + "'");
      W.commit();
      //   return R[0][0].as<int>();
    } catch (std::exception const &e) {
      std::cerr << e.what() << std::endl;
    }
  }
};

struct Users : Table<User> {
  Users(std::string t) : Table(std::move(t)){};
  Expression id{"id"}, user_id{"user_id"}, account_name{"account_name"},
      full_name{"full_name"}, password{"password"};

  std::string on_tables(std::string const &str) override {
    if (str == "dedependence_task_user") {
      return "users.id = " + str + ".task_id";
    }
    return "users.id = " + str + ".project_id";
  }
};

struct Tasks : Table<Task> {
  Tasks(std::string t) : Table(std::move(t)){};
  Expression id{"id"}, task_id{"task_id"}, name{"name"},
      description{"description"}, date{"date"}, project_id{"project_id"},
      urgency{"urgency"}, status{"status"};

  std::string on_tables(std::string const &str) override {
    return "tasks.id = " + str + ".task_id";
  }
};

struct Projects : Table<Project> {
  Projects(std::string t) : Table(t){};
  Expression id{"id"}, name{"name"}, date{"date"}, project_id{"project_id"};

  std::string on_tables(std::string const &str) override {
    return "projects.id = " + str + ".task_id";
  }
};

}  // namespace db

#endif  // CO_WORK_ORM_H
