//
// Created by egor on 06.03.2021.
//
#include <iostream>
#include <memory>
#include <nlohmann/json.hpp>
#include <string>
#include "application_context.hpp"
#include "handler_registration.hpp"
#include "multicast.h"
#include "serialization.h"
#include "task_service.hpp"

using nlohmann::json;

void get_all_tasks_handler(json &in_json, TcpConnection::pointer &connection) {
  auto tasksDTO = in_json.get<RequestFormat<TaskGetAllDTO>>().data;
  auto project_id = tasksDTO.project_id;
  json out_json;
  out_json["tasks"] = task_service::get_tasks(tasksDTO);
  connection->out_message() = out_json.dump();
  connection->socket().async_write_some(
      asio::buffer(connection->out_message().data(),
                   connection->out_message().size()),
      [&, connection, project_id](asio::error_code const &ec, std::size_t len) {
        if (!ec) {
          application_context::add_connection(
              "project" + std::to_string(project_id), connection);
#ifdef LOGGING
          std::cout << "Subscribing to project<project_id>" << std::endl;
          std::cout << "Wrote 'task get_all' response: "
                    << connection->out_message().substr(0, len) << std::endl;
#endif
        }
      });
#ifdef LOGGING
  std::cout << "Get all tasks handler" << std::endl;
#endif
}

void create_task_handler(json &in_json,
                         TcpConnection::pointer & /*connection*/) {
  json out_json;
  auto taskDTO = in_json.get<RequestFormat<TaskCreateDTO>>().data;
  Task task = task_service::create_task(taskDTO);
  multicasting::do_multicast("project" + std::to_string(task.project_id), task);
#ifdef LOGGING
  std::cout << "Create new task" << std::endl;
#endif
}

void edit_task_handler(json &in_json, TcpConnection::pointer &connection) {
  json out_json;
  auto editDTO = in_json.get<RequestFormat<TaskEditDTO>>().data;
  out_json["task"] = task_service::edit_task(editDTO);
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
