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

  operator bool() {
    return has_value();
  }

  operator T() {
    return value;
  }

  T get() {
    return value;
  }

  T &val() {
    return value;
  }

  bool has_value() {
    return set;
  }
};

struct Task {
  uint32_t id;
  std::string name;
  std::string description;
  std::string date;
  unsigned int project_id;
  uint8_t urgency;
  std::string status;
};

struct Project {
  uint32_t id;
  std::string name;
  std::string date;
};

struct AttachedFile {};

struct User {
  uint32_t id;
  std::string account_name;
  std::string full_name;
  Roles role_in_system;
  // std::string email;
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
  uint32_t project_id;
};

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
  OptionalField<std::string> date{};
  OptionalField<uint8_t> urgency{};
};

struct ProjectCreateDTO {
  std::string name;
  std::string date;
};

#endif  // CO_WORK_STRUCTURES_H
