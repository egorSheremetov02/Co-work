#ifndef CO_WORK_ORM_H
#define CO_WORK_ORM_H
#include "../shared/structures.h"
#include <iostream>
#include <optional>
#include <pqxx/pqxx>
#include <string>
#include <sstream>
#include <vector>
#include "serialization_orm.h"

namespace db {

  struct test{
    int a;
    test operator or(test other){
      return test{a + other.a};
    }
    test operator ==(test &other){
      return test{a*other.a};
    }
    std::ostream &operator<<(std::ostream &os) {
        return os << a;
    }
  };

  struct Expression {
    std::string expr;
    Expression operator and(Expression other){
      return Expression{expr+" AND "+other.expr};
    }

    Expression operator ==(std::string data){
      return Expression{expr + "=" + "\'" +data+ "\'"};
    }
  };

  template <typename T>
  struct Table{
    std::string table;
    std::map<std::string,std::string> relations;

    void add_relations(std::string relate_to, std::string table){
      relations[relate_to]=table;
    }

    std::vector<T> convert_to_vector(const pqxx::result &R){
      if(R.size()==0)
      return {};
      std::vector<T> data;
      for(const auto &row : R){
        T tmp;
        from_orm(row,tmp);
        data.push_back(tmp);
      }
      return data;
    }

    std::vector<T> select_all(){
      try{
      pqxx::connection C("dbname=co-work_db user=co-work");
      pqxx::work W{C};
      C.prepare("select","SELECT * FROM "+table);
      pqxx::result R{W.exec_prepared("select")};
      return convert_to_vector(R);
    } catch (std::exception const &e) {
        std::cerr << e.what() << std::endl;
    }
    return {};
    }

    std::vector<T> select_where(Expression request){
      try{
      pqxx::connection C("dbname=co-work_db user=co-work");
      pqxx::work W{C};
    //  std::string req="SELECT * FROM "+table+" WHERE " + request.expr;
      C.prepare("select","SELECT * FROM "+table+" WHERE " + request.expr);
      pqxx::result R{W.exec_prepared("select")};
      return convert_to_vector(R);
    } catch (std::exception const &e) {
        std::cerr << e.what() << std::endl;
    }
    return {};
       }

  std::vector<T> select_join_where(std::string table_to_join,bool expression){
    try{
    pqxx::connection C("dbname=co-work_db user=co-work");
    pqxx::work W{C};
    C.prepare("select","SELECT * FROM "+table+" INNER JOIN " + relations[table_to_join] + " INNER JOIN " + table_to_join);
    pqxx::result R{W.exec_prepared("select")};
    return convert_to_vector(R);
  } catch (std::exception const &e) {
      std::cerr << e.what() << std::endl;
  }
  return {};
    }

   bool insert_where(T object){

   }

   bool update(T object){

   }

    };
struct Users : Table<User> {
Expression id{"id"},account_name{"account_name"},full_name{"full_name"},password{"password"};

};
}

#endif //CO_WORK_ORM_H
