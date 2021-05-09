#ifndef CO_WORK_STRUCTURES_H
#define CO_WORK_STRUCTURES_H

#include <string>

enum Roles { ADMIN, USER };

enum Actions { ADD_COMMENT, CREATE_TASK };

// Used instead of standard optional for easier serialization
template <typename T>
struct OptionalField {
  public:
  // NOLINTNEXTLINE
  bool set = false;
  // NOLINTNEXTLINE
  T value{};

  OptionalField() = default;

  // not marked explicit on purpose
  // (implicit conversion seems pretty convenient in this case)
  OptionalField(T const &v) : value(v), set(true) {}

  OptionalField &operator=(T const &v) {
    this->value = v;
    this->set = true;
  }

  OptionalField &operator=(T &&v) {
    this->value = std::move(v);
    this->set = true;
  }

  operator bool() const {
    return has_value();
  }

  operator T() const {
    return value;
  }

  [[nodiscard]] T get() const {
    return value;
  }

  T &val() {
    return value;
  }

  [[nodiscard]] bool has_value() const {
    return set;
  }
};

struct Task {
  uint32_t id;
  std::string name;
  std::string description;
  std::string due_date;
  unsigned int project_id;
  uint8_t urgency;
  std::string status;
  std::string start_date;
};

struct Project {
  uint32_t id;
  std::string name;
  std::string due_date;
  std::string start_date;
};

struct AttachedFile {
  uint32_t task_id;
  std::string path_to_file;
  std::string filename;
};

struct User {
  uint32_t id;
  std::string account_name;
  std::string full_name;
  Roles role_in_system;
  std::string email;
};

struct Action {
  uint32_t task_id;
  uint32_t user_id;
  Actions action_type;
  std::string date;
  std::string data;
};

struct TaskCreateDTO {
  std::string name;
  std::string description;
  uint8_t urgency{};
  std::string status;
  uint32_t project_id{};
  std::string due_date;
};

struct UserCreateDTO {
  std::string account_name;
  std::string full_name;
  Roles role_in_system;
  std::string email;
};

struct AuthReqDTO {
  std::string login;
  std::string password;
  std::string email;
};

struct ProjectCreateDTO {
  std::string name;
  std::string due_date;
};

// TODO: add DTO for files

struct TaskGetAllDTO {
  uint32_t project_id{};
  OptionalField<uint32_t> tasks_per_page{};
  OptionalField<uint32_t> page_number{};
};

struct TaskEditDTO {
  uint32_t task_id{};
  OptionalField<std::string> name{};
  OptionalField<std::string> description{};
  OptionalField<std::string> status{};
  OptionalField<std::string> due_date{};
  OptionalField<uint8_t> urgency{};
};

struct ProjectEditDTO {
  uint32_t project_id{};
  OptionalField<std::string> name{};
  OptionalField<std::string> due_date{};
};

#endif  // CO_WORK_STRUCTURES_H
