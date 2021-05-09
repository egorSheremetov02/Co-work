#ifndef CO_WORK_ORM_H
#define CO_WORK_ORM_H

#include <iostream>
#include <optional>
#include <pqxx/pqxx>
#include <sstream>
#include <string>
#include <utility>
#include <vector>
#include "../shared/structures.h"
#include "serialization_orm.h"

namespace db {

struct Expression {
  std::string expr_;

  Expression(std::string expr) : expr_(std::move(expr)) {}

  Expression operator&&(Expression const &other) const {
    return Expression{expr_ + " AND " + other.expr_};
  }

  Expression operator==(std::string const &data) const {
    return Expression{expr_ + "=" + "\'" + data + "\'"};
  }

  Expression operator==(int data) const {
    return Expression{expr_ + "=" + "\'" + std::to_string(data) + "\'"};
  }
  Expression operator>(int data) const {
    return Expression{expr_ + ">" + "\'" + std::to_string(data) + "\'"};
  }

  Expression operator<(int data) const {
    return Expression{expr_ + "<" + "\'" + std::to_string(data) + "\'"};
  }

  Expression operator+=(Expression const &other) const {
    return Expression{expr_ + ", " + other.expr_};
  }
};
template <typename T>
struct Table;

struct Select {
  std::string what{};
  std::string condition{};
  std::string order{};
  uint32_t lim{};
  uint32_t off{};

  Select() = default;

  template <typename T>
  Select operator()(Table<T> const &table_) {
    what = table_.table_;
    return *this;
  }

  Select where(Expression const &request) {
    condition = request.expr_;
    return *this;
  }
  Select limit(uint32_t limit_) {
    lim = limit_;
    return *this;
  }
  Select offset(uint32_t offset_) {
    off = offset_;
    return *this;
  }

  template <typename T>
  Select join(Table<T> &t) {
    what =
        t.relations_[what] + " INNER JOIN " + what + " ON " + t.on_tables(what);
    return *this;
  }

  Select order_by(std::string const &column) {
    this->order = column;
    return *this;
  }

  [[nodiscard]] std::string build() const {
    std::string result = "SELECT * FROM " + what;
    if (!condition.empty()) {
      result += " WHERE " + condition;
    }
    if (!order.empty()) {
      result += " ORDER BY " + order;
    }
    if (lim != 0) {
      result += " LIMIT " + std::to_string(lim);
    }
    if (off != 0) {
      result += " OFFSET " + std::to_string(off);
    }
    std::cout << result << std::endl;
    return result;
  }
};

template <typename T>
struct Table {
  std::string table_;
  pqxx::connection *C;
  Table(std::string t, pqxx::connection *c) : table_(std::move(t)), C(c){};
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
      out(tmp);
    }
    return data;
  }

  std::vector<T> operator()(Select const &select) {
    try {
      pqxx::work W{*C};
      pqxx::result R{W.exec(select.build())};
      W.commit();
      return convert_to_vector(R);
    } catch (std::exception const &e) {
      std::cerr << e.what() << std::endl;
    }
    return {};
  }

  int insert(T const &object) {  // return id in db
    try {
      pqxx::work W{*C};
      pqxx::result R = W.exec("INSERT INTO " + table_ + to_orm(object));
      W.commit();
      return R[0][0].as<int>();
    } catch (std::exception const &e) {
      std::cerr << e.what() << std::endl;
    }
    return -1;
  }
  /*
    void delete(Expression const &request) {  // TODO return VALUES
      try {
        pqxx::work W{*C};
        pqxx::result R =
            W.exec("DELETE FROM " + table_ + " WHERE " + request.expr_);
        W.commit();

      } catch (std::exception const &e) {
        std::cerr << e.what() << std::endl;
      }
    }*/

  template <typename Z>
  bool insert_join(std::string const &table_to_join,
                   T const &object1,
                   Z const &object2) {
    try {
      pqxx::work W{*C};
      pqxx::result R = W.exec("INSERT INTO " + relations_[table_to_join] +
                              to_orm(object1, object2));
      W.commit();
      return true;
    } catch (std::exception const &e) {
      std::cerr << e.what() << std::endl;
    }
    return false;
  }

  bool update(int id, Expression const &request) {
    try {
      pqxx::work W{*C};
      pqxx::result R = W.exec("UPDATE " + table_ + " SET " + request.expr_ +
                              "WHERE id='" + std::to_string(id) + "'");
      W.commit();
    } catch (std::exception const &e) {
      std::cerr << e.what() << std::endl;
    }
  }
};

struct Users : Table<User> {
  Users(std::string t, pqxx::connection *c) : Table(std::move(t), c){};
  Expression id{"id"}, user_id{"user_id"}, account_name{"account_name"},
      full_name{"full_name"}, password{"password"}, email{"email"};

  std::string on_tables(std::string const &str) override {
    if (str == "tasks") {
      return str + ".id = " + relations_[str] + ".task_id";
    }
    return str + ".id = " + relations_[str] + ".project_id";
  }
};

struct Tasks : Table<Task> {
  Tasks(std::string t, pqxx::connection *c) : Table(std::move(t), c){};
  Expression id{"id"}, task_id{"task_id"}, name{"name"},
      description{"description"}, due_date{"due_date"},
      project_id{"project_id"}, urgency{"urgency"}, status{"status"},
      start_date{"start_date"};

  std::string on_tables(std::string const &str) override {
    return str + ".id = " + relations_[str] + ".task_id";
  }
};

struct Projects : Table<Project> {
  Projects(std::string t, pqxx::connection *c) : Table(t, c){};
  Expression id{"id"}, name{"name"}, due_date{"due_date"},
      project_id{"project_id"}, start_date{"start_date"};

  std::string on_tables(std::string const &str) override {
    return str + ".id = " + relations_[str] + ".task_id";
  }
};

}  // namespace db

#endif  // CO_WORK_ORM_H
