//
// Created by egor on 06.03.2021.
//

#ifndef CO_WORK_HANDLER_HPP
#define CO_WORK_HANDLER_HPP
#include <functional>
#include <nlohmann/json.hpp>
#include "tcp_connection.hpp"

using Handler = std::function<void(nlohmann::json &, TcpConnection::pointer &)>;

#endif  // CO_WORK_HANDLER_HPP
