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
struct AuthorizedFormat : RequestFormat<T> {
    User user;
    std::string token;
};

namespace nlohmann {

    template<typename T>
    void to_json(json &j, const RequestFormat<T> &request) {
        json i;
        j = json{{"resource", request.resource},
                 {"data",     request.data}};
    }

}

#endif //CO_WORK_MESSAGE_FORMAT_H
