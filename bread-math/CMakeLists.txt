cmake_minimum_required(VERSION 3.16)
project(bread-math LANGUAGES CXX)

file(GLOB_RECURSE BREAD_MATH_HEADERS "${CMAKE_CURRENT_SOURCE_DIR}/Include/*.h" "${CMAKE_CURRENT_SOURCE_DIR}/Include/*.hpp")
file(GLOB_RECURSE BREAD_MATH_SOURCES "${CMAKE_CURRENT_SOURCE_DIR}/Source/*.cpp")

add_library(bread-math STATIC ${BREAD_MATH_SOURCES} ${BREAD_MATH_HEADERS})

target_include_directories(bread-math
  PUBLIC
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/Include>
    $<INSTALL_INTERFACE:include/bread-math>
)

target_compile_features(bread-math PUBLIC cxx_std_17)

# bread-math depends on bread-fnd
target_link_libraries(bread-math PUBLIC bread-fnd)

install(TARGETS bread-math EXPORT bread-mathTargets
  ARCHIVE DESTINATION lib
  LIBRARY DESTINATION lib
  RUNTIME DESTINATION bin
)
install(DIRECTORY Include/ DESTINATION include)
