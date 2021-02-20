//ограничение на h TODO

#include <string>

enum roles {
    ADMIN, USER
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

struct User {
    int id;
    std::string account_name;
    std::string full_name;
    roles role_in_system;
};

roles get_role_from_string(std::string const &str_role) {
    std::unordered_map<std::string, roles> string_to_role = {
            {"ADMIN", roles::ADMIN},
            {"USER", roles::USER}
    };
}

void to_json(nlohmann::json &j, const User &user) {
    j = nlohmann::json{{"id", user.id},
                       {"account_name",  user.account_name},
                       {"full_name", user.full_name},
                       {"role_in_system", user.role_in_system}};
}

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
