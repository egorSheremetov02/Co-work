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
struct Prepared {
  std::string variable{};
  OptionalField<uint32_t> data_int;
  OptionalField<std::string> data_str;

  Prepared(std::string var, std::string str) : variable(var), data_str(str){};
  Prepared(std::string var, uint32_t num) : variable(var), data_int(num){};
  void out() {
    // std::cout << data_int.get() << std::endl;
  }
};

struct Expression {
  std::string expr_{};
  mutable std::vector<Prepared> data_;

  Expression() = default;

  Expression(std::string expr) : expr_(std::move(expr)) {}
  Expression(std::string expr, std::vector<Prepared> d)
      : expr_(std::move(expr)), data_(d) {}
  Expression(std::string expr, std::vector<Prepared> d, Prepared p)
      : expr_(std::move(expr)), data_(d) {
    data_.push_back(p);
  }
  Expression(std::string expr,
             std::vector<Prepared> d1,
             std::vector<Prepared> d2)
      : expr_(std::move(expr)), data_(d1) {
    data_.insert(data_.end(), d2.begin(), d2.end());
  }

  Expression operator&&(Expression const other) const {
    //  data_.insert(data_.end(), other.data_.begin(), other.data_.end());
    return Expression{expr_ + " AND " + other.expr_, data_, other.data_};
  }

  Expression operator==(std::string const data) const {
    Prepared p(expr_ + "=", data);
    return Expression{expr_ + "=" + "\'" + data + "\'", data_, p};
  }

  Expression operator==(int data) const {
    Prepared p(expr_ + "=", data);
    return Expression{expr_ + "=" + "\'" + std::to_string(data) + "\'", data_,
                      p};
  }
  Expression operator>(int data) const {
    Prepared p(expr_ + ">", data);
    return Expression{expr_ + ">" + "\'" + std::to_string(data) + "\'", data_,
                      p};
  }

  Expression operator<(int data) const {
    Prepared p(expr_ + "<", data);
    return Expression{expr_ + "<" + "\'" + std::to_string(data) + "\'", data_,
                      p};
  }

  Expression operator+=(Expression other) {
    expr_ += (expr_ == "" ? "" : ", ") + other.expr_;
    data_.insert(data_.end(), other.data_.begin(), other.data_.end());
    return Expression{(expr_ == "" ? "" : ", ") + other.expr_, data_};
  }
  std::string build() const {
    std::string res{};
    for (int i = 1; i <= data_.size(); i++) {
      res += data_[i - 1].variable + "$" + std::to_string(i);
      if (i != data_.size()) {
        res += " AND ";
      }
    }
    return res;
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
  std::vector<std::string> p;

  Select() = default;

  template <typename T>
  Select operator()(Table<T> const &table_) {
    what = table_.table_;
    return *this;
  }

  Select where(Expression const &request) {
    condition = request.expr_;
    for (Prepared z : request.data_) {
      z.out();
      p.push_back(z.data_int.has_value() ? std::to_string(z.data_int.get())
                                         : z.data_str.get());
    }
    condition = request.build();
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

  [[nodiscard]] std::vector<std::string> build(std::string &out) const {
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
#ifdef LOGGING
    std::cout << result << std::endl;
#endif
    out = result;
    return p;
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
#ifdef LOGGING
      out(tmp);
#endif
    }
    return data;
  }

  std::vector<T> operator()(Select const &select) {
    try {
      pqxx::work W{*C};
      std::string request;
      std::vector<std::string> params = select.build(request);
      C->prepare("ex", request);
      pqxx::result R{
          W.exec_prepared("ex", pqxx::prepare::make_dynamic_params(params))};
      W.commit();
      C->unprepare("ex");
      return convert_to_vector(R);
    } catch (std::exception const &e) {
      std::cerr << e.what() << std::endl;
    }
    return {};
  }

  int insert(T const &object) {  // return id in db
    try {
      pqxx::work W{*C};
#ifdef LOGGING
      std::cout << "INSERT INTO " + table_ + to_orm(object) << std::endl;
#endif
      pqxx::result R = W.exec("INSERT INTO " + table_ + to_orm(object));
      W.commit();
      return R[0][0].as<int>();
    } catch (std::exception const &e) {
      std::cerr << e.what() << std::endl;
    }
    return -1;
  }

  bool del(Expression const &request) {
    try {
      pqxx::work W{*C};
#ifdef LOGGING
      std::cout << "DELETE FROM " + table_ + " WHERE " + request.expr_
                << std::endl;
#endif
      pqxx::result R =
          W.exec("DELETE FROM " + table_ + " WHERE " + request.expr_);
      W.commit();
      return true;
    } catch (std::exception const &e) {
      std::cerr << e.what() << std::endl;
      return false;
    }
  }

  bool del_join(std::string const &table_to_join, Expression const &request) {
    try {
      pqxx::work W{*C};
#ifdef LOGGING
      std::cout << "DELETE FROM " + relations_[table_to_join] + " WHERE " +
                       request.expr_
                << std::endl;
#endif
      pqxx::result R = W.exec("DELETE FROM " + relations_[table_to_join] +
                              " WHERE " + request.expr_);
      W.commit();
      return true;
    } catch (std::exception const &e) {
      std::cerr << e.what() << std::endl;
      return false;
    }
  }

  template <typename Z>
  bool insert_join(std::string const &table_to_join,
                   T const &object1,
                   Z const &object2) {
    try {
      pqxx::work W{*C};
#ifdef LOGGING

      std::cout << "INSERT INTO " + relations_[table_to_join] +
                       to_orm(object1, object2)
                << std::endl;
#endif
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
      std::cout << "UPDATE " + table_ + " SET " + request.expr_ + "WHERE id='" +
                       std::to_string(id) + "'"
                << std::endl;
      pqxx::result R = W.exec("UPDATE " + table_ + " SET " + request.expr_ +
                              "WHERE id='" + std::to_string(id) + "'");
      W.commit();
      return true;
    } catch (std::exception const &e) {
      std::cerr << e.what() << std::endl;
      return false;
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

  int insert(RegistrationReqDTO const &dto) {  // return id in db
    try {
      pqxx::work W{*C};
#ifdef LOGGING
      std::cout << "INSERT INTO " + table_ + to_orm(dto) << std::endl;
#endif
      pqxx::result R = W.exec("INSERT INTO " + table_ + to_orm(dto));
      W.commit();
      return R[0][0].as<int>();
    } catch (std::exception const &e) {
      std::cerr << e.what() << std::endl;
    }
    return 0;
  }
};

struct Tasks : Table<Task> {
  Tasks(std::string t, pqxx::connection *c) : Table(std::move(t), c){};
  Expression id{"id"}, task_id{"task_id"}, name{"name"}, user_id{"user_id"},
      description{"description"}, due_date{"due_date"},
      project_id{"project_id"}, urgency{"urgency"}, status{"status"},
      start_date{"start_date"};

  std::string on_tables(std::string const &str) override {
    return "users.id = " + relations_[str] + ".user_id";
  }
};

struct Projects : Table<Project> {
  Projects(std::string t, pqxx::connection *c) : Table(t, c){};
  Expression id{"id"}, name{"name"}, due_date{"due_date"}, user_id{"user_id"},
      project_id{"project_id"}, start_date{"start_date"};

  std::string on_tables(std::string const &str) override {
    return "users.id = " + relations_[str] + ".user_id";
  }
};

struct Actions : Table<Action> {
  Actions(std::string t, pqxx::connection *c) : Table(t, c){};
  Expression task_id{"task_id"}, user_id{"user_id"},
      action_type{"type_of_action"}, date_of_action{"date_of_action"},
      data{"object_of_action"};
  std::string on_tables(std::string const &str) override {
    return str;
  }
};

struct Files : Table<AttachedFile> {
  Files(std::string t, pqxx::connection *c) : Table(t, c){};
  Expression task_id{"task_id"}, path_to_file{"path_to_file"},
      filename{"filename"};
  std::string on_tables(std::string const &str) override {
    return str;
  }
};

}  // namespace db

#endif  // CO_WORK_ORM_H
