//
// Created by egor on 27.02.2021.
//
#include "controller.h"
#include <nlohmann/json.hpp>
#include "handler_registration.h"

Handler &ApplicationController::get_handler(std::string const &resource) {
  return get_handler_storage()[resource];
}
