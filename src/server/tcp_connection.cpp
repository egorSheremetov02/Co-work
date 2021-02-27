//
// Created by egor on 27.02.2021.
//
#include <iostream>
#include <string>
#include <nlohmann/json.hpp>
#include "tcp_connection.h"
#include "controller.h"
#include "auth_service.h"
#include "src/shared/structures.h"
#include "src/shared/request.h"
#include "src/shared/request_format.h"
#include "src/shared/serialization.h"

using nlohmann::json;

TcpConnection::TcpConnection(asio::io_context &io_context, std::size_t max_in_message_size,
                             std::size_t max_out_message_size)
        : socket_(io_context) {
    in_message_.resize(max_in_message_size);
    out_message_.resize(max_out_message_size);
}


void TcpConnection::do_read() {
    socket_.async_read_some(
            asio::buffer(in_message_.data(), in_message_.size()),
            [&, this, connection = shared_from_this()](asio::error_code const &ec, std::size_t len) mutable {
                if (ec) {
                    connection->socket().close();
                    std::cout << "Async read error: " << ec.message() << std::endl;
                    return;
                }
                const std::string &str_request = in_message().substr(0, len);
                json json_request = json::parse(str_request);
                std::string resource = json_request.at("resource");
                std::cout << "Resource: " << resource << std::endl;
                Handler handler = ApplicationController::get_handler(resource);
                handler(json_request);
                do_read();
            });
}

void TcpConnection::handle_write() {}

TcpConnection::pointer
TcpConnection::create(asio::io_context &io_context, std::size_t max_in_message_size, std::size_t max_out_message_size) {
    return TcpConnection::pointer(new TcpConnection(io_context, max_in_message_size, max_out_message_size));
}

tcp::socket &TcpConnection::socket() {
    return socket_;
}

std::string &TcpConnection::in_message() {
    return in_message_;
}

std::string &TcpConnection::out_message() {
    return out_message_;
}

void authentication_handler(std::size_t len, TcpConnection::pointer &connection) {

    std::string auth_res;

    AuthService auth;
    std::string auth_request_str = connection->in_message().substr(0, len);
    auto auth_request = json::parse(auth_request_str).get<RequestFormat<AuthReqDTO>>();

    ResponseFormat<User> auth_response;

    std::optional<User> opt_user = auth.validate(auth_request.data);
    if (!opt_user.has_value()) {
        std::cout << "Invalid auth token: " << auth_request_str << std::endl;
        auth_response.error = "No user with such credentials was found";
    } else {
        std::cout << "Successfully authenticated" << std::endl;
        auth_response.data = opt_user.value();
    }
    write_auth_response(auth_response, connection);
}

void authenticate(TcpConnection::pointer &connection) {
    connection->socket().async_read_some(
            asio::buffer(connection->in_message().data(), connection->in_message().size()),
            [&, connection](asio::error_code const &ec, std::size_t len) mutable {
                if (ec) {
                    std::cout << "Error while reading client" << std::endl;
                    std::cout << ec.message() << std::endl;
                    connection->socket().close();
                    return;
                }
                authentication_handler(len, connection);
            });
}

void write_auth_response(ResponseFormat<User> const &auth_response, TcpConnection::pointer &connection) {
    json j = auth_response;
    connection->out_message() = j.dump();
    connection->socket().async_write_some(
            asio::buffer(connection->out_message().data(), connection->out_message().size()),
            [&, connection, auth_response](asio::error_code const &ec,
                                           std::size_t len) mutable {
                if (ec) {
                    std::cout << "Error while writing to client" << std::endl;
                    return;
                }
                std::cout << "Wrote data to client: " << auth_response.data.account_name << std::endl;
                if (auth_response.error.empty()) {
                    connection->do_read();
                } else {
                    connection->socket().close();
                }
            });
}

