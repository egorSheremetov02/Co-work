#include "../shared/structures.h"
#include <iostream>
#include <optional>
#include <pqxx/pqxx>
#include <string>
#include <vector>
#include "../shared/db_connection.h"

std::optional<User> load_user(const pqxx::result &R) {
  if (R.size() == 0)
    return {};
  User data{static_cast<unsigned int>(R[0][0].as<int>()), R[0][1].c_str(),
            R[0][2].c_str(), ADMIN};
  return std::optional<User>{data};
}

std::optional<Task> load_task(){
 return {};
}

std::vector<Task> load_vector_of_tasks(const pqxx::result &R){
  if(R.size()==0)
  return {};
  std::vector<Task> data;
  for(const auto &row : R){
     Task tmp{static_cast<unsigned int>(row[0].as<int>()), row[1].c_str(),
               row[2].c_str(),row[3].c_str(),static_cast<unsigned int>(row[4].as<int>()),static_cast<unsigned int>(row[5].as<int>()),row[6].c_str()};
     data.push_back(tmp);
  }
  return data;

}

std::optional<User> auth(std::string account_name, std::string password) {
  try {

    pqxx::connection C("dbname=co-work_db user=co-work");
    pqxx::work W{C};
C.prepare("get_user","SELECT * FROM users WHERE account_name=$1 AND password=$2");
pqxx::result R{W.exec_prepared("get_user",account_name,password)};

    return load_user(R);
    // TODO если несколько одинаковых пользователей? мб чекать при создании
    // новых
  } catch (std::exception const &e) {
    std::cerr << e.what() << '\n';
  }
  return std::optional<User>{};
}

std::vector<Task> get_tasks(unsigned int user_id) {
  try {

    pqxx::connection C("dbname=co-work_db user=co-work");
    pqxx::work W{C};

    C.prepare("get_tasks","SELECT id,name,description,due_date,project_id,urgency,status FROM dependence_task_user INNER JOIN tasks ON tasks.id=dependence_task_user.task_id WHERE dependence_task_user.user_id=$1");
    pqxx::result R{W.exec_prepared("get_tasks",1)};

    return load_vector_of_tasks(R);

  } catch (std::exception const &e) {
    std::cerr << e.what() << '\n';
  }
  return {};
}

/*
std::vector<task> get_all_tasks(std::string user_id,pqxx::work& W){

std::vector<task> result;
pqxx::result R{W.exec("SELECT id,name FROM users WHERE id="+user_id)};
for(auto row: R){
  resultt.push_back({0,row[1].c_str()});
}
  return resultt;
}


    try
    {
        pqxx::connection C("dbname=co-work user=test");
        std::cout << "Connected to " << C.dbname() << std::endl;
        pqxx::work W{C};

        pqxx::result R{W.exec("SELECT name FROM users")};

        std::cout << "Found " << R.size() << "tasks:\n";
        for (auto row: R)
            std::cout << row[0].c_str() << '\n';

        std::cout << "Doubling all employees' salaries...\n";
      //  W.exec0("UPDATE employee SET salary = salary*2");
std::vector<task> out=get_all_tasks("1",W);
for(auto z: out){
  z.print();

}
        std::string name="\"Egor_Suvurov\"";
        W.exec("INSERT INTO tbl (name) VALUES (\'name1\')");
        std::cout << "Making changes definite: ";
        W.commit();
        std::cout << "OK.\n";
    }
    catch (std::exception const &e)
    {
        std::cerr << e.what() << '\n';
        return 1;
    }
    return 0;
*/
