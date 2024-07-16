
cmake_minimum_required(VERSION 3.29)

project(marble C)

set(CMAKE_C_STANDARD 99)

include_directories(./include)

add_compile_options(-Wall -Wextra -Wpedantic -Werror)

set(marble_src
    src/marble_hash.c
    src/marble_list.c
)

add_library(${PROJECT_NAME}_static STATIC ${marble_src})
add_library(${PROJECT_NAME} SHARED ${marble_src})

set_target_properties(${PROJECT_NAME}_static PROPERTIES OUTPUT_NAME ${PROJECT_NAME})

set_property(TARGET ${PROJECT_NAME}_static PROPERTY C_EXTENSIONS false)
set_property(TARGET ${PROJECT_NAME} PROPERTY C_EXTENSIONS false)

