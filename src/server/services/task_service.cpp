//
// Created by egor on 17.04.2021.
//
#include "task_service.hpp"
#include <exception>
#include "app_db_connection.hpp"
#include "serialization.h"

Task task_service::create_task(TaskCreateDTO const &taskDTO) {
  Task task = from_dto(taskDTO);

  // TODO : generate date right way
  task.due_date = "01-01-2021";
  task.id = /*1*/ get_app_db().create_task(task);

  return task;
}

std::vector<Task> task_service::get_tasks(TaskGetAllDTO &dto) {
  //  std::vector<Task> v;
  //  for (uint32_t i = 0; i < 10; ++i) {
  //    v.push_back({i, "task " + std::to_string(i + 1), "task description " +
  //    std::to_string(i + 1)});
  //  }
  //  return v;
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
