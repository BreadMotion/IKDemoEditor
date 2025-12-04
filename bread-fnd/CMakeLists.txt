cmake_minimum_required(VERSION 3.16)
project(bread-fnd LANGUAGES CXX)

# ソースの収集（例）
file(GLOB_RECURSE BREAD_FND_HEADERS "${CMAKE_CURRENT_SOURCE_DIR}/Include/*.h" "${CMAKE_CURRENT_SOURCE_DIR}/Include/*.hpp")
file(GLOB_RECURSE BREAD_FND_SOURCES "${CMAKE_CURRENT_SOURCE_DIR}/Source/*.cpp" "${CMAKE_CURRENT_SOURCE_DIR}/Source/*.cxx")

add_library(bread-fnd STATIC ${BREAD_FND_SOURCES} ${BREAD_FND_HEADERS})

target_include_directories(bread-fnd
  PUBLIC
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/Include>
    $<INSTALL_INTERFACE:include/bread-fnd>
)

target_compile_features(bread-fnd PUBLIC cxx_std_17)

# Install (optional)
install(TARGETS bread-fnd EXPORT bread-fndTargets
  ARCHIVE DESTINATION lib
  LIBRARY DESTINATION lib
  RUNTIME DESTINATION bin
)
install(DIRECTORY Include/ DESTINATION include)
