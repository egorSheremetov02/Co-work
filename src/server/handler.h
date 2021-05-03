//
// Created by egor on 06.03.2021.
//

#ifndef CO_WORK_HANDLER_H
#define CO_WORK_HANDLER_H
#include <functional>
#include <nlohmann/json.hpp>
#include "tcp_connection.h"

using Handler = std::function<void(nlohmann::json &, TcpConnection::pointer &)>;

#endif  // CO_WORK_HANDLER_H
