//
// Created by egor on 13.02.2021.
//
#ifndef CO_WORK_SERVER_HPP
#define CO_WORK_SERVER_HPP

#include <asio.hpp>
#include <deque>
#include <exception>
#include <functional>
#include <iostream>
#include <memory>
#include <nlohmann/json.hpp>
#include <set>
#include <string>
#include <unordered_map>
#include "request.h"
#include "request_format.h"
#include "response.h"
#include "response_format.h"
#include "structures.h"
#include "tcp_connection.h"

using asio::ip::tcp;
using nlohmann::json;

using AuthRequest = RequestFormat<AuthReqDTO>;

struct ConnectionAcceptor {
  public:
  explicit ConnectionAcceptor(asio::io_service &service);

  private:
  void do_accept();

  void handle_accept(TcpConnection::pointer connection);

  tcp::acceptor acceptor_;
  asio::io_service &service_;
};

#endif  // CO_WORK_SERVER_HPP
