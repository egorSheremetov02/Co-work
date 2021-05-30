//
// Created by egor on 06.03.2021.
//

#ifndef CO_WORK_HANDLER_REGISTRATION_HPP
#define CO_WORK_HANDLER_REGISTRATION_HPP
#include <functional>
#include <iostream>
#include <nlohmann/json.hpp>
#include <string>
#include <unordered_map>
#include "src/server/core/handler.hpp"

using HandlerStorage = std::unordered_map<std::string, Handler>;

HandlerStorage &get_handler_storage();

#define REGISTER_HANDLER_INTERNAL(resource, handler, line)         \
                                                                   \
  const struct InitHandler##line {                                 \
    InitHandler##line() {                                          \
      std::cout << "Registered handler for resource: " << resource \
                << std::endl;                                      \
      get_handler_storage()[resource] = handler;                   \
    }                                                              \
  } init_handler_##line

// NOLINTNEXTLINE: cppcoreguidelines-macro-usage
#define REGISTER_HANDLER_INTERNAL1(line, resource, handler) \
  REGISTER_HANDLER_INTERNAL(resource, handler, line)
// NOLINTNEXTLINE: cppcoreguidelines-macro-usage
#define REGISTER_HANDLER(resource, handler) \
  REGISTER_HANDLER_INTERNAL1(__LINE__, resource, handler)

#endif  // CO_WORK_HANDLER_REGISTRATION_HPP
