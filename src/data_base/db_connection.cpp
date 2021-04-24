#include "../shared/structures.h"
#include <iostream>
#include <optional>
#include <pqxx/pqxx>
#include <string>
#include <vector>
#include "db_connection.h"

DataBase::DataBase() : users("users"), tasks("tasks"), projs("projects") {
    users.add_relations("tasks", "dependence_task_user");
    tasks.add_relations("users", "dependence_task_user");
    projs.add_relations("users", "dependence_project_user");
}

int DataBase::create_task(Task const &t) {
    return tasks.insert(t);
}

int DataBase::create_project(Project const &t) {
    return projs.insert(t);
}

std::optional<User> DataBase::auth(std::string const &login, std::string const &password) {
    std::vector<User> tmp = users.select_where(users.account_name == login and users.password == password);
    if (tmp.size() == 1) {
        return tmp[0];
    } else {
        return std::nullopt;
    }
}
