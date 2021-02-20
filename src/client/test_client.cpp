//
// Created by egor on 12.02.2021.
//
#include <iostream>
#include <asio.hpp>
#include "../shared/response_format.h"
#include "../shared/request_format.h"
#include "../../include/structures.h"

using asio::ip::tcp;

asio::io_service service;
tcp::socket socket1(service);
std::string auth_response;

int main() {

    const asio::ip::basic_endpoint<tcp> &endpoint = tcp::endpoint(asio::ip::address::from_string("127.0.0.1"),
                                                                  3030);
    socket1.async_connect(endpoint, [&](asio::error_code const &ec) {
        std::cout << "Successfully connected" << std::endl;
        std::string passcode;
        std::cin >> passcode;
        RequestFormat<User> request;
        request.resource = "authentication";
        User u{1, "admin", "Egor Suvorov", roles::ADMIN};
        request.data = u;
        socket1.async_write_some(asio::buffer(passcode.data(), passcode.size()),
                                 [&](asio::error_code const &ec, std::size_t) {
                                     if (ec) return;
                                     auth_response.resize(20);
                                     socket1.async_read_some(asio::buffer(auth_response.data(), auth_response.size()),
                                                             [&](asio::error_code const &ec, std::size_t len) {
                                                                 std::cout << "Successfully read data from server: "
                                                                           << auth_response << " of size " << len
                                                                           << std::endl;
                                                                 if (auth_response.substr(0, len) == "S") {
                                                                     std::cout << "Successfully authenticated to server"
                                                                               << std::endl;
                                                                     std::string resource, data;
                                                                     std::cin >> resource >> data;
                                                                     std::string res = resource + " " + data;
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
                                                                             << "Unauthorized request to authorized resource"
                                                                             << std::endl;
                                                                 }
                                                             });
                                 });
    });

    service.run();
}

