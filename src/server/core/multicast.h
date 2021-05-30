//
// Created by egor on 07.05.2021.
//

#ifndef CO_WORK_MULTICAST_H
#define CO_WORK_MULTICAST_H
#include <string>
#include "application_context.hpp"
#include "tcp_connection.hpp"

namespace multicasting {
template <typename T>
void do_multicast(std::string const &resource_id,
                  T data,
                  std::string const &action) {
  using nlohmann::json;
  ResponseFormat<T> data_to_share;
  data_to_share.data = std::move(data);
  data_to_share.metadata = action;
  json json_message = data_to_share;
  std::cout << "start multicast" << std::endl;
  std::shared_ptr<std::string> str_message(
      new std::string(json_message.dump()));
  std::cout << application_context::get_connections(resource_id).size()
            << std::endl;
  for (const auto &connection :
       application_context::get_connections(resource_id)) {
    connection->socket().template async_write_some(
        asio::buffer(str_message->data(), str_message->size()),
        [str_message, connection
#ifdef LOGGING
         ,
         resource = resource_id
#endif
    ](asio::error_code const &ec, [[maybe_unused]] std::size_t len) {
          std::cout << "Handling resource" << std::endl;
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
}  // namespace multicasting

#endif  // CO_WORK_MULTICAST_H
