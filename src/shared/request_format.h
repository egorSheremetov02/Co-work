//
// Created by egor on 19.02.2021.
//

#ifndef CO_WORK_REQUEST_FORMAT_H
#define CO_WORK_REQUEST_FORMAT_H

#include <nlohmann/json.hpp>
#include "structures.h"

template <typename T>
struct RequestFormat {
  std::string resource;
  T data;
};

template <typename T>
struct AuthorizedRequestFormat : RequestFormat<T> {
  User user;
  std::string token;
};

#endif  // CO_WORK_MESSAGE_FORMAT_H
