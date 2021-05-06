//
// Created by egor on 06.03.2021.
//
#include <iostream>
#include <nlohmann/json.hpp>
#include "serialization.h"
#include "src/server/core/handler.hpp"
#include "src/server/core/handler_registration.hpp"
#include "task_service.hpp"

using nlohmann::json;

void get_all_tasks_handler(json & /*input_json*/,
                           TcpConnection::pointer &connection) {
  json out_json;
  out_json["tasks"] = task_service::get_tasks();
  connection->out_message() = out_json.dump();
  connection->socket().async_write_some(
      asio::buffer(connection->out_message().data(),
                   connection->out_message().size()),
      [&, connection](asio::error_code const & /*ec*/, std::size_t len) {
#ifdef LOGGING
        std::cout << "Wrote 'task get_all' response: "
                  << connection->out_message().substr(0, len) << std::endl;
#endif
      });
#ifdef LOGGING
  std::cout << "Get all tasks handler" << std::endl;
#endif
}

void create_task_handler(json &in_json, TcpConnection::pointer &connection) {
  json out_json;
  auto taskDTO = in_json.get<RequestFormat<TaskCreateDTO>>();
  out_json["task"] = task_service::create_task(taskDTO.data);
  connection->out_message() = out_json.dump();
  connection->socket().async_write_some(
      asio::buffer(connection->out_message().data(),
                   connection->out_message().size()),
      [&, connection](asio::error_code const & /*ec*/, std::size_t len) {
#ifdef LOGGING
        std::cout << "Wrote 'task create' response: "
                  << connection->out_message().substr(0, len) << std::endl;
#endif
      });
#ifdef LOGGING
  std::cout << "Create new task" << std::endl;
#endif
}

void edit_task_handler(json &in_json, TcpConnection::pointer &connection) {
  json out_json;
  out_json["task"] = task_service::edit_task(in_json.at("task_id"));
  connection->out_message() = out_json.dump();
  connection->socket().async_write_some(
      asio::buffer(connection->out_message().data(),
                   connection->out_message().size()),
      [&, connection](asio::error_code const & /*ec*/, std::size_t len) {
#ifdef LOGGING
        std::cout << "Wrote 'task edit' response: "
                  << connection->out_message().substr(0, len) << std::endl;
#endif
      });
#ifdef LOGGING
  std::cout << "Edit task" << std::endl;
#endif
}

REGISTER_HANDLER("task edit", edit_task_handler);

REGISTER_HANDLER("task get_all", get_all_tasks_handler);

REGISTER_HANDLER("task create", create_task_handler);