//
// Created by egor on 27.02.2021.
//
#include "controller.h"

Handler ApplicationController::get_handler(std::string const & resource) {
    if (resource.find("projects get")) {
        return [&](json & j) {

        };
    } else if (resource.find("tasks get")) {
        return [&](json & j) {

        };
    } else if (resource.find("projects get_all")) {
        return [&](json & j) {

        };
    }
    return [&](json & j) {

    };
}

