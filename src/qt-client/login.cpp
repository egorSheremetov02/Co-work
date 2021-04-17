#include "login.h"
#include "ui_login.h"
#include "../../src/shared/serialization.h"
#include "../../src/shared/request_format.h"
#include "../../src/shared/response_format.h"
#include "../../src/shared/request.h"
#include "../../src/shared/response.h"

#include<asio.hpp>
#include<nlohmann/json.hpp>
#include <iostream>

using asio::ip::tcp;
using nlohmann::json;

std::string auth_response;

asio::io_service &get_service() {
    static auto *service = new asio::io_service();
    return *service;
}

tcp::socket &get_socket() {
    static auto *socket = new tcp::socket(get_service());
    return *socket;
}

login::login(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::login)
{
    ui->setupUi(this);
}

login::~login()
{
    delete ui;
}

void login::on_pushButton_logIn_clicked()
{
//    QString username = ui ->lineEdit_username -> text();
//    QString password = ui ->lineEdit_password -> text();

//     const asio::ip::basic_endpoint<tcp> &endpoint = tcp::endpoint(asio::ip::address::from_string("127.0.0.1"),
//                                                                          3030);

//     auto *socket1 = new tcp::socket(get_service());

//     socket1->connect(endpoint);

//     RequestFormat<AuthReqDTO> auth_request;
//     auth_request.resource = "authenticate";
//     auth_request.data = {username.toStdString(), password.toStdString()};
//     json j = auth_request;
//     auth_response.resize(1024);
//     std::string str_auth_request = j.dump();

//     socket1->write_some(asio::buffer(str_auth_request.data(), str_auth_request.size()));
//     std::string server_response;
//     server_response.resize(1024);

//     std::size_t len = socket1->read_some(asio::buffer(server_response.data(), server_response.size()));

//     json json_auth_response = json::parse(server_response.substr(0, len));
//      auto response = json_auth_response.get<ResponseFormat<User>>();
//      if (response.error.empty()) {
//             _kanban = new kanban();
//             _kanban->show();
//       } else {
//            ui -> label_errors -> setText("ти питун");
//            socket1 -> close();
//      }
         _kanban = new kanban();
         _kanban->show();
}
