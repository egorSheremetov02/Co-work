#include "../shared/structures.h"
#include <iostream>
#include <optional>
#include <pqxx/pqxx>
#include <string>
#include <vector>
#include "orm.h"
#include "db_connection.h"
//#include "../shared/db_connection.h"

int main(){

/*db::Users users("users");
db::Tasks tasks("tasks");
db::Projects projs("projects");

users.add_relations("tasks","dependence_task_user");
tasks.add_relations("users","dependence_task_user");
projs.add_relations("users","dependence_project_user");

User current=users.select_where(users.account_name == "admin" and users.password == "qwerty")[0];

Project p=projs.select_where(projs.id==3)[0];
//p.id=projs.insert(p);
//projs.insert_join("users",p,current);

Task t1{0,"Task_P1","Description","2021-04-17 06:05:05",p.id,7,"status"};
Task t2{0,"Task_P2","Description","2021-04-17 06:05:05",p.id,7,"status"};

//t1.id=tasks.insert(t1);
//t2.id=tasks.insert(t2);
//tasks.insert_join("users",t1,current);
std::vector<Task> b1=tasks.select_join_where("users",users.user_id == current.id and tasks.project_id==p.id);
std::cout<<"!\n";
std::vector<Task> b2=tasks.select_where(tasks.project_id==p.id);
std::cout<<"aaa\n";
Task tt=tasks.select_where(tasks.id==14)[0];
tasks.update(tt.id,tasks.description == "afa");
std::cout<<"aaa\n";
std::vector<Task> b4=tasks.select_where(tasks.project_id==p.id);
*/
//std::vector<Project> pp=projs.select_all();
DataBase databasa;
Task t{0,"Task_task","Description","2021-04-17 06:05:05",3,7,"status"};
int tt=databasa.create_task(t);

}
