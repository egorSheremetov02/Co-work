//
// Created by egor on 12.02.2021.
//
#include <asio.hpp>
#include <chrono>
#include <iostream>
#include <memory>
#include <nlohmann/json.hpp>
#include "src/shared/request.h"
#include "src/shared/request_format.h"
#include "src/shared/response_format.h"
#include "src/shared/serialization.h"
#include "src/shared/structures.h"

using asio::ip::tcp;
using nlohmann::json;

std::vector<std::string> resources;
asio::io_service service;
tcp::socket socket1(service);
std::string response;

int counter = 0;

struct InitMap {
  InitMap() {
    if (resources.empty()) {
      resources.emplace_back("task get_all");
      resources.emplace_back("task create");
      resources.emplace_back("task edit");
      resources.emplace_back("project create");
    }
  }
};

void do_write();

void do_write_nonblocking();

void do_read() {
  ++counter;
  response.resize(1024);
  socket1.async_read_some(
      asio::buffer(response.data(), response.size()),
      [&](asio::error_code const & /*ec*/, std::size_t len) {
        //        std::cout << response.substr(0, len) << std::endl;
        //        do_write();
        do_write_nonblocking();
      });
}

void do_write() {
  // Interactive version
  std::cout << "Choose resource you want to connect to:" << std::endl;
  for (int i = 0; i < resources.size(); ++i) {
    std::cout << '\t' << i << ' ' << resources[i] << std::endl;
  }
  int i = 0;
  std::cin >> i;

  if (resources[i] == "task create") {
    RequestFormat<TaskCreateDTO> task_create_request = {
        resources[i],
        {"test task " + std::to_string(random() % INT32_MAX),
         "task description " + std::to_string(random() % INT32_MAX), 9,
         "in progress", 1}};
    json json_project_request = task_create_request;
    std::shared_ptr<std::string> res(
        new std::string(json_project_request.dump()));
    socket1.async_write_some(
        asio::buffer(res->data(), res->size()),
        [&](asio::error_code const &ec, std::size_t) {
          if (ec) {
            std::cout << ec.message() << std::endl;
          } else {
            std::cout << "Successfully wrote data to stream" << std::endl;
            do_read();
          }
        });
  } else if (resources[i] == "task get_all") {
    RequestFormat<TaskGetAllDTO> task_get_all_request = {resources[i], {1}};
    json json_get_all_tasks_req = task_get_all_request;
    //    std::cout << json_get_all_tasks_req.dump(4) << std::endl;
    std::shared_ptr<std::string> res(
        new std::string(json_get_all_tasks_req.dump()));
    socket1.async_write_some(
        asio::buffer(res->data(), res->size()),
        [&](asio::error_code const &ec, std::size_t) {
          if (ec) {
            std::cout << ec.message() << std::endl;
          } else {
            std::cout << "Successfully wrote data to stream" << std::endl;
            do_read();
          }
        });
  } else if (resources[i] == "task edit") {
    json json_task_edit_req;
    json_task_edit_req["resource"] = "task edit";
    json_task_edit_req["task_id"] = 1;
    std::shared_ptr<std::string> res(
        new std::string(json_task_edit_req.dump()));
    socket1.async_write_some(
        asio::buffer(res->data(), res->size()),
        [res](asio::error_code const &ec, std::size_t) {
          if (ec) {
            std::cout << ec.message() << std::endl;
          } else {
            std::cout << "Successfully wrote data to stream" << std::endl;
            do_read();
          }
        });
  } else if (resources[i] == "project create") {
    RequestFormat<ProjectCreateDTO> project_create_request = {
        "project create", {"demo project", "01.09.2021"}};
    json json_project_create = project_create_request;
    std::shared_ptr<std::string> res(
        new std::string(json_project_create.dump()));
    socket1.async_write_some(
        asio::buffer(res->data(), res->size()),
        [res](asio::error_code const &ec, std::size_t) {
          if (ec) {
            std::cout << ec.message() << std::endl;
          } else {
            std::cout << "Successfully wrote data to stream" << std::endl;
            do_read();
          }
        });
  } else {
    assert(false);
  }
}

void do_subscribe() {
  RequestFormat<TaskGetAllDTO> task_get_all_request = {"task get_all", {1}};
  json json_get_all_tasks_req = task_get_all_request;
  std::shared_ptr<std::string> res(
      new std::string(json_get_all_tasks_req.dump()));
  socket1.async_write_some(asio::buffer(res->data(), res->size()),
                           [&, res](asio::error_code const &ec, std::size_t) {
                             if (ec) {
                               std::cout << ec.message() << std::endl;
                             } else {
                               //                               std::cout <<
                               //                               "Successfully
                               //                               wrote data to
                               //                               stream"
                               //                                         <<
                               //                                         std::endl;
                               do_read();
                             }
                           });
}

void do_write_nonblocking() {
  /*
  asio::steady_timer t(service, asio::chrono::seconds(2));
  t.async_wait([&](asio::error_code const &ec) {
    socket1.async_write_some(asio::buffer(response.data(), response.size()),
                             [&](asio::error_code const &ec, std::size_t
                             len) {
                               if (ec) {
                                 return;
                               }
                             });
  });
  socket1.write_some(asio::buffer(s));
  */
#ifdef FIRST_TEST
  RequestFormat<TaskCreateDTO> task_create_request = {
      "task create",
      {"test task " + std::to_string(random() % INT32_MAX),
       "task description " + std::to_string(random() % INT32_MAX), 9,
       "in progress", 1, "01.03.2022"}};
  json json_project_request = task_create_request;
  std::shared_ptr<std::string> res(
      new std::string(json_project_request.dump()));
  socket1.async_write_some(asio::buffer(res->data(), res->size()),
                           [&](asio::error_code const &ec, std::size_t) {
                             if (ec) {
                               std::cout << ec.message() << std::endl;
                             } else {
                               //          std::cout << "Successfully wrote data
                               //          to stream" << std::endl;
                               asio::steady_timer t(
                                   service, asio::chrono::milliseconds(300));
                               t.wait();
                               do_read();
                             }
                           });
#else
#ifdef STRESS_TEST
  if (counter < 1) {
#endif
    do_read();
#ifdef STRESS_TEST
  }
#endif
#endif
}

void authenticate() {
  const asio::ip::basic_endpoint<tcp> &endpoint =
      tcp::endpoint(asio::ip::address::from_string("127.0.0.1"), 3030);

  socket1.async_connect(endpoint, [&](asio::error_code const & /*ec*/) {
    //    std::cout << "Successfully connected" << std::endl;
    std::string passcode;
    std::string login;
#ifdef STRESS_TEST
    login = "admin";
#else
    std::cout << "Enter login: ";
    std::cin >> login;
#endif
#ifdef STRESS_TEST
    passcode = "qwerty";
#else
    std::cout << "Enter password: ";
    std::cin >> passcode;
#endif
    RequestFormat<AuthReqDTO> auth_request;
    auth_request.resource = "authentication";
    auth_request.data = {login, passcode};
    json j = auth_request;
    std::string str_auth_request = j.dump();
    socket1.async_write_some(
        asio::buffer(str_auth_request.data(), str_auth_request.size()),
        [&](asio::error_code const &ec, std::size_t) {
          if (ec) {
            return;
          }
          response.resize(10000);
          socket1.async_read_some(
              asio::buffer(response.data(), response.size()),
              [&](asio::error_code const & /*ec*/, std::size_t len) {
                //                std::cout << "Successfully read data from
                //                server: " << response
                //                          << " of size " << len << std::endl;
                json json_auth_response = json::parse(response.substr(0, len));
                auto response = json_auth_response.get<ResponseFormat<User>>();
                if (response.error.empty()) {
                  //                  std::cout << "Successfully authenticated
                  //                  to server"
                  //                            << std::endl;
                  //                  do_write();
                  //                  do_subscribe();
                  //                  std::cout << "Successfully authorized" <<
                  //                  std::endl;
                } else {
                  socket1.close();
                  std::cout
                      << "Unauthorized auth_request to authorized resource"
                      << std::endl;
                  authenticate();
                }
              });
        });
  });
}

int main() {
  [[maybe_unused]] InitMap init;
  authenticate();

  service.run();
}
