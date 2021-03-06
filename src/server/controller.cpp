//
// Created by egor on 27.02.2021.
//
#include <nlohmann/json.hpp>
#include "controller.h"
#include "handler_registration.h"

Handler & ApplicationController::get_handler(std::string const & resource) {
    return get_handler_storage()[resource];
}

