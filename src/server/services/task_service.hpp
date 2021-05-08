//
// Created by egor on 04.04.2021.
//

#ifndef CO_WORK_TASK_SERVICE_HPP
#define CO_WORK_TASK_SERVICE_HPP

#include <nlohmann/json.hpp>
#include <vector>
#include "structures.h"

namespace task_service {
Task create_task(TaskCreateDTO const &);

std::vector<Task> get_tasks(TaskGetAllDTO &dto);

Task edit_task(uint32_t);
};  // namespace task_service

#endif  // CO_WORK_TASK_SERVICE_HPP
