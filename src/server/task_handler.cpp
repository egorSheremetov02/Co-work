//
// Created by egor on 06.03.2021.
//
#include <iostream>
#include <nlohmann/json.hpp>
#include "handler_registration.h"
#include "../shared/serialization.h"
#include "handler.h"

using nlohmann::json;

void get_all_tasks_handler(json &input_json, TcpConnection::pointer &connection) {
#ifdef LOGGING
    std::cout << "Get all tasks handler" << std::endl;
#endif
    std::vector<Task> tasks = {
            {1, "task1", "task1 description", "01.01.2021", 12, 9, "in progress"},
            {2, "task2", "task2 description", "01.02.2021", 12, 9, "in progress"}
    };
    json out_json;
    out_json["tasks"] = tasks;
    connection->out_message() = out_json.dump();
    connection->socket().async_write_some(
            asio::buffer(connection->out_message().data(), connection->out_message().size()),
            [&, connection](asio::error_code const &ec, std::size_t len) {
#ifdef LOGGING
                std::cout << "Wrote 'task get_all' response: " << connection->out_message().substr(0, len) << std::endl;
#endif
            });
}

void create_task_handler(json &j, TcpConnection::pointer &connection) {
#ifdef LOGGING
    std::cout << "Create new task" << std::endl;
#endif
}

REGISTER_HANDLER("task get_all", get_all_tasks_handler);

REGISTER_HANDLER("task create", create_task_handler);
