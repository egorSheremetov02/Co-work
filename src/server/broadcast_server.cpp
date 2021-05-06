//
// Created by egor on 13.02.2021.
//
#include <asio.hpp>
#include "server.hpp"

int main() {
  asio::io_context event_loop;
#ifdef LOGGING
  std::cout << "Starting server..." << std::endl;
#endif
  ConnectionAcceptor server(event_loop);
  event_loop.run();
}
