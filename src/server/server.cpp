//
// Created by egor on 13.02.2021.
//

#include "server.hpp"
#include <memory>
#include <utility>
#include <iomanip>

using nlohmann::json;
using asio::ip::tcp;

void ConnectionAcceptor::do_accept() {
    TcpConnection::pointer new_connection = TcpConnection::create(service_);
    acceptor_.async_accept(new_connection->socket(), [&, connection = new_connection](asio::error_code const &ec) {
        handle_accept(connection);
    });
}

void ConnectionAcceptor::handle_accept(TcpConnection::pointer connection) {
#ifdef LOGGING
    std::cout << "Accepted connection" << std::endl;
#endif
    do_accept();
    // Here we gonna authenticate our user...
    authenticate(connection);
}

ConnectionAcceptor::ConnectionAcceptor(asio::io_service &service) : service_(service), acceptor_(
        service, tcp::endpoint(tcp::v4(), 3030)) {
    do_accept();
}
