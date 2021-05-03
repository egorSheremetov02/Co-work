//
// Created by egor on 27.02.2021.
//
#include "application_context.h"

namespace application_context {

MulticastClients &get_multicast_clients() {
  static auto *multicast_clients = new MulticastClients();
  return *multicast_clients;
}

void check_resource_existence(std::string const &resource_id) {
  if (get_multicast_clients().find(resource_id) ==
      get_multicast_clients().end()) {
    throw std::invalid_argument("Can't delete resource with id: " +
                                resource_id);
  }
}

void check_connection_existence(std::string const &resource_id,
                                TcpConnection::pointer &connection) {
  check_resource_existence(resource_id);
  if (get_multicast_clients()[resource_id].find(connection) ==
      get_multicast_clients()[resource_id].end()) {
    throw std::invalid_argument(
        "Can't delete connection associated with resource with id: " +
        resource_id);
  }
}

ClientsList &get_connections(std::string const &resource_id) {
  return get_multicast_clients()[resource_id];
}

void add_connection(std::string const &resource_id,
                    const TcpConnection::pointer &connection) {
  get_multicast_clients()[resource_id].insert(connection);
}

void remove_connection(std::string const &resource_id,
                       TcpConnection::pointer &connection) {
  check_connection_existence(resource_id, connection);
  ClientsList &subscribers = get_multicast_clients()[resource_id];
  subscribers.erase(connection);
  if (subscribers.empty()) {
    get_multicast_clients().erase(resource_id);
  }
}

template <typename T>
void multicast(std::string const &resource_id, T const &data) {}
}  // namespace application_context
