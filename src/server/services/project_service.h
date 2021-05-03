//
// Created by egor on 03.05.2021.
//

#include "structures.h"

#ifndef CO_WORK_PROJECT_SERVICE_H
#define CO_WORK_PROJECT_SERVICE_H

namespace project_service {

Project get_project(uint32_t id);

Project create_project(ProjectCreateDTO &);

}  // namespace project_service

#endif  // CO_WORK_PROJECT_SERVICE_H
