if (UNIX AND NOT APPLE)
    set(LINUX TRUE)
endif ()

cmake_minimum_required(VERSION 3.17)
project(Co_work)
set(CMAKE_CXX_STANDARD 17)

include_directories(.)

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

SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -DASIO_STANDALONE -pthread -lpqxx -lpq -DEGOR")
#SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -DASIO_STANDALONE -pthread -lpqxx -lpq -DKRESTINA")

if (LINUX)
    message(STATUS ">>> Linux")
    set(PQXX /usr/local/include/pqxx)
else ()
    message(STATUS ">>> MacOs")
    set(PQXX /usr/local/Cellar/libpqxx/7.3.1/include/pqxx)
endif ()

find_library(PQXX_LIB pqxx)
find_library(PQ_LIB pq)

add_executable(server src/server/main_server.cpp)

add_executable(client src/client/test_client.cpp)

add_executable(tests src/client/test.cpp)

#add_executable(db-test DB_test/a.cpp)

add_executable(broadcast-server
        src/server/application_context.cpp
        src/server/auth_service.cpp
        src/server/tcp_connection.cpp
        src/server/controller.cpp
        src/server/broadcast_server.cpp
        src/server/server.cpp
        src/server/task_handler.cpp
        DB_test/a.cpp)

#target_link_libraries(db-test ${PQXX_LIB} ${PQ_LIB})
target_link_libraries(broadcast-server PRIVATE nlohmann_json::nlohmann_json ${PQXX_LIB} ${PQ_LIB})

target_link_libraries(tests PRIVATE nlohmann_json::nlohmann_json)

target_link_libraries(client PRIVATE nlohmann_json::nlohmann_json)