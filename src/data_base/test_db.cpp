#include "db_connection.h"

int main(){
  //std::cout<<auth("admin","qwerty")->full_name<<std::endl;

  std::vector<Task> data=get_tasks(1);

for(auto & z: data){
  std::cout<<z.name<<std::endl;}

}
