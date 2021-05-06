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
  j.at("data").get_to(project.date);
}

inline void to_json(nlohmann::json &j, Project const &project) {
  j = nlohmann::json{
      {"id", project.id}, {"name", project.name}, {"date", project.date}};
}

inline void from_json(nlohmann::json const &j, Task &task) {
  j.at("id").get_to(task.id);
  j.at("name").get_to(task.name);
  j.at("description").get_to(task.description);
  j.at("date").get_to(task.date);
  j.at("project_id").get_to(task.project_id);
  j.at("urgency").get_to(task.urgency);
  j.at("status").get_to(task.urgency);
}

inline void to_json(nlohmann::json &j, Task const &task) {
  j = nlohmann::json{{"id", task.id},
                     {"name", task.name},
                     {"date", task.date},
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

inline void to_json(nlohmann::json &j, User const &user) {
  j = nlohmann::json{{"id", user.id},
                     {"account_name", user.account_name},
                     {"full_name", user.full_name},
                     {"role_in_system", user.role_in_system}};
}

inline void from_json(nlohmann::json const &j, User &user) {
  j.at("id").get_to(user.id);
  j.at("account_name").get_to(user.account_name);
  j.at("full_name").get_to(user.full_name);
  j.at("role_in_system").get_to(user.role_in_system);
}

inline void to_json(nlohmann::json &j, TaskCreateDTO const &taskDTO) {
  j = nlohmann::json{{"description", taskDTO.description},
                     {"name", taskDTO.name},
                     {"urgency", taskDTO.urgency},
                     {"status", taskDTO.status},
                     {"project_id", taskDTO.project_id}};
}

inline void from_json(nlohmann::json const &j, TaskCreateDTO &taskDTO) {
  j.at("name").get_to(taskDTO.name);
  j.at("description").get_to(taskDTO.description);
  j.at("urgency").get_to(taskDTO.urgency);
  j.at("status").get_to(taskDTO.status);
  j.at("project_id").get_to(taskDTO.project_id);
}

inline void to_json(nlohmann::json &j, ProjectCreateDTO const &projectDTO) {
  j = nlohmann::json{{"name", projectDTO.name}, {"date", projectDTO.date}};
}

inline void from_json(nlohmann::json const &j, ProjectCreateDTO &projectDTO) {
  j.at("name").get_to(projectDTO.name);
  j.at("date").get_to(projectDTO.date);
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

template <typename T>
inline void to_json(nlohmann::json &j, const ResponseFormat<T> &request) {
  j = nlohmann::json{{"error", request.error}, {"data", request.data}};
}

template <typename T>
inline void from_json(nlohmann::json const &j, ResponseFormat<T> &request) {
  j.at("data").get_to(request.data);
  j.at("error").get_to(request.error);
}

/* DTO Serialization */

inline Task from_dto(TaskCreateDTO const &dto) {
  Task task;
  task.name = dto.name;
  task.description = dto.description;
  task.status = dto.status;
  task.urgency = dto.urgency;
  task.project_id = dto.project_id;
  return task;
}

inline Task from_dto(TaskCreateDTO &&dto) {
  Task task;
  task.name = std::move(dto.name);
  task.description = std::move(dto.description);
  task.status = std::move(dto.status);
  task.urgency = dto.urgency;
  task.project_id = dto.project_id;
  return task;
}

inline Project from_dto(ProjectCreateDTO const &dto) {
  Project project;
  project.name = dto.name;
  project.date = dto.date;
  return project;
}

inline Project from_dto(ProjectCreateDTO &&dto) {
  Project project;
  project.name = std::move(dto.name);
  project.date = std::move(dto.date);
  return project;
}

#endif  // CO_WORK_SERIALIZATION_H
