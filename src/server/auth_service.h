//
// Created by egor on 27.02.2021.
//

#ifndef CO_WORK_AUTH_SERVICE_H
#define CO_WORK_AUTH_SERVICE_H

#include <src/shared/structures.h>
#include "src/shared/request.h"

struct AuthService {
  [[nodiscard]] std::optional<User> validate(const AuthReqDTO &payload) const;
};

#endif  // CO_WORK_AUTH_SERVICE_H
