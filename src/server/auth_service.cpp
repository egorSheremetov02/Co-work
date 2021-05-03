//
// Created by egor on 27.02.2021.
//
#include "auth_service.h"
#include <optional>
#include "app_db_connection.h"
#include "request.h"
#include "structures.h"

std::optional<User> AuthService::validate(const AuthReqDTO &payload) const {
  if (get_app_db().auth(payload.login, payload.password)) {
    User u;
    u.account_name = "egor.suvorov";
    u.role_in_system = ADMIN;
    u.full_name = "Egor Suvorov";
    u.id = 1;
    return std::optional<User>{u};
  }
  return std::optional<User>{};
}
