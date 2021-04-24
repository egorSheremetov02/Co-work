//
// Created by egor on 17.04.2021.
//
#include "task_service.h"
#include "app_db_connection.h"
#include "../shared/serialization.h"
#include "../data_base/orm.h"

Task TaskService::create_task(TaskCreateDTO const &taskDTO) {

    Task task = from_dto(taskDTO);

    task.project_id = 1;
    task.date = "01-01-2021";
    task.id = get_app_db().create_task(task);

    return task;
}

std::vector<Task> TaskService::get_tasks() {
    return get_app_db().tasks.select_all();
}

Task TaskService::edit_task(uint32_t task_id) {
    std::vector<Task> tasks = std::move(get_tasks());
    Task &to_be_edited = *std::find_if(std::begin(tasks), std::end(tasks), [&](Task const &task) {
        return task_id == task.id;
    });

    to_be_edited.description += " edited ";

    return to_be_edited;
}

