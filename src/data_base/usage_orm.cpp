#include "../shared/structures.h"
#include <iostream>
#include <optional>
#include <pqxx/pqxx>
#include <string>
#include <vector>
#include "orm.h"
//#include "../shared/db_connection.h"

int main(){

db::Users users{"users"};
users.add_relations("tasks","dependence_task_user");

std::vector<User> a;
a=users.select_where(users.account_name == "admin" and users.password == "qwerty");

std::cout<<a[0].id<<std::endl;
std::cout<<a[0].account_name<<std::endl;
std::cout<<a[0].full_name<<std::endl;

}
