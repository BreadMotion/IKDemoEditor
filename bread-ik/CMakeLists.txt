cmake_minimum_required(VERSION 3.16)
project(bread-ik LANGUAGES CXX)

# Placeholder: Source collection to be filled in next steps
file(GLOB_RECURSE BREAD_IK_HEADERS "${CMAKE_CURRENT_SOURCE_DIR}/Include/*.h" "${CMAKE_CURRENT_SOURCE_DIR}/Include/*.hpp")
file(GLOB_RECURSE BREAD_IK_SOURCES "${CMAKE_CURRENT_SOURCE_DIR}/Source/*.cpp")

add_library(bread-ik STATIC ${BREAD_IK_SOURCES} ${BREAD_IK_HEADERS})

target_include_directories(bread-ik
  PUBLIC
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/Include>
    $<INSTALL_INTERFACE:include/bread-ik>
)

target_compile_features(bread-ik PUBLIC cxx_std_17)

# Dependencies
target_link_libraries(bread-ik PUBLIC bread-math bread-framework)

install(TARGETS bread-ik EXPORT bread-ikTargets
  ARCHIVE DESTINATION lib
  LIBRARY DESTINATION lib
  RUNTIME DESTINATION bin
)
install(DIRECTORY Include/ DESTINATION include)
