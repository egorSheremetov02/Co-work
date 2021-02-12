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

SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -DASIO_STANDALONE -pthread -lpqxx -lpq")

if (LINUX)
    message(STATUS ">>> Linux")
    set(PQXX /usr/local/include/pqxx)
else ()
    message(STATUS ">>> Unix")
    set(PQXX /usr/local/Cellar/libpqxx/7.3.1/include/pqxx)
endif ()

find_library(PQXX_LIB pqxx)
find_library(PQ_LIB pq)

add_executable(server src/server/main_server.cpp)

add_executable(client src/client/test_client.cpp)

add_executable(db-test DB_test/a.cpp)

TARGET_LINK_LIBRARIES(db-test ${PQXX_LIB} ${PQ_LIB})
