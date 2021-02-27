#ifndef CO_WORK_DB_CONNECTION_H
#define CO_WORK_DB_CONNECTION_H
#include "../shared/structures.h"
#include <iostream>
#include <optional>
#include <pqxx/pqxx>
#include <string>
#include <vector>

std::optional<User> load_user(const pqxx::result &R);

std::optional<Task> load_task();

std::optional<User> auth(std::string account_name, std::string password) ;

std::vector<Task> get_tasks(unsigned int user_id);

std::vector<Task> load_vector_of_tasks(const pqxx::result &R);
/*
std::vector<task> get_all_tasks(std::string user_id,pqxx::work& W){

std::vector<task> result;
pqxx::result R{W.exec("SELECT id,name FROM users WHERE id="+user_id)};
for(auto row: R){
  resultt.push_back({0,row[1].c_str()});
}
  return resultt;
}

int main()
{
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

 //std::cout<<auth("admin","qwerty")->full_name;

}*/
#endif //CO_WORK_DB_CONNECTION_H
