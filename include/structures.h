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
            {"USER",  roles::USER}
    };
    return string_to_role[str_role];
}

std::string get_string_from_role(roles const &role) {
    std::unordered_map<roles, std::string> role_to_string = {
            {roles::ADMIN, "ADMIN"},
            {roles::USER,  "USER"}
    };
    return role_to_string[role];
}

void to_json(nlohmann::json &j, roles const &role) {
    j = get_string_from_role(role);
}

void to_json(nlohmann::json &j, const User &user) {
    j = nlohmann::json{{"id",             user.id},
                       {"account_name",   user.account_name},
                       {"full_name",      user.full_name},
                       {"role_in_system", get_string_from_role(user.role_in_system)}};
}

void from_json(nlohmann::json const &j, User &user) {
    j.at("id").get_to(user.id);
    j.at("account_name").get_to(user.account_name);
    j.at("full_name").get_to(user.full_name);
    j.at("role_in_system").get_to(user.role_in_system);
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
