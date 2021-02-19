//ограничение на h TODO

#include <string>

enum roles {
    ADMIN
};
enum actions {
    ADD_COMMENT, CREATE_TASK
};

struct task {
    int id;
    std::string name;
    std::string description;
    std::string date;
    int project_id;
    int urgency;
    std::string status;
};

struct project {
    int id;
    std::string name;
    std::string date;
};

struct user {
    int id;
    std::string account_name;
    std::string full_name;
    roles role_in_system;
};

struct action {
    int task_id;
    int user_id;
    std::string data;
};

struct create : action {
    std::string text;
};

struct comment : action {
    std::string text_of_comment;
};
