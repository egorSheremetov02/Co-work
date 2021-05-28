//
// Created by egor on 20.05.2021.
//
#include "globals.h"

const int defaults::server::default_buffer_size = 1 << 12;

const std::string defaults::server::edit_task_metadata = "task edited";

const std::string defaults::server::create_task_metadata = "task created";

const std::string defaults::server::project_subscription_prefix = "project";

const std::string endpoints::tasks::create = "task create";

const std::string endpoints::tasks::get_all = "task get_all";

const std::string endpoints::tasks::edit = "task edit";

const std::string endpoints::projects::create = "project create";

const std::string endpoints::task_history::get_all = "task get_history";

const std::string defaults::server::create_project_metadata = "project created";

const std::string defaults::server::get_all_tasks_metadata = "task all";