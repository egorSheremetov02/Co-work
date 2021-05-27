//
// Created by egor on 26.02.2021.
//

#ifndef CO_WORK_SERIALIZATION_H
#define CO_WORK_SERIALIZATION_H

#include "request.h"
#include "request_format.h"
#include "response.h"
#include "response_format.h"
#include "structures.h"

/* JSON Serialization */

NLOHMANN_JSON_SERIALIZE_ENUM(Roles,
                             {
                                 {ADMIN, "admin"},
                                 {USER, "user"},
                             })

inline void from_json(nlohmann::json const &j, Project &project) {
  j.at("id").get_to(project.id);
  j.at("name").get_to(project.name);
  j.at("due_data").get_to(project.due_date);
  j.at("start_data").get_to(project.start_date);
}

inline void to_json(nlohmann::json &j, Project const &project) {
  j = nlohmann::json{{"id", project.id},
                     {"name", project.name},
                     {"due_date", project.due_date},
                     {"start_date", project.start_date}};
}

inline void from_json(nlohmann::json const &j, Task &task) {
  j.at("id").get_to(task.id);
  j.at("name").get_to(task.name);
  j.at("description").get_to(task.description);
  j.at("due_date").get_to(task.due_date);
  j.at("start_date").get_to(task.start_date);
  j.at("project_id").get_to(task.project_id);
  j.at("urgency").get_to(task.urgency);
  j.at("status").get_to(task.urgency);
}

inline void to_json(nlohmann::json &j, Task const &task) {
  j = nlohmann::json{{"id", task.id},
                     {"name", task.name},
                     {"due_date", task.due_date},
                     {"start_date", task.start_date},
                     {"description", task.description},
                     {"project_id", task.project_id},
                     {"urgency", task.urgency},
                     {"status", task.status}};
}

inline void from_json(nlohmann::json const &j, AuthReqDTO &authDTO) {
  j.at("password").get_to(authDTO.password);
  j.at("login").get_to(authDTO.login);
}

inline void to_json(nlohmann::json &j, AuthReqDTO const &authDTO) {
  j = nlohmann::json{{"password", authDTO.password}, {"login", authDTO.login}};
}

template <typename T>
inline void to_json(nlohmann::json &j, const RequestFormat<T> &request) {
  j = nlohmann::json{{"resource", request.resource}, {"data", request.data}};
}

template <typename T>
inline void to_json(nlohmann::json &j,
                    const AuthorizedRequestFormat<T> &request) {
  j = nlohmann::json{{"resource", request.resource},
                     {"data", request.data},
                     {"user", request.user},
                     {"token", request.token}};
}

template <typename T>
inline void from_json(nlohmann::json const &j, RequestFormat<T> &request) {
  j.at("data").get_to(request.data);
  j.at("resource").get_to(request.resource);
}

template <typename T>
inline void from_json(nlohmann::json const &j,
                      AuthorizedRequestFormat<T> &request) {
  from_json<T>(j, static_cast<RequestFormat<T> &>(request));
  j.at("user").get_to(request.user);
  j.at("token").get_to(request.token);
}

template <typename T>
inline void from_json(nlohmann::json const &j, OptionalField<T> &opt_field) {
  j.at("value").get_to(opt_field.value);
  j.at("set").get_to(opt_field.set);
}

template <typename T>
inline void to_json(nlohmann::json &j, OptionalField<T> const &opt_field) {
  j = nlohmann::json{{"value", opt_field.value}, {"set", opt_field.set}};
}

inline void to_json(nlohmann::json &j, User const &user) {
  j = nlohmann::json{{"id", user.id},
                     {"account_name", user.account_name},
                     {"full_name", user.full_name},
                     {"role_in_system", user.role_in_system},
                     {"email", user.email}};
}

inline void from_json(nlohmann::json const &j, User &user) {
  j.at("id").get_to(user.id);
  j.at("account_name").get_to(user.account_name);
  j.at("full_name").get_to(user.full_name);
  j.at("role_in_system").get_to(user.role_in_system);
  j.at("email").get_to(user.email);
}

inline void to_json(nlohmann::json &j, TaskCreateDTO const &taskDTO) {
  j = nlohmann::json{
      {"description", taskDTO.description}, {"name", taskDTO.name},
      {"urgency", taskDTO.urgency},         {"status", taskDTO.status},
      {"project_id", taskDTO.project_id},   {"due_date", taskDTO.due_date}};
}

inline void from_json(nlohmann::json const &j, TaskCreateDTO &taskDTO) {
  j.at("name").get_to(taskDTO.name);
  j.at("description").get_to(taskDTO.description);
  j.at("urgency").get_to(taskDTO.urgency);
  j.at("status").get_to(taskDTO.status);
  j.at("due_date").get_to(taskDTO.due_date);
  j.at("project_id").get_to(taskDTO.project_id);
}

inline void to_json(nlohmann::json &j, ProjectCreateDTO const &projectDTO) {
  j = nlohmann::json{{"name", projectDTO.name},
                     {"due_date", projectDTO.due_date}};
}

inline void from_json(nlohmann::json const &j, ProjectCreateDTO &projectDTO) {
  j.at("name").get_to(projectDTO.name);
  j.at("due_date").get_to(projectDTO.due_date);
}

inline void to_json(nlohmann::json &j, TaskGetAllDTO const &tasksDTO) {
  j = nlohmann::json{{"project_id", tasksDTO.project_id},
                     {"page_number", tasksDTO.page_number},
                     {"tasks_per_page", tasksDTO.tasks_per_page}};
}

inline void from_json(nlohmann::json const &j, TaskGetAllDTO &tasksDTO) {
  j.at("project_id").get_to(tasksDTO.project_id);
  j.at("page_number").get_to(tasksDTO.page_number);
  j.at("tasks_per_page").get_to(tasksDTO.tasks_per_page);
}

inline void to_json(nlohmann::json &j, TaskEditDTO &editDTO) {
  j = nlohmann::json{
      {"task_id", editDTO.task_id},         {"name", editDTO.name},
      {"description", editDTO.description}, {"status", editDTO.status},
      {"due_date", editDTO.due_date},       {"urgency", editDTO.urgency}};
}

inline void from_json(nlohmann::json const &j, TaskEditDTO &editDTO) {
  j.at("task_id").get_to(editDTO.task_id);
  j.at("name").get_to(editDTO.name);
  j.at("description").get_to(editDTO.description);
  j.at("status").get_to(editDTO.status);
  j.at("due_date").get_to(editDTO.due_date);
  j.at("urgency").get_to(editDTO.urgency);
}

//
inline void from_json(nlohmann::json const &j, Comment &comment) {
  j.at("comment").get_to(comment.comment);
}

inline void to_json(nlohmann::json &j, Comment const &comment) {
  j = nlohmann::json{{"comment", comment.comment}};
}

inline void from_json(nlohmann::json const &j, Create &create) {
  j.at("create").get_to(create.create);
}

inline void to_json(nlohmann::json &j, Create const &create) {
  j = nlohmann::json{{"create", create.create}};
}

inline void from_json(nlohmann::json const &j, Edited &editted) {
  j.at("field").get_to(editted.field);
  j.at("before").get_to(editted.before);
  j.at("after").get_to(editted.after);
}

inline void to_json(nlohmann::json &j, Edited const &editted) {
  j = nlohmann::json{{"field", editted.field},
                     {"before", editted.before},
                     {"after", editted.after}};
}

inline void from_json(nlohmann::json const &j, Edit &edit) {
  j.at("edited_fields").get_to(edit.edited_fields);
}

inline void to_json(nlohmann::json &j, Edit const &edit) {
  j = nlohmann::json{{"edited_fields", edit.edited_fields}};
}

//

template <typename T>
inline void to_json(nlohmann::json &j, const ResponseFormat<T> &response) {
  j = nlohmann::json{{"error", response.error},
                     {"data", response.data},
                     {"metadata", response.metadata}};
}

template <typename T>
inline void from_json(nlohmann::json const &j, ResponseFormat<T> &response) {
  j.at("data").get_to(response.data);
  j.at("error").get_to(response.error);
  j.at("metadata").get_to(response.metadata);
}

/* DTO Conversions */
// TODO : put it in own file for conversions since it isn't about serialization

inline Task from_dto(TaskCreateDTO const &dto) {
  Task task;
  task.name = dto.name;
  task.description = dto.description;
  task.status = dto.status;
  task.due_date = dto.due_date;
  task.urgency = dto.urgency;
  task.project_id = dto.project_id;
  return task;
}

inline Task from_dto(TaskCreateDTO &&dto) {
  Task task;
  task.name = std::move(dto.name);
  task.description = std::move(dto.description);
  task.status = std::move(dto.status);
  task.due_date = dto.due_date;
  task.urgency = dto.urgency;
  task.project_id = dto.project_id;
  return task;
}

inline Project from_dto(ProjectCreateDTO const &dto) {
  Project project;
  project.name = dto.name;
  project.due_date = dto.due_date;
  return project;
}

inline Project from_dto(ProjectCreateDTO &&dto) {
  Project project;
  project.name = std::move(dto.name);
  project.due_date = std::move(dto.due_date);
  return project;
}

// TODO
inline User from_dto(UserCreateDTO &&dto) {
  User user;
  /*user.name = std::move(dto.name);
  user.due_date = std::move(dto.due_date);*/
  return user;
}

#endif  // CO_WORK_SERIALIZATION_H
