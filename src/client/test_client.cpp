//
// Created by egor on 12.02.2021.
//
#include <iostream>
#include <asio.hpp>

using asio::ip::tcp;


asio::io_service service;
tcp::socket socket1(service);

std::array<char, 1024> buf;

void read_handler(const asio::error_code &ec, std::size_t len) {
    if (ec) {
        return;
    }

    for (int i = 0; i < len; ++i) {
        std::cout << buf[i];
    }
    std::cout << std::endl;

    socket1.async_read_some(asio::buffer(buf, 1024), [&](const asio::error_code &ec, std::size_t len) {
        read_handler(ec, len);
    });
}

int main() {

    const asio::ip::basic_endpoint<tcp> &endpoint = tcp::endpoint(asio::ip::address::from_string("127.0.0.1"),
                                                                  3030);
    socket1.async_connect(endpoint, [&](const asio::error_code &ec) {
        socket1.async_read_some(asio::buffer(buf, 1024), [&](const asio::error_code &ec, std::size_t len) {
            read_handler(ec, len);
        });
    });

    service.run();
}

