//
// Created by egor on 26.02.2021.
//
#include <iostream>
#include <nlohmann/json.hpp>
#include "../../include/structures.h"
#include "../shared/request_format.h"

using nlohmann::json;

int main() {
    // TEST 1 (User is serialized and deserialized in JSON as expected)
    {
        User u;
        u.id = 1;
        u.role_in_system = ADMIN;
        u.account_name = "ideal.idealov";
        u.full_name = "Ideal Idealov";
        json j = u;
        User u1 = j.get<User>();
        assert(u1.id == 1);
        assert(u1.full_name == "Ideal Idealov");
        assert(u1.account_name == "ideal.idealov");
        assert(u1.role_in_system == ADMIN);
    }
    // TEST 1 (User is serialized and deserialized in JSON as expected)
    {

    }
}
