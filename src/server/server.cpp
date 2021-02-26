//
// Created by egor on 13.02.2021.
//

#include "server.hpp"
#include <memory>
#include <utility>
#include <iomanip>
#include <nlohmann/json.hpp>
#include "db.h"
#include "src/shared/request_format.h"
#include "src/shared/response_format.h"
#include "src/shared/request.h"
#include "src/shared/response.h"
#include "src/shared/serialization.h"

using nlohmann::json;
using asio::ip::tcp;

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
            // TODO -- middleware layer
            asio::buffer(in_message_.data(), in_message_.size()),
            [&, this, connection = shared_from_this()](asio::error_code const &ec, std::size_t len) mutable {
                if (ec) {
                    connection->socket().close();
                    std::cout << "Async read error: " << ec.message() << std::endl;
                    return;
                }
            });
}

std::optional<User> AuthService::validate(const AuthReqDTO &payload) const {
    if (auth(payload.login, payload.password)) {
        User u;
        u.account_name = "egor.suvorov";
        u.role_in_system = ADMIN;
        u.full_name = "Egor Suvorov";
        u.id = 1;
        return std::optional<User>{u};
    }
    return std::optional<User>{};
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


ConnectionAcceptor::ConnectionAcceptor(asio::io_service &service) : service_(service), acceptor_(
        service, tcp::endpoint(tcp::v4(), 3030)) {
    do_accept();
}

namespace application_context {

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
