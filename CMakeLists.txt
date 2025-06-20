cmake_minimum_required(VERSION 3.10)

set(CMAKE_GENERATOR
        "Unix Makefiles"
        CACHE STRING "generator"
)
set(CMAKE_CXX_STANDARD 20)

project(zappySERVER
        VERSION 1.0.0
        DESCRIPTION "Game server for the zappy project"
        LANGUAGES CXX
)

if(CMAKE_BUILD_TYPE STREQUAL "Debug")
    set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} -g3 -O0")
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -g3 -O0")
endif()

file(GLOB_RECURSE src "src/*.cpp")
add_executable(zappy_server ${src})

set_target_properties(zappy_server
        PROPERTIES PREFIX "${CMAKE_SOURCE_DIR}/"
)
