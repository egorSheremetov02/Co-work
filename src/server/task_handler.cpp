//
// Created by egor on 06.03.2021.
//
#include <iostream>
#include <nlohmann/json.hpp>
#include "handler_registration.h"
#include "handler.h"

using nlohmann::json;

void get_all_tasks_handler(json & j) {
    std::cout << "Get all tasks handler" << std::endl;
}

REGISTER_HANDLER("task get all", get_all_tasks_handler);
