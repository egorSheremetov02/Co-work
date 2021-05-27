//
// Created by egor on 27.02.2021.
//

#ifndef CO_WORK_CONTROLLER_H
#define CO_WORK_CONTROLLER_H

#include <functional>
#include <nlohmann/json.hpp>
#include "handler.h"
#include "handler_registration.h"

struct ApplicationController {
    static Handler & get_handler(std::string const & resource);
};

#endif //CO_WORK_CONTROLLER_H