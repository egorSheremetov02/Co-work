if (UNIX AND NOT APPLE)
    set(LINUX TRUE)
endif ()

cmake_minimum_required(VERSION 3.17)
project(Co_work)
set(CMAKE_CXX_STANDARD 17)

include_directories(.)

include_directories(date/include)
include_directories(src/shared)
include_directories(src/data_base)

find_package(CURL REQUIRED)
include_directories(${CURL_INCLUDE_DIR})
link_libraries(${CURL_LIBRARIES})

include(FetchContent)

FetchContent_Declare(
        asio
        GIT_REPOSITORY https://github.com/chriskohlhoff/asio
        GIT_TAG asio-1-14-0
        UPDATE_DISCONNECTED 1
)

FetchContent_Populate(asio)
include_directories(${asio_SOURCE_DIR}/asio/include/)

FetchContent_Declare(json
        GIT_REPOSITORY https://github.com/nlohmann/json.git
        GIT_TAG v3.7.3
        UPDATE_DISCONNECTED 1)

FetchContent_GetProperties(json)
if (NOT json_POPULATED)
    FetchContent_Populate(json)
    add_subdirectory(${json_SOURCE_DIR} ${json_BINARY_DIR} EXCLUDE_FROM_ALL)
endif ()

#SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -DASIO_STANDALONE -pthread -lpqxx -lpq)
#SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -DASIO_STANDALONE -pthread -lpqxx -lpq -DKRESTINA")
#SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -DASIO_STANDALONE -pthread -lpqxx -lpq -DIRINA")
#SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -DASIO_STANDALONE -pthread -L/usr/local/opt/libpq/lib -I/usr/local/opt/libpq/include -L/usr/local/opt/libpqxx/lib -I/usr/local/opt/libpqxx/include -lpq -DIRINA")
#SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -DASIO_STANDALONE -pthread -L/usr/local/opt/libpq/lib -I/usr/local/opt/libpq/include -L/usr/local/opt/libpqxx/lib -I/usr/local/opt/libpqxx/include -lpq -DIRINA -DDBUSER=co-work")
SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -DASIO_STANDALONE -pthread -lpqxx -lpq -DEGOR -DDBUSER=egor")

if (LINUX)
    message(STATUS ">>> Linux")
    set(PQXX /usr/local/include/pqxx)
else ()
    message(STATUS ">>> MacOs")
    set(PQXX /usr/local/Cellar/libpqxx/7.3.1/include/pqxx)
endif ()

find_library(PQXX_LIB pqxx)
find_library(PQ_LIB pq)

add_executable(client src/client/test_client.cpp)

add_executable(tests date/src/tz.cpp src/client/test.cpp)

add_executable(orm-test src/data_base/db_connection.cpp src/data_base/usage_orm.cpp)

add_executable(broadcast-server
        src/server/application_context.cpp
        src/server/auth_service.cpp
        src/server/tcp_connection.cpp
        src/server/controller.cpp
        src/server/broadcast_server.cpp
        src/server/server.cpp
        src/server/task_handler.cpp
        src/server/handler_registration.cpp
        src/data_base/db_connection.cpp
        src/server/task_service.cpp
        src/server/project_handler.cpp
        src/server/project_service.cpp
        src/server/app_db_connection.cpp)

add_executable(log-broadcast-server
        src/server/application_context.cpp
        src/server/auth_service.cpp
        src/server/tcp_connection.cpp
        src/server/controller.cpp
        src/server/broadcast_server.cpp
        src/server/server.cpp
        src/server/task_handler.cpp
        src/server/handler_registration.cpp
        src/data_base/db_connection.cpp
        src/server/task_service.cpp
        src/server/project_handler.cpp
        src/server/project_service.cpp
        src/server/app_db_connection.cpp)

target_compile_definitions(log-broadcast-server PUBLIC -DLOGGING)
target_link_libraries(log-broadcast-server PRIVATE nlohmann_json::nlohmann_json ${PQXX_LIB} ${PQ_LIB})
#target_link_libraries(log-broadcast-server PUBLIC date/include)

#target_link_libraries(db-test ${PQXX_LIB} ${PQ_LIB})
target_link_libraries(orm-test ${PQXX_LIB} ${PQ_LIB})
target_link_libraries(broadcast-server PRIVATE nlohmann_json::nlohmann_json ${PQXX_LIB} ${PQ_LIB})
#target_link_libraries(broadcast-server PUBLIC date/include)

target_link_libraries(tests PRIVATE nlohmann_json::nlohmann_json)
#target_link_libraries(tests PUBLIC date/include)

target_link_libraries(client PRIVATE nlohmann_json::nlohmann_json)
#target_link_libraries(client PUBLIC date/include)
