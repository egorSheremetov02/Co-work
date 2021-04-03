//
// Created by egor on 06.03.2021.
//
#include "handler_registration.h"

HandlerStorage & get_handler_storage() {
    static auto *storage = new HandlerStorage();
    return *storage;
}

