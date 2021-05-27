//
// Created by egor on 27.05.2021.
//
#include <iostream>
#include <nlohmann/json.hpp>
#include "globals.h"
#include "handler_registration.hpp"
#include "request_format.h"
#include "serialization.h"
#include "structures.h"
#include "task_history_service.h"
#include "tcp_connection.hpp"

using json = nlohmann::json;

void get_history(json &in_json, TcpConnection::pointer &connection) {
  json out_json;
  auto actionsDTO = in_json.get<RequestFormat<ActionGetAllDTO>>().data;
  std::vector<Action> actions =
      task_history_service::get_task_history(actionsDTO);
  out_json = actions;
  connection->out_message() = out_json.dump();
  connection->socket().async_write_some(
      asio::buffer(connection->out_message().data(),
                   connection->out_message().size()),
      [&](asio::error_code const &ec, [[maybe_unused]] std::size_t len) {
        if (ec) {
          connection->socket().close();
          delete connection;
        }
      });
}

REGISTER_HANDLER(endpoints::task_history::get_all, get_history);
