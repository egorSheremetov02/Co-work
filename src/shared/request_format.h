//
// Created by egor on 19.02.2021.
//

#ifndef CO_WORK_REQUEST_FORMAT_H
#define CO_WORK_REQUEST_FORMAT_H

#include "../../include/structures.h"
#include <nlohmann/json.hpp>

template<typename T>
struct RequestFormat {
    std::string resource;
    T data;
};

template<typename T>
struct AuthorizedRequestFormat : RequestFormat<T> {
    User user;
    std::string token;
};


template<typename T>
void to_json(nlohmann::json &j, const RequestFormat<T> &request) {
    j = nlohmann::json{{"resource", request.resource},
                       {"data",     request.data}};
}

template <typename T>
void to_json(nlohmann::json &j, const AuthorizedRequestFormat<T> &request) {
    to_json(j, static_cast<RequestFormat<T> &>)
}

template<typename T>
void from_json(nlohmann::json const &j, RequestFormat<T> &request) {
    j.at("data").get_to(request.data);
    j.at("resource").get_to(request.resource);
}

template<typename T>
void from_json(nlohmann::json const &j, AuthorizedRequestFormat<T> &request) {
    from_json<T>(j, static_cast<RequestFormat<T> &>(request));
    j.at("user").get_to(request.user);
    j.at("token").get_to(request.token);
}


#endif //CO_WORK_MESSAGE_FORMAT_H
