//
// Created by egor on 24.04.2021.
//
#ifndef CO_WORK_APP_DB_CONNECTION_H
#define CO_WORK_APP_DB_CONNECTION_H
#include "../data_base/db_connection.h"
#include "../shared/structures.h"

DataBase &get_app_db();

Project &get_project_tmp();

#endif  // CO_WORK_APP_DB_CONNECTION_H
