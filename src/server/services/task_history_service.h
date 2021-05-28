//
// Created by egor on 27.05.2021.
//

#ifndef CO_WORK_TASK_HISTORY_SERVICE_H
#define CO_WORK_TASK_HISTORY_SERVICE_H
#include "serialization.h"
#include "structures.h"

namespace task_history_service {

std::vector<Action> get_task_history(ActionGetAllDTO const &);

}

#endif  // CO_WORK_TASK_HISTORY_SERVICE_H
