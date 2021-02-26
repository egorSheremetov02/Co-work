//
// Created by egor on 26.02.2021.
//

#ifndef CO_WORK_REQUEST_H
#define CO_WORK_REQUEST_H

#include <nlohmann/json.hpp>

struct AuthDTO {
    std::string login;
    std::string password;
};

void from_json(nlohmann::json const &j, AuthDTO &authDTO) {
    j.at("password").get_to(authDTO.password);
    j.at("login").get_to(authDTO.login);
}

void to_json(nlohmann::json &j, AuthDTO const &authDTO) {
    j = nlohmann::json{{"password", authDTO.password},
                       {"login",    authDTO.login}};
}


//namespace ns {
//    class address {
//    private:
//        std::string street;
//        int housenumber;
//        int postcode;
//
//    public:
//        NLOHMANN_DEFINE_TYPE_INTRUSIVE(address, street, housenumber, postcode)
//    };
//}


#endif //CO_WORK_REQUEST_H
