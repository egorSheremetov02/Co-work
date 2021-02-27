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

asio::io_service service;
tcp::socket socket1(service);
std::string auth_response;

int main() {

    const asio::ip::basic_endpoint<tcp> &endpoint = tcp::endpoint(asio::ip::address::from_string("127.0.0.1"),
                                                                  3030);
    {
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
                                         auth_response.resize(1024);
                                         socket1.async_read_some(
                                                 asio::buffer(auth_response.data(), auth_response.size()),
                                                 [&](asio::error_code const &ec, std::size_t len) {
                                                     std::cout << "Successfully read data from server: "
                                                               << auth_response << " of size " << len
                                                               << std::endl;
                                                     json json_auth_response = json::parse(
                                                             auth_response.substr(0, len));
                                                     auto response = json_auth_response.get<ResponseFormat<User>>();
                                                     if (response.error.empty()) {
                                                         std::cout << "Successfully authenticated to server"
                                                                   << std::endl;
                                                         std::string resource, data;
                                                         std::cin >> resource >> data;
                                                         RequestFormat<User> test_request = {
                                                                 "projects get {id}",
                                                                 {}
                                                         };
                                                         json json_project_request = test_request;
                                                         std::string res = json_project_request.dump();
                                                         socket1.async_write_some(
                                                                 asio::buffer(res.data(), res.size()),
                                                                 [&](asio::error_code const &ec,
                                                                     std::size_t) {
                                                                     if (ec) return;
                                                                     std::cout
                                                                             << "Successfully wrote data to stream"
                                                                             << std::endl;
                                                                 });
                                                     } else {
                                                         std::cout
                                                                 << "Unauthorized auth_request to authorized resource"
                                                                 << std::endl;
                                                     }
                                                 });
                                     });
        });
    }

    service.run();
}

