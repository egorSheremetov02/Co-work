//
// Created by egor on 06.03.2021.
//

#ifndef CO_WORK_HANDLER_REGISTRATION_H
#define CO_WORK_HANDLER_REGISTRATION_H
#include <functional>
#include <nlohmann/json.hpp>
#include <string>
#include <unordered_map>
#include "handler.h"

using HandlerStorage = std::unordered_map<std::string, Handler>;

HandlerStorage &get_handler_storage();

#define REGISTER_HANDLER_INTERNAL(resource, handler, line) \
                                                           \
  struct InitHandler##line {                               \
    InitHandler##line() {                                  \
      get_handler_storage()[resource] = handler;           \
    }                                                      \
  } init_handler_##line

// NOLINTNEXTLINE: cppcoreguidelines-macro-usage
#define REGISTER_HANDLER_INTERNAL1(line, resource, handler) \
  REGISTER_HANDLER_INTERNAL(resource, handler, line)
// NOLINTNEXTLINE: cppcoreguidelines-macro-usage
#define REGISTER_HANDLER(resource, handler) \
  REGISTER_HANDLER_INTERNAL1(__LINE__, resource, handler)

#endif  // CO_WORK_HANDLER_REGISTRATION_H
