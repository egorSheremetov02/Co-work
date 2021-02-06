//
// Created by egor on 03.02.2021.
//
#include <iostream>
#include <asio.hpp>

using asio::ip::tcp;

std::vector<char> buf(128);

void handler(
        const asio::error_code &error, // Result of operation.
        std::size_t len           // Number of bytes read.
) {

    std::cout.write(buf.data(), len);

    buf.clear();
}

int main() {
    try {

        asio::io_context io_context;

        tcp::socket socket(io_context);

//        auto work = std::make_shared<asio::io_context::work>(io_context);
//
        io_context.run();


        for (int i = 0; i < 10; ++i) {
            socket.connect(tcp::endpoint(asio::ip::address::from_string("127.0.0.1"), 3030));

            socket.async_read_some(asio::buffer(buf, buf.size()), [&](const asio::error_code &ec, std::size_t len) {});
//
//            std::cout.write(buf.data(), len);

            socket.close();
        }
    }
    catch (std::exception &e) {
        std::cerr << e.what() << std::endl;
    }

    return 0;
}