//
// Created by egor on 20.05.2021.
//

#ifndef CO_WORK_GLOBALS_H
#define CO_WORK_GLOBALS_H
#include <string>

namespace defaults::server {
extern const int default_buffer_size;
extern const std::string edit_task_metadata;
extern const std::string create_task_metadata;
extern const std::string create_project_metadata;
extern const std::string get_all_tasks_metadata;
extern const std::string project_subscription_prefix;
}  // namespace defaults::server

namespace endpoints::tasks {
extern const std::string get_all;
extern const std::string create;
extern const std::string edit;
}  // namespace endpoints::tasks

namespace endpoints::projects {
extern const std::string create;
}

namespace endpoints::task_history {
extern const std::string get_all;
}

#endif  // CO_WORK_GLOBALS_H
