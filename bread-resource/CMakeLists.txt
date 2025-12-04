cmake_minimum_required(VERSION 3.16)
project(bread-resource LANGUAGES CXX)

# Placeholder: Source collection to be filled in next steps
file(GLOB_RECURSE BREAD_RESOURCE_HEADERS "${CMAKE_CURRENT_SOURCE_DIR}/Include/*.h" "${CMAKE_CURRENT_SOURCE_DIR}/Include/*.hpp")
file(GLOB_RECURSE BREAD_RESOURCE_SOURCES "${CMAKE_CURRENT_SOURCE_DIR}/Source/*.cpp")

add_library(bread-resource STATIC ${BREAD_RESOURCE_SOURCES} ${BREAD_RESOURCE_HEADERS})

target_include_directories(bread-resource
  PUBLIC
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/Include>
    $<INSTALL_INTERFACE:include/bread-resource>
)

target_compile_features(bread-resource PUBLIC cxx_std_17)

# Dependencies
target_link_libraries(bread-resource PUBLIC bread-fnd bread-graphics bread-math)

install(TARGETS bread-resource EXPORT bread-resourceTargets
  ARCHIVE DESTINATION lib
  LIBRARY DESTINATION lib
  RUNTIME DESTINATION bin
)
install(DIRECTORY Include/ DESTINATION include)
