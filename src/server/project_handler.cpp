//
// Created by egor on 03.05.2021.
//
#include <iostream>
#include <nlohmann/json.hpp>
#include "handler_registration.h"
#include "project_service.h"
#include "request_format.h"
#include "serialization.h"
#include "structures.h"
#include "tcp_connection.h"

void create_project_handler(nlohmann::json &in_json,
                            TcpConnection::pointer &connection) {
  nlohmann::json out_json;
  // in_json.get<RequestFormat<TaskCreateDTO>>();
  auto projectDTO = in_json.get<RequestFormat<ProjectCreateDTO>>();
  out_json["project"] = project_service::create_project(projectDTO.data);
  connection->out_message() = out_json.dump();
  connection->socket().async_write_some(
      asio::buffer(connection->out_message().data(),
                   connection->out_message().size()),
      [&, connection](asio::error_code const & /*ec*/, std::size_t len) {
#ifdef LOGGING
        std::cout << "Wrote 'project create' response: "
                  << connection->out_message().substr(0, len) << std::endl;
#endif
      });
#ifdef LOGGING
  std::cout << "Create new project" << std::endl;
#endif
}

REGISTER_HANDLER("project create", create_project_handler);
