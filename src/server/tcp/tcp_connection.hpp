//
// Created by egor on 27.02.2021.
//

#ifndef CO_WORK_TCP_CONNECTION_HPP
#define CO_WORK_TCP_CONNECTION_HPP
#include <asio.hpp>
#include <string>
#include "response_format.h"
#include "structures.h"

struct TcpConnection : public std::enable_shared_from_this<TcpConnection> {
  public:
  using pointer = std::shared_ptr<TcpConnection>;

  static pointer create(asio::io_context &io_context,
                        std::size_t max_in_message_size = 1024,
                        std::size_t max_out_message_size = 1024);

  void do_read();

  asio::ip::tcp::socket &socket();

  std::string &in_message();

  std::string &out_message();

  private:
  explicit TcpConnection(asio::io_context &io_context,
                         std::size_t max_in_message_size,
                         std::size_t max_out_message_size);

  void handle_write();

  asio::ip::tcp::socket socket_;
  std::string in_message_;
  std::string out_message_;
};

void authenticate(TcpConnection::pointer &connection);

void authentication_handler(std::size_t len,
                            TcpConnection::pointer &connection);

void write_auth_response(ResponseFormat<User> const &auth_response,
                         TcpConnection::pointer &connection);

#endif  // CO_WORK_TCP_CONNECTION_HPP
