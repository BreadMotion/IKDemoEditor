cmake_minimum_required(VERSION 3.16)
project(bread-framework LANGUAGES CXX)

# Placeholder: Source collection to be filled in next steps
file(GLOB_RECURSE BREAD_FRAMEWORK_HEADERS "${CMAKE_CURRENT_SOURCE_DIR}/Include/*.h" "${CMAKE_CURRENT_SOURCE_DIR}/Include/*.hpp")
file(GLOB_RECURSE BREAD_FRAMEWORK_SOURCES "${CMAKE_CURRENT_SOURCE_DIR}/Source/*.cpp")

add_library(bread-framework STATIC ${BREAD_FRAMEWORK_SOURCES} ${BREAD_FRAMEWORK_HEADERS})

target_include_directories(bread-framework
  PUBLIC
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/Include>
    $<INSTALL_INTERFACE:include/bread-framework>
)

target_compile_features(bread-framework PUBLIC cxx_std_17)

# Dependencies
target_link_libraries(bread-framework PUBLIC bread-fnd bread-math bread-graphics bread-resource)

install(TARGETS bread-framework EXPORT bread-frameworkTargets
  ARCHIVE DESTINATION lib
  LIBRARY DESTINATION lib
  RUNTIME DESTINATION bin
)
install(DIRECTORY Include/ DESTINATION include)
