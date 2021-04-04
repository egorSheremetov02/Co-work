//
// Created by egor on 04.04.2021.
//

#ifndef CO_WORK_TASK_SERVICE_H
#define CO_WORK_TASK_SERVICE_H

#include "../shared/structures.h"
#include <nlohmann/json.hpp>

struct TaskService {
    Task create_task() {
        return {
                rand() % 1e9,
                "test task",
                "test description",
                "01.09.2021",
                rand() % 1e9,
                9,
                "in progress"
        };
    }

    void
};

#endif //CO_WORK_TASK_SERVICE_H
