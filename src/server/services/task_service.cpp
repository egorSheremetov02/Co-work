//
// Created by egor on 17.04.2021.
//
#include "task_service.hpp"
#include <exception>
#include "app_db_connection.hpp"
#include "serialization.h"

Task task_service::create_task(TaskCreateDTO const &taskDTO) {
  //  task.due_date = "01-01-2021";
  uint32_t task_id = get_app_db().create_task(taskDTO, taskDTO.user_id);
  return get_app_db().get_task(task_id).value();
}

std::vector<Task> task_service::get_tasks(TaskGetAllDTO &dto) {
  return get_app_db().get_all_tasks_of_proj(dto);
}

Task task_service::edit_task(TaskEditDTO const &editDTO) {
  if (get_app_db().update_task(editDTO)) {
    return get_app_db().get_task(editDTO.task_id).value();
  } else {
    // TODO : maybe add custom exception hierarchy
    throw std::invalid_argument("There is no task with given id");
  }
}
