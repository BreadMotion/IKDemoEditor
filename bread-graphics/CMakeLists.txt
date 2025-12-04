cmake_minimum_required(VERSION 3.16)
project(bread-graphics LANGUAGES CXX)

# Placeholder: Source collection to be filled in next steps
file(GLOB_RECURSE BREAD_GRAPHICS_HEADERS "${CMAKE_CURRENT_SOURCE_DIR}/Include/*.h" "${CMAKE_CURRENT_SOURCE_DIR}/Include/*.hpp")
file(GLOB_RECURSE BREAD_GRAPHICS_SOURCES "${CMAKE_CURRENT_SOURCE_DIR}/Source/*.cpp")

add_library(bread-graphics STATIC ${BREAD_GRAPHICS_SOURCES} ${BREAD_GRAPHICS_HEADERS})

target_include_directories(bread-graphics
  PUBLIC
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/Include>
    $<INSTALL_INTERFACE:include/bread-graphics>
)

target_compile_features(bread-graphics PUBLIC cxx_std_17)

# Dependencies
target_link_libraries(bread-graphics PUBLIC bread-fnd bread-math)

install(TARGETS bread-graphics EXPORT bread-graphicsTargets
  ARCHIVE DESTINATION lib
  LIBRARY DESTINATION lib
  RUNTIME DESTINATION bin
)
install(DIRECTORY Include/ DESTINATION include)
