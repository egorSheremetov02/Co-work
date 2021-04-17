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

  struct Expression {
    std::string expr;
    Expression operator and(Expression other){
      return Expression{expr+" AND "+other.expr};
    }

    Expression operator ==(std::string data){
      return Expression{expr + "=" + "\'" +data+ "\'"};
    }

    Expression operator ==(int data){
      return Expression{expr + "=" + "\'" +std::to_string(data)+ "\'"};
    }
  };

  template <typename T>
  struct Table{

    std::string table;
      Table(std::string t):table(t){};
    std::map<std::string,std::string> relations;
    virtual std::string on_tables(std::string str){};

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
        out(tmp);
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
      C.prepare("select","SELECT * FROM "+table+" WHERE " + request.expr);
      pqxx::result R{W.exec_prepared("select")};
      return convert_to_vector(R);
    } catch (std::exception const &e) {
        std::cerr << e.what() << std::endl;
    }
    return {};
       }

  std::vector<T> select_join_where(std::string table_to_join,Expression request){
    try{
    pqxx::connection C("dbname=co-work_db user=co-work");
    pqxx::work W{C};
    C.prepare("select","SELECT * FROM "+ relations[table_to_join]+ " INNER JOIN " + table +" ON "+on_tables(relations[table_to_join])+" WHERE " + request.expr);
    pqxx::result R{W.exec_prepared("select")};
    return convert_to_vector(R);
  } catch (std::exception const &e) {
      std::cerr << e.what() << std::endl;
  }
  return {};
    }

   int insert(T object){ //return id in db
     try{
     pqxx::connection C("dbname=co-work_db user=co-work");
     pqxx::work W{C};
     pqxx::result R = W.exec("INSERT INTO "+table+to_orm(object));
     W.commit();
     return R[0][0].as<int>();
   } catch (std::exception const &e) {
       std::cerr << e.what() << std::endl;
   }
   }
template<typename Z>
   bool insert_join(std::string table_to_join,T object1,Z object2){
     try{
     pqxx::connection C("dbname=co-work_db user=co-work");
     pqxx::work W{C};
     pqxx::result R = W.exec("INSERT INTO "+relations[table_to_join]+to_orm(object1,object2));
     W.commit();
     return true;
   } catch (std::exception const &e) {
       std::cerr << e.what() << std::endl;
   }
   return false;
   }

   void update(int id,Expression request){
     try{
     pqxx::connection C("dbname=co-work_db user=co-work");
     pqxx::work W{C};
     pqxx::result R = W.exec("UPDATE "+table+" SET "+request.expr +"WHERE id='"+std::to_string(id)+"'");
     W.commit();
  //   return R[0][0].as<int>();
     } catch (std::exception const &e) {
       std::cerr << e.what() << std::endl;
     }
   }

    };
struct Users : Table<User> {
  Users(std::string t):Table(t){};
Expression id{"id"},user_id{"user_id"},account_name{"account_name"},full_name{"full_name"},password{"password"};
std::string on_tables(std::string str) override {
  if(str=="dedependence_task_user")
    return "users.id = "+str+".task_id";
    return "users.id = "+str+".project_id";
}
};
struct Tasks : Table<Task> {
  Tasks(std::string t):Table(t){};
Expression id{"id"},task_id{"task_id"},name{"name"},description{"description"},date{"date"},project_id{"project_id"},urgency{"urgency"},status{"status"};
std::string on_tables(std::string str) override {
    return "tasks.id = "+str+".task_id";
}
};

struct Projects : Table<Project> {
    Projects(std::string t):Table(t){};
Expression id{"id"},name{"name"},date{"date"},project_id{"project_id"};
std::string on_tables(std::string str) override {
  return "projects.id = "+str+".task_id";
}
};

}


#endif //CO_WORK_ORM_H
