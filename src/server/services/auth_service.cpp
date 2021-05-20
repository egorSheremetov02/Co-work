//
// Created by egor on 27.02.2021.
//
#include "auth_service.hpp"
#include <optional>
#include "request.h"
#include "src/server/app_db/app_db_connection.hpp"
#include "structures.h"

std::optional<User> AuthService::validate(const AuthReqDTO &payload) const {
  return get_app_db().auth(payload.login, payload.password);
}
