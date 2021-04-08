#ifndef CO_WORK_STRUCTURES_H
#define CO_WORK_STRUCTURES_H

#include <string>

enum Roles {
    ADMIN, USER
};

enum Actions {
    ADD_COMMENT, CREATE_TASK
};

struct Task {
    unsigned int id;
    std::string name;
    std::string description;
    std::string date;
    unsigned int project_id;
    unsigned int urgency;
    std::string status;
};

struct Project {
    unsigned int id;
    std::string name;
    std::string date;
};

struct User {
    unsigned int id;
    std::string account_name;
    std::string full_name;
    Roles role_in_system;
};

struct Action {
    unsigned int task_id;
    unsigned int user_id;
    std::string data;
};

struct CreateAction : Action {
    std::string text;
};

struct CommentAction : Action {
    std::string text_of_comment;
};

struct TaskCreateDTO {
    std::string name;
    std::string description;
    uint32_t year;
    uint32_t month;
    uint32_t day;
    uint32_t hour;
    uint32_t second;
    std::string timezone;
};
#endif //CO_WORK_STRUCTURES_H
