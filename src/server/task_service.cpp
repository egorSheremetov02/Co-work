//
// Created by egor on 17.04.2021.
//
#include "task_service.h"

Task TaskService::create_task(TaskCreateDTO const &taskDTO) {

    Task task{
            static_cast<unsigned int>(rand() % static_cast<int>(1e9)),
            taskDTO.name,
            taskDTO.description,
            "01.09.2021",
            static_cast<unsigned int>(rand() % static_cast<int>(1e9)),
            taskDTO.urgency,
            taskDTO.status
    };

    get_tasks().push_back(task);

    return task;
}

std::vector<Task> &TaskService::get_tasks() {
    static std::vector<Task> tasks = {
            {1, "task1", "task1 description", "01.01.2021", 12, 9, "in progress"},
            {2, "task2", "task2 description", "01.02.2021", 12, 9, "in progress"}
    };
    return tasks;
}

Task TaskService::edit_task(uint32_t task_id) {
    std::vector<Task> &tasks = get_tasks();
    Task &to_be_edited = *std::find_if(std::begin(tasks), std::end(tasks), [&](Task const &task) {
        return task_id == task.id;
    });

    to_be_edited.description += " edited ";

    return to_be_edited;
}

