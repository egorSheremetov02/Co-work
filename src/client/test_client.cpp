//
// Created by egor on 12.02.2021.
//
#include <iostream>
#include <asio.hpp>
#include <nlohmann/json.hpp>
#include "src/shared/serialization.h"
#include "src/shared/response_format.h"
#include "src/shared/request_format.h"
#include "src/shared/structures.h"
#include "src/shared/request.h"

using asio::ip::tcp;
using nlohmann::json;

std::vector<std::string> resources;
asio::io_service service;
tcp::socket socket1(service);
std::string response;

struct InitMap {
    InitMap() {
        if (resources.empty()) {
            resources.emplace_back("task get_all");
            resources.emplace_back("task create");
            resources.emplace_back("task edit");
        }
    }
};

void do_write();

void do_read() {
    response.resize(1024);
    socket1.async_read_some(asio::buffer(response.data(), response.size()),
                            [&](asio::error_code const &ec, std::size_t len) {
                                std::cout << response.substr(0, len) << std::endl;
                                do_write();
                            });
}

void do_write() {
    std::cout << "Choose resource you want to connect to:" << std::endl;
    for (int i = 0; i < resources.size(); ++i) {
        std::cout << '\t' << i << ' ' << resources[i] << std::endl;
    }
    int i = 0;
    std::cin >> i;

    if (resources[i] == "task create") {
        RequestFormat<TaskCreateDTO> task_create_request = {
                resources[i],
                {"test task " + std::to_string(rand()),
                 "task description " + std::to_string(rand()),
                 9,
                 "in progress"}
        };
        json json_project_request = task_create_request;
        std::cout << json_project_request.dump(4) << std::endl;
        std::string res = json_project_request.dump();
        socket1.async_write_some(
                asio::buffer(res.data(), res.size()),
                [&](asio::error_code const &ec,
                    std::size_t) {
                    if (ec) {
                        std::cout << ec.message() << std::endl;
                    } else {
                        std::cout
                                << "Successfully wrote data to stream"
                                << std::endl;
                        do_read();
                    }
                });
    } else if (resources[i] == "task get_all") {
        RequestFormat<int> task_get_all_request = {
                resources[i]
        };
        json json_project_request = task_get_all_request;
        std::cout << json_project_request.dump(4) << std::endl;
        std::string res = json_project_request.dump();
        socket1.async_write_some(
                asio::buffer(res.data(), res.size()),
                [&](asio::error_code const &ec,
                    std::size_t) {
                    if (ec) {
                        std::cout << ec.message() << std::endl;
                    } else {
                        std::cout
                                << "Successfully wrote data to stream"
                                << std::endl;
                        do_read();
                    }
                });
    } else if (resources[i] == "task edit") {
        json json_project_request;
        json_project_request["resource"] = "task edit";
        json_project_request["task_id"] = 1;
        std::string res = json_project_request.dump();
        socket1.async_write_some(
                asio::buffer(res.data(), res.size()),
                [&](asio::error_code const &ec,
                    std::size_t) {
                    if (ec) {
                        std::cout << ec.message() << std::endl;
                    } else {
                        std::cout
                                << "Successfully wrote data to stream"
                                << std::endl;
                        do_read();
                    }
                });
    } else {
        assert(false);
    }
}

void authenticate() {
    const asio::ip::basic_endpoint<tcp> &endpoint = tcp::endpoint(asio::ip::address::from_string("127.0.0.1"),
                                                                  3030);

    socket1.async_connect(endpoint, [&](asio::error_code const &ec) {
        std::cout << "Successfully connected" << std::endl;
        std::string passcode;
        std::string login;
        std::cout << "Enter login: ";
        std::cin >> login;
        std::cout << "Enter password: ";
        std::cin >> passcode;
        RequestFormat<AuthReqDTO> auth_request;
        auth_request.resource = "authentication";
        auth_request.data = {login, passcode};
        json j = auth_request;
        std::string str_auth_request = j.dump();
        socket1.async_write_some(asio::buffer(str_auth_request.data(), str_auth_request.size()),
                                 [&](asio::error_code const &ec, std::size_t) {
                                     if (ec) return;
                                     response.resize(1024);
                                     socket1.async_read_some(
                                             asio::buffer(response.data(), response.size()),
                                             [&](asio::error_code const &ec, std::size_t len) {
                                                 std::cout << "Successfully read data from server: "
                                                           << response << " of size " << len
                                                           << std::endl;
                                                 json json_auth_response = json::parse(
                                                         response.substr(0, len));
                                                 auto response = json_auth_response.get<ResponseFormat<User>>();
                                                 if (response.error.empty()) {
                                                     std::cout << "Successfully authenticated to server"
                                                               << std::endl;
                                                     do_write();
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

