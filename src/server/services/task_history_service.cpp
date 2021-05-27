//
// Created by egor on 27.05.2021.
//
#include "task_history_service.h"
#include "app_db_connection.hpp"

std::vector<Action> get_task_history(ActionGetAllDTO const &actionDTO) {
  return get_app_db().get_all_actions_of_task(actionDTO);
}
