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
    int id;
    std::string name;
    std::string description;
    std::string date;
    int project_id;
    int urgency;
    std::string status;
};

struct Project {
    int id;
    std::string name;
    std::string date;
};

struct User {
    int id;
    std::string account_name;
    std::string full_name;
    Roles role_in_system;
};

struct Action {
    int task_id;
    int user_id;
    std::string data;
};

struct CreateAction : Action {
    std::string text;
};

struct CommentAction : Action {
    std::string text_of_comment;
};
#endif //CO_WORK_STRUCTURES_H
