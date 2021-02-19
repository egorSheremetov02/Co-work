//
// Created by egor on 19.02.2021.
//

#ifndef CO_WORK_USER_H
#define CO_WORK_USER_H

enum UserRole {
    ADMIN, USER
};

struct User {
    int id;
    std::string account_name;
    std::string full_name;
    std::string password;
    UserRole role;
};
#endif //CO_WORK_USER_H
