//
// Created by egor on 27.02.2021.
//
#include "controller.hpp"
#include <nlohmann/json.hpp>
#include "handler_registration.hpp"

Handler &ApplicationController::get_handler(std::string const &resource) {
  return get_handler_storage()[resource];
}
