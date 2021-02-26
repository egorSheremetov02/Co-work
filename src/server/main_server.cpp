//
// Created by egor on 12.02.2021.
//
#include <ctime>
#include <iostream>
#include <string>
#include <memory>
#include <asio.hpp>

using asio::ip::tcp;


std::string make_daytime_string() {
    using namespace std;
    time_t now = time(nullptr);
    return ctime(&now);
}


class TcpConnection : public std::enable_shared_from_this<TcpConnection> {
public:
    typedef std::shared_ptr<TcpConnection> pointer;

    static pointer create(asio::io_context &io_context) {
        return pointer(new TcpConnection(io_context));
    }

    tcp::socket &socket() {
        return socket_;
    }

    void start() {
        in_message_ = make_daytime_string();

        asio::async_write(socket_, asio::buffer(in_message_),
                /*[](const asio::error_code & ec, std::size_t len) {} */
                          std::bind(&TcpConnection::handle_write, shared_from_this()));
    }

private:
    explicit TcpConnection(asio::io_context &io_context)
            : socket_(io_context) {
    }

    void handle_write() {}

    tcp::socket socket_;
    std::string in_message_;
};


class tcp_server {
public:
    explicit tcp_server(asio::io_context &io_context)
            : io_context_(io_context),
              acceptor_(io_context, tcp::endpoint(tcp::v4(), 3030)),
              timer_(io_context, asio::chrono::seconds(5)) {
        start_accept();
    }

private:
    void start_accept() {
        TcpConnection::pointer new_connection = TcpConnection::create(io_context_);

        acceptor_.async_accept(new_connection->socket(),
                               std::bind(&tcp_server::handle_accept, this, new_connection,
                                         std::placeholders::_1));
    }

    void handle_accept(const TcpConnection::pointer &new_connection,
                       const asio::error_code &error) {

        ++counter_;

        std::cout << counter_ << ". Before timer" << std::endl;

        timer_.async_wait(std::bind(&tcp_server::handle_wait, this, error, new_connection));
    }

    void handle_wait(asio::error_code &ec, const TcpConnection::pointer &new_connection) {
        if (!ec) {
            new_connection->start();
        }

        std::cout << "Inside timer" << std::endl;

        timer_.expires_at(timer_.expiry() + asio::chrono::seconds(5));
        start_accept();

    }

    asio::io_context &io_context_;
    tcp::acceptor acceptor_;
    asio::steady_timer timer_;

    int counter_ = 0;
};


int main() {
    try {
        asio::io_context io_context;
        tcp_server server(io_context);
        io_context.run();
    }
    catch (std::exception &e) {
        std::cerr << e.what() << std::endl;
    }

    return 0;
}
