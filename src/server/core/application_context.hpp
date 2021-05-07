//
// Created by egor on 27.02.2021.
//

#ifndef CO_WORK_APPLICATION_CONTEXT_HPP
#define CO_WORK_APPLICATION_CONTEXT_HPP

#include <iostream>
#include <nlohmann/json.hpp>
#include <set>
#include <unordered_map>
#include "tcp_connection.hpp"

namespace application_context {

using ClientsList = std::set<TcpConnection::pointer>;
using MulticastClients = std::unordered_map<std::string, ClientsList>;

MulticastClients &get_multicast_clients();

void check_resource_existence(std::string const &resource_id);

void check_connection_existence(std::string const &resource_id,
                                TcpConnection::pointer &connection);

ClientsList &get_connections(std::string const &resource_id);

void add_connection(std::string const &resource_id,
                    const TcpConnection::pointer &connection);

void remove_connection(std::string const &resource_id,
                       TcpConnection::pointer connection);

template <typename T>
void multicast(std::string const &resource_id, T const &data) {
  using nlohmann::json;
  ResponseFormat<T> data_to_share;
  data_to_share.data = std::move(data);
  json json_message = data_to_share;
  std::shared_ptr<std::string> str_message(
      new std::string(json_message.dump()));
  for (const auto &connection : get_connections(resource_id)) {
    connection->socket().template async_write_some(
        asio::buffer(str_message->data(), str_message->size()),
        [str_message, connection
#ifdef LOGGING
         ,
         resource = resource_id
#endif
    ](asio::error_code const &ec, std::size_t len) {
          if (ec) {
            // handle error
            return;
          }
#ifdef LOGGING
          std::cout << "Subscription " << resource << " invocation: " << len
                    << std::endl;
#endif
        });
  }
}
}  // namespace application_context
#endif  // CO_WORK_APPLICATION_CONTEXT_HPP
