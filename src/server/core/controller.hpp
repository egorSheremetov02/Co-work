//
// Created by egor on 27.02.2021.
//

#ifndef CO_WORK_CONTROLLER_HPP
#define CO_WORK_CONTROLLER_HPP

#include <functional>
#include <nlohmann/json.hpp>
#include "handler.hpp"
#include "handler_registration.hpp"

struct ApplicationController {
  static Handler &get_handler(std::string const &resource);
};

#endif  // CO_WORK_CONTROLLER_HPP
