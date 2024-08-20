cmake_minimum_required(VERSION 3.10)

set(APP_NAME BreadEditor)
project(${APP_NAME})

add_subdirectory(Library)
add_subdirectory(BREADProject)

target_link_libraries(${APP_NAME} PRIVATE Library)
target_include_directories(${APP_NAME} PRIVATE "")