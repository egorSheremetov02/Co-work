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

ClientsList &get_connections(std::string const &resource_id);

void add_connection(std::string const &resource_id,
                    const TcpConnection::pointer &connection);

void remove_connection(std::string const &resource_id,
                       TcpConnection::pointer connection);
}  // namespace application_context
#endif  // CO_WORK_APPLICATION_CONTEXT_HPP
