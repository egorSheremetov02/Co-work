//
// Created by egor on 03.05.2021.
//
#include <iostream>
#include <nlohmann/json.hpp>
#include "globals.h"
#include "project_service.hpp"
#include "request_format.h"
#include "serialization.h"
#include "src/server/core/handler_registration.hpp"
#include "structures.h"
#include "tcp_connection.hpp"

void create_project_handler(nlohmann::json &in_json,
                            TcpConnection::pointer &connection) {
  nlohmann::json out_json;
  // in_json.get<RequestFormat<TaskCreateDTO>>();
  auto projectDTO = in_json.get<RequestFormat<ProjectCreateDTO>>();
  ResponseFormat<Project> project_create_DTO;
  project_create_DTO.data = project_service::create_project(projectDTO.data);
  project_create_DTO.metadata = defaults::server::create_project_metadata;
  //  out_json["project"] = project_service::create_project(projectDTO.data);
  out_json = project_create_DTO;
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

REGISTER_HANDLER(endpoints::projects::create, create_project_handler);
