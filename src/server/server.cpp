//
// Created by egor on 13.02.2021.
//

#include "server.hpp"
#include <memory>
#include <utility>

using asio::ip::tcp;


TcpConnection::pointer TcpConnection::create(asio::io_context &io_context, std::size_t max_message_size) {
    return TcpConnection::pointer(new TcpConnection(io_context, max_message_size));
}

tcp::socket &TcpConnection::socket() {
    return socket_;
}

std::string &TcpConnection::message() {
    return message_;
}


void ConnectionAcceptor::do_accept() {
    TcpConnection::pointer new_connection = TcpConnection::create(service_);
    acceptor_.async_accept(new_connection->socket(), [&, connection = new_connection](asio::error_code const &ec) {
        handle_accept(connection);
    });
}

void ConnectionAcceptor::handle_accept(TcpConnection::pointer connection) {
    std::cout << "Accepted connection" << std::endl;
    do_accept();
    // Here we gonna authenticate our user...
    authenticate(connection);
}


void TcpConnection::do_read() {
    socket_.async_read_some(
            asio::buffer(message_.data(), message_.size()),
            [&, this, connection = shared_from_this()](asio::error_code const &ec, std::size_t len) mutable {
                if (ec) {
                    connection->socket().close();
                    std::cout << "Async read error: " << ec.message() << std::endl;
                    return;
                }

                std::stringstream ss(message().substr(0, len));
                std::string resource;
                std::string data;
                ss >> resource >> data;
                std::cout << "Resource: " << resource << ", Data: " << data << std::endl;
                std::cout << std::endl;
                do_read();
            });
}

bool AuthService::validate(const std::string &payload) const {
    std::string auth_token = "password";
    if (payload.size() != auth_token.size()) {
        return false;
    }
    return payload == auth_token;
}

void authenticate(TcpConnection::pointer &connection) {
    connection->socket().async_read_some(
            asio::buffer(connection->message().data(), connection->message().size()),
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

void authentication_handler(std::size_t len, TcpConnection::pointer &connection) {

    std::string auth_res;

    AuthService auth;
    std::string substr = connection->message().substr(0, len);

    if (!auth.validate(substr)) {
        std::cout << "Invalid auth token: " << substr << std::endl;
        auth_res = "F";
    } else {
        std::cout << "Successfully authenticated" << std::endl;
        auth_res = "S";
    }

    write_auth_response(auth_res, connection);
}

void write_auth_response(AuthResponse const &auth_res, TcpConnection::pointer &connection) {
    connection->socket().async_write_some(
            asio::buffer(auth_res.data(), auth_res.size()),
            [&, connection, auth_res](asio::error_code const &ec,
                                      std::size_t len) mutable {
                if (ec) {
                    std::cout << "Error while writing to client" << std::endl;
                    return;
                }
                std::cout << "Wrote data to client: " << auth_res << std::endl;
                if (auth_res == "S") {
                    connection->do_read();
                } else {
                    connection->socket().close();
                }
            });
}


ConnectionAcceptor::ConnectionAcceptor(asio::io_service &service) : service_(service), acceptor_(
        service, tcp::endpoint(tcp::v4(), 3030)) {
    do_accept();
}

namespace ApplicationContext {

    MulticastClients &get_multicast_clients() {
        static auto *multicast_clients = new MulticastClients();
        return *multicast_clients;
    }

    void check_resource_existence(std::string const &resource_id) {
        if (get_multicast_clients().find(resource_id) == get_multicast_clients().end()) {
            throw std::invalid_argument("Can't delete resource with id: " + resource_id);
        }
    }

    void check_connection_existence(std::string const &resource_id, TcpConnection::pointer &connection) {
        check_resource_existence(resource_id);
        if (get_multicast_clients()[resource_id].find(connection) == get_multicast_clients()[resource_id].end()) {
            throw std::invalid_argument("Can't delete connection associated with resource with id: " + resource_id);
        }
    }

    ClientsList &get_connections(std::string const &resource_id) {
        return get_multicast_clients()[resource_id];
    }

    void add_connection(std::string const &resource_id, const TcpConnection::pointer &connection) {
        get_multicast_clients()[resource_id].insert(connection);
    }

    void remove_connection(std::string const &resource_id, TcpConnection::pointer &connection) {
        check_connection_existence(resource_id, connection);
        ClientsList &subscribers = get_multicast_clients()[resource_id];
        subscribers.erase(connection);
        if (subscribers.empty()) {
            get_multicast_clients().erase(resource_id);
        }
    }
}
