//
// Created by egor on 13.02.2021.
//
#ifndef CO_WORK_SERVER_HPP
#define CO_WORK_SERVER_HPP

#include <deque>
#include <iostream>
#include <string>
#include <unordered_map>
#include <set>
#include <exception>
#include <nlohmann/json.hpp>
#include <memory>
#include <asio.hpp>
#include "../shared/response_format.h"
#include "../shared/request_format.h"
#include "../shared/request.h"
#include "../shared/response.h"
#include "src/shared/structures.h"

using asio::ip::tcp;
using nlohmann::json;

using AuthRequest = RequestFormat<AuthReqDTO>;

using Handler = std::function<void(json &)>;

struct AuthService {
    [[nodiscard]] std::optional<User> validate(const AuthReqDTO &payload) const;
};


struct TcpConnection : public std::enable_shared_from_this<TcpConnection> {
public:
    using pointer = std::shared_ptr<TcpConnection>;

    static pointer create(asio::io_context &io_context, std::size_t max_in_message_size = 1024, std::size_t max_out_message_size = 1024);

    void do_read();

    tcp::socket &socket();

    std::string &in_message();

    std::string &out_message();

private:
    explicit TcpConnection(asio::io_context &io_context, std::size_t max_in_message_size, std::size_t max_out_message_size)
            : socket_(io_context) {
        in_message_.resize(max_in_message_size);
        out_message_.resize(max_out_message_size);
    }

    void handle_write() {}

    tcp::socket socket_;
    std::string in_message_;
    std::string out_message_;
};


struct ConnectionAcceptor {
public:
    explicit ConnectionAcceptor(asio::io_service &service);

private:
    void do_accept();

    void handle_accept(TcpConnection::pointer connection);

    tcp::acceptor acceptor_;
    asio::io_service &service_;
};

void authenticate(TcpConnection::pointer &connection);

void authentication_handler(std::size_t len, TcpConnection::pointer &connection);

void write_auth_response(ResponseFormat<User> const &auth_response, TcpConnection::pointer &connection);

namespace application_context {

    using ClientsList = std::set<TcpConnection::pointer>;
    using MulticastClients = std::unordered_map<std::string, ClientsList>;

    MulticastClients &get_multicast_clients();

    void check_resource_existence(std::string const &resource_id);

    void check_connection_existence(std::string const &resource_id, TcpConnection::pointer &connection);

    ClientsList &get_connections(std::string const &resource_id);

    void add_connection(std::string const &resource_id, const TcpConnection::pointer &connection);

    void remove_connection(std::string const &resource_id, TcpConnection::pointer &connection);


    template<typename T>
    void multicast(std::string const &resource_id, T const &data) {

    }
}

struct ApplicationController {
    static Handler get_handler(std::string const & resource);
};

#endif //CO_WORK_SERVER_HPP
