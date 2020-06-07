cmake_minimum_required(VERSION 3.10)
project(perfromTest)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ./bin)

ADD_SUBDIRECTORY(db/mysq/orm)
add_executable(perfromTest
        main.cpp
        Loop.cpp
        Loop.h
        LibEvent.cpp
        LibEvent.h
        Mainloop.cpp
        Mainloop.h
        Utils.h
        Utils.cpp
        CirQueue.cpp
        CirQueue.h)

target_link_libraries(perfromTest pthread)
target_link_libraries(perfromTest event)
target_link_libraries(perfromTest crypto)
target_link_libraries(perfromTest ssl)
target_link_libraries(perfromTest mysqlcppconn8)

