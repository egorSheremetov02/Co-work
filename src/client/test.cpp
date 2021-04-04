//
// Created by egor on 26.02.2021.
//
#include <iostream>
#include <nlohmann/json.hpp>
#include "src/shared/structures.h"
#include "src/shared/response_format.h"
#include "src/shared/request_format.h"
#include "src/shared/request.h"
#include "src/shared/serialization.h"
#include "date/tz.h"

using nlohmann::json;

int main() {
    //    auto get_test_user = [&]() -> User {
    //        User u;
    //        u.id = 1;
    //        u.role_in_system = ADMIN;
    //        u.account_name = "ideal.idealov";
    //        u.full_name = "Ideal Idealov";
    //        return u;
    //    };
    // Test that timezone conversion works properly
    {
        using namespace date;
        using namespace std::chrono;
        auto utc = system_clock::now();
        auto berlin = make_zoned("Europe/Berlin", utc);
        auto local  = make_zoned(current_zone(), berlin);
        auto utc2   = berlin.get_sys_time();
        std::cout << format("%F %T %Z", utc) << '\n';
        std::cout << format("%F %T %Z", berlin) << '\n';
        std::cout << format("%F %T %Z", local) << '\n';
        std::cout << format("%F %Z", local) << '\n';
        std::cout << format("%F %T %Z", utc2) << '\n';
    }
}
