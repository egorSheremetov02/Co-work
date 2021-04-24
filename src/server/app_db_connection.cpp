//
// Created by egor on 24.04.2021.
//

#include "app_db_connection.h"

DataBase &get_app_db() {
    static DataBase db;
    return db;
}

Project &get_project_tmp() {
    static struct Wrapper {
        Project project;
        Wrapper() {
            project.name = "Co-work";
            project.date = "01.06.2021";
            project.id = get_app_db().create_project(project);
        }
    } proj_wrapper;
    return proj_wrapper.project;
}