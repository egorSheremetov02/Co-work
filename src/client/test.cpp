//
// Created by egor on 26.02.2021.
//
#include <iostream>
#include <nlohmann/json.hpp>
#include "../../include/structures.h"
#include "../shared/response_format.h"
#include "../shared/request_format.h"
#include "../shared/request.h"

using nlohmann::json;

int main() {
    auto get_test_user = [&]() -> User {
        User u;
        u.id = 1;
        u.role_in_system = ADMIN;
        u.account_name = "ideal.idealov";
        u.full_name = "Ideal Idealov";
        return u;
    };
        // serialization tests
    {
        // TEST 1 (User is serialized and deserialized in JSON as expected)
        {
            User u = get_test_user();
            json j = u;
            User u1 = j.get<User>();
            assert(u1.id == 1);
            assert(u1.full_name == "Ideal Idealov");
            assert(u1.account_name == "ideal.idealov");
            assert(u1.role_in_system == ADMIN);
        }
        // TEST 2 (Request is serialized and deserialized in JSON as expected)
        {
            User u = get_test_user();
            RequestFormat<User> request;
            request.resource = "auth";
            request.data = u;
            json j = request;
            auto request1 = j.get<RequestFormat<User>>();
            assert(request1.resource == "auth");
            assert((request1.data.full_name == u.full_name && request1.data.role_in_system == u.role_in_system &&
                    request1.data.account_name == u.account_name));
        }
        // TEST 3 (Auth Request is serialized and deserialized as expected)
        {
            AuthDTO dto = {
                    "ideal.idealov",
                    "qwerty"
            };
            RequestFormat<AuthDTO> auth_request;
            auth_request.resource = "auth get";
            auth_request.data = dto;
            json j = auth_request;
            auto auth_request1 = j.get<RequestFormat<AuthDTO>>();
            assert(auth_request1.resource == auth_request.resource);
            assert(auth_request1.data.login == auth_request.data.login);
            assert(auth_request1.data.password == auth_request.data.password);
        }
        // TEST 4 (Auth Response is serialized and deserialized as expected)
        {
            // No error case
            User u = get_test_user();
            ResponseFormat<User> auth_response;
            auth_response.data = u;
            json j1 = auth_response;
            auto auth_response1 = j1.get<ResponseFormat<User>>();
            assert(auth_response1.error.empty());
            assert((auth_response1.data.full_name == u.full_name &&
                    auth_response1.data.role_in_system == u.role_in_system &&
                    auth_response1.data.account_name == u.account_name));
            ResponseFormat<User> auth_response_error;
            auth_response_error.error = "Bad password";
            json j2 = auth_response_error;
            auto auth_response_error1 = j2.get<ResponseFormat<User>>();
            assert(!auth_response_error1.error.empty());
        }
    }
}
