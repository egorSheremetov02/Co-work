//
// Created by egor on 27.02.2021.
//

#ifndef CO_WORK_AUTH_SERVICE_H
#define CO_WORK_AUTH_SERVICE_H

#include "request.h"
#include "structures.h"

struct AuthService {
  [[nodiscard]] std::optional<User> validate(const AuthReqDTO &payload) const;
};

#endif  // CO_WORK_AUTH_SERVICE_H
