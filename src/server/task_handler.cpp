//
// Created by egor on 06.03.2021.
//
#include <iostream>
#include <nlohmann/json.hpp>
#include "handler_registration.h"
#include "handler.h"

using nlohmann::json;

void get_all_tasks_handler(json & j) {
#ifdef LOGGING
    std::cout << "Get all tasks handler" << std::endl;
#endif
}

void create_task_handler(json & j) {
#ifdef LOGGING
    std::cout << "Create new task" << std::endl;
#endif
}

REGISTER_HANDLER("task get_all", get_all_tasks_handler);
REGISTER_HANDLER("task create", create_task_handler);
