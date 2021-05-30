//
// Created by egor on 06.03.2021.
//
#include <iostream>
#include <nlohmann/json.hpp>
#include <string>
#include "application_context.hpp"
#include "globals.h"
#include "handler_registration.hpp"
#include "multicast.h"
#include "serialization.h"
#include "task_service.hpp"
#include "util.h"

using nlohmann::json;

void get_all_tasks_handler(json &in_json, TcpConnection::pointer &connection) {
  auto tasksDTO = in_json.get<RequestFormat<TaskGetAllDTO>>().data;
  auto project_id = tasksDTO.project_id;
  json out_json;
  ResponseFormat<std::vector<Task>> tasksResponse;
  tasksResponse.data = task_service::get_tasks(tasksDTO);
  tasksResponse.metadata = defaults::server::get_all_tasks_metadata;
  // out_json = task_service::get_tasks(tasksDTO);
  out_json = tasksResponse;
  connection->out_message() = out_json.dump();
  connection->socket().async_write_some(
      asio::buffer(connection->out_message().data(),
                   connection->out_message().size()),
      [&, connection, project_id](asio::error_code const &ec,
                                  [[maybe_unused]] std::size_t len) {
        if (!ec) {
          application_context::add_connection(
              util::project_subscription_name(project_id), connection);
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

void create_task_handler(json &in_json, TcpConnection::pointer &connection) {
  json out_json;
  auto taskDTO = in_json.get<RequestFormat<TaskCreateDTO>>().data;
  Task task = task_service::create_task(taskDTO);
  application_context::add_connection(
      util::project_subscription_name(taskDTO.project_id), connection);
  multicasting::do_multicast(util::project_subscription_name(task.project_id),
                             task, defaults::server::create_task_metadata);
#ifdef LOGGING
  std::cout << "Create new task" << std::endl;
#endif
}

void edit_task_handler(json &in_json, TcpConnection::pointer &connection) {
  json out_json;
  auto editDTO = in_json.get<RequestFormat<TaskEditDTO>>().data;
  try {
    Task edited_task = task_service::edit_task(editDTO);
    application_context::add_connection(
        util::project_subscription_name(edited_task.project_id), connection);
    multicasting::do_multicast(
        util::project_subscription_name(edited_task.project_id), edited_task,
        defaults::server::edit_task_metadata);
  } catch (std::exception &exception) {
    ResponseFormat<Task> response;
    response.metadata = defaults::server::edit_task_metadata;
    response.error = exception.what();
    out_json = response;
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
  }

#ifdef LOGGING
  std::cout << "Edit task" << std::endl;
#endif
}

REGISTER_HANDLER("task edit", edit_task_handler);

REGISTER_HANDLER("task get_all", get_all_tasks_handler);

REGISTER_HANDLER("task create", create_task_handler);
