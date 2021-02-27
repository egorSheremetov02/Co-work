//
// Created by egor on 26.02.2021.
//

#ifndef CO_WORK_SERIALIZATION_H
#define CO_WORK_SERIALIZATION_H

#include "request.h"
#include "response.h"
#include "response_format.h"
#include "request_format.h"
#include "structures.h"


NLOHMANN_JSON_SERIALIZE_ENUM(Roles, {
    {ADMIN, "admin"},
    {USER, "user"},
})

inline void from_json(nlohmann::json const &j, AuthReqDTO &authDTO) {
    j.at("password").get_to(authDTO.password);
    j.at("login").get_to(authDTO.login);
}

inline void to_json(nlohmann::json &j, AuthReqDTO const &authDTO) {
    j = nlohmann::json{{"password", authDTO.password},
                       {"login",    authDTO.login}};
}


template<typename T>
inline void to_json(nlohmann::json &j, const RequestFormat<T> &request) {
    j = nlohmann::json{{"resource", request.resource},
                       {"data",     request.data}};
}

template<typename T>
inline void to_json(nlohmann::json &j, const AuthorizedRequestFormat<T> &request) {
    j = nlohmann::json{{"resource", request.resource},
                       {"data",     request.data},
                       {"user",     request.user},
                       {"token",    request.token}};
}

template<typename T>
inline void from_json(nlohmann::json const &j, RequestFormat<T> &request) {
    j.at("data").get_to(request.data);
    j.at("resource").get_to(request.resource);
}

template<typename T>
inline void from_json(nlohmann::json const &j, AuthorizedRequestFormat<T> &request) {
    from_json<T>(j, static_cast<RequestFormat<T> &>(request));
    j.at("user").get_to(request.user);
    j.at("token").get_to(request.token);
}

inline void to_json(nlohmann::json &j, const User &user) {
    j = nlohmann::json{{"id",             user.id},
                       {"account_name",   user.account_name},
                       {"full_name",      user.full_name},
                       {"role_in_system", user.role_in_system}};
}

inline void from_json(nlohmann::json const &j, User &user) {
    j.at("id").get_to(user.id);
    j.at("account_name").get_to(user.account_name);
    j.at("full_name").get_to(user.full_name);
    j.at("role_in_system").get_to(user.role_in_system);
}

template<typename T>
inline void to_json(nlohmann::json &j, const ResponseFormat<T> &request) {
    j = nlohmann::json{{"error", request.error},
                       {"data",  request.data}};
}

template<typename T>
inline void from_json(nlohmann::json const &j, ResponseFormat<T> &request) {
    j.at("data").get_to(request.data);
    j.at("error").get_to(request.error);
}

#endif //CO_WORK_SERIALIZATION_H
