//
// Created by egor on 27.05.2021.
//
#include "util.h"
#include "globals.h"

std::string util::project_subscription_name(uint32_t project_id) {
  return defaults::server::project_subscription_prefix +
         std::to_string(project_id);
}
