cmake_minimum_required(VERSION 3.10)
project(perfromTest)

set(CMAKE_CXX_STANDARD 14)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ./bin)

add_executable(perfromTest main.cpp Loop.cpp Loop.h LibEvent.cpp LibEvent.h Mainloop.cpp Mainloop.h Utils.h Utils.cpp CirQueue.cpp CirQueue.h)


target_link_libraries(perfromTest pthread)
target_link_libraries(perfromTest event)