cmake_minimum_required(VERSION 3.2)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -pthread -std=c++17")
SET(ENABLE_MYSQL ON)
if (ENABLE_MYSQL)
add_definitions(-DORMPP_ENABLE_MYSQL)
set(SOURCE_FILES main.cpp dbng.hpp unit_test.hpp pg_types.h
        type_mapping.hpp utility.hpp entity.hpp mysql.hpp
        connection_pool.hpp ormpp_cfg.hpp)
endif()

INCLUDE_DIRECTORIES(
                    ${CMAKE_SOURCE_DIR}/iguana
                   )

add_library(ormpp_static ${SOURCE_FILES})
set_target_properties(ormpp_static PROPERTIES OUTPUT_NAME ormpplib)
set_target_properties(ormpp_static PROPERTIES CLEAN_DIRECT_OUTPUT 1)

if (ENABLE_MYSQL)
target_link_libraries(ormpp_static mysqlclient)
endif()
