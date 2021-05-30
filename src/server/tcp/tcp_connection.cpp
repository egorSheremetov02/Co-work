//
// Created by egor on 27.02.2021.
//
#include "tcp_connection.hpp"
#include <iostream>
#include <nlohmann/json.hpp>
#include <string>
#include "application_context.hpp"
#include "auth_service.hpp"
#include "controller.hpp"
#include "request.h"
#include "request_format.h"
#include "serialization.h"
#include "structures.h"

// int counter = 0;

using nlohmann::json, asio::ip::tcp;

TcpConnection::TcpConnection(asio::io_context &io_context,
                             std::size_t max_in_message_size,
                             std::size_t max_out_message_size)
    : cxt(io_context), socket_(io_context) {
  in_message_.resize(max_in_message_size);
  out_message_.resize(max_out_message_size);
}

void TcpConnection::do_read() {
  socket_.async_read_some(
      asio::buffer(in_message_.data(), in_message_.size()),
      /*[&, this, connection = shared_from_this()]*/ [&, this,
                                                      connection = this](
                                                         asio::error_code const
                                                             &ec,
                                                         std::size_t
                                                             len) mutable {
        if (ec) {
          connection->socket().close();
#ifdef LOGGING
          std::cout << "Async read error: " << ec.message() << std::endl;
#endif
          delete connection;
          return;
        }
        const std::string &str_request = in_message().substr(0, len);
        json json_request = json::parse(str_request);
        std::string resource = json_request.at("resource");
#ifdef LOGGING
        std::cout << "Resource: " << resource << std::endl;
#endif
        ApplicationController::get_handler(resource)(json_request, connection);
        do_read();
      });
}

TcpConnection::pointer TcpConnection::create(asio::io_context &io_context,
                                             std::size_t max_in_message_size,
                                             std::size_t max_out_message_size) {
  return TcpConnection::pointer(
      new TcpConnection(io_context, max_in_message_size, max_out_message_size));
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

void authentication_handler(std::size_t len,
                            TcpConnection::pointer &connection) {
  std::string auth_res;

  AuthService auth;
  std::string auth_request_str = connection->in_message().substr(0, len);
  auto auth_request =
      json::parse(auth_request_str).get<RequestFormat<AuthReqDTO>>();

  ResponseFormat<User> auth_response;

  std::optional<User> opt_user = auth.validate(auth_request.data);
  if (!opt_user) {
#ifdef LOGGING
    std::cout << "Invalid user credentials: " << auth_request_str << std::endl;
#endif
    auth_response.error = "No user with such credentials was found";
  } else {
#ifdef LOGGING
    std::cout << "Successfully authenticated" << std::endl;
#endif
    auth_response.data = opt_user.value();
  }
  write_auth_response(auth_response, connection);
  //  heartbeat(connection);
}

void authenticate(TcpConnection::pointer &connection) {
  connection->socket().async_read_some(
      asio::buffer(connection->in_message().data(),
                   connection->in_message().size()),
      [&, connection](asio::error_code const &ec, std::size_t len) mutable {
        if (ec) {
#ifdef LOGGING
          std::cout << "Error while reading client" << std::endl;
          std::cout << ec.message() << std::endl;
#endif
          connection->socket().close();
          return;
        }
        authentication_handler(len, connection);
      });
}

void write_auth_response(ResponseFormat<User> const &auth_response,
                         TcpConnection::pointer &connection) {
  json j = auth_response;
  connection->out_message() = j.dump();
  connection->socket().async_write_some(
      asio::buffer(connection->out_message().data(),
                   connection->out_message().size()),
      [&, connection, auth_response](asio::error_code const &ec,
                                     std::size_t /*len*/) mutable {
        if (ec) {
#ifdef LOGGING
          std::cout << "Error while writing to client" << std::endl;
#endif
          return;
        }
#ifdef LOGGING
        std::cout << "Wrote data to client: " << auth_response.data.account_name
                  << std::endl;
#endif
        if (auth_response.error.empty()) {
#ifdef LOGGING
          std::cout << "Attempt to read client" << std::endl;
#endif
          connection->do_read();
        } else {
          connection->socket().close();
        }
      });
}

// every 5 seconds checks if client is alive
void heartbeat(TcpConnection::pointer &connection) {
  std::shared_ptr<asio::steady_timer> t(
      new asio::steady_timer(connection->cxt, asio::chrono::seconds(5)));
  //  asio::steady_timer t(connection->cxt, asio::chrono::seconds(5));
  t->async_wait([&, t]([[maybe_unused]] asio::error_code const &ec) {
    auto *response = new ResponseFormat<std::string>();
    response->metadata = "heartbeat";
    connection->socket().async_write_some(
        asio::buffer(response->data.data(), response->data.size()),
        [&](asio::error_code const &ec, [[maybe_unused]] std::size_t len) {
          std::cout << "Heartbeat start" << std::endl;
          delete response;
          if (ec) {
            connection->socket().close();
            delete connection;
          }
          std::cout << "Heartbeat finished" << std::endl;
          heartbeat(connection);
        });
  });
}

void TcpConnection::add_subscription(std::string const &resource_id) {
  subscriptions_.insert(resource_id);
}

TcpConnection::~TcpConnection() {
  for (auto const &resource_id : subscriptions_) {
    //    application_context::remove_connection(resource_id,
    //    shared_from_this());
    application_context::remove_connection(resource_id, this);
  }
}
