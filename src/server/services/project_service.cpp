//
// Created by egor on 03.05.2021.
//
#include "project_service.hpp"
#include "app_db_connection.hpp"
#include "serialization.h"
#include "structures.h"

namespace project_service {

Project get_project(uint32_t id) {
  return {1, "co-work", "01.06.2021"};
}

Project create_project(ProjectCreateDTO &projectDTO) {
  Project project = from_dto(std::move(projectDTO));
  project.id = get_app_db().create_project(project);
  return project;
}

}  // namespace project_service