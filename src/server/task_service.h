//
// Created by egor on 04.04.2021.
//

#ifndef CO_WORK_TASK_SERVICE_H
#define CO_WORK_TASK_SERVICE_H

#include "../shared/structures.h"
#include <vector>
#include <nlohmann/json.hpp>

struct TaskService {
    static Task create_task(TaskCreateDTO const &);

    static std::vector<Task> get_tasks();

    static Task edit_task(uint32_t);

};

#endif //CO_WORK_TASK_SERVICE_H
