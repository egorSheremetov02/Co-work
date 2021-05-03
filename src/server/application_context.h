//
// Created by egor on 27.02.2021.
//

#ifndef CO_WORK_APPLICATION_CONTEXT_H
#define CO_WORK_APPLICATION_CONTEXT_H

#include <set>
#include <unordered_map>
#include "tcp_connection.h"

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
                       TcpConnection::pointer &connection);

template <typename T>
void multicast(std::string const &resource_id, T const &data);
}  // namespace application_context
#endif  // CO_WORK_APPLICATION_CONTEXT_H
