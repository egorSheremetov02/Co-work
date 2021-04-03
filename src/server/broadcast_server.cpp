//
// Created by egor on 13.02.2021.
//
#include "server.hpp"
#include <asio.hpp>


int main() {
    asio::io_context service;
#ifdef LOGGING
    std::cout << "Starting server..." << std::endl;
#endif
    ConnectionAcceptor server(service);
    service.run();
}

