cmake_minimum_required(VERSION 3.16)
project(bread-editor LANGUAGES CXX)

# Placeholder: Source collection to be filled in next steps
file(GLOB_RECURSE BREAD_EDITOR_HEADERS "${CMAKE_CURRENT_SOURCE_DIR}/Include/*.h" "${CMAKE_CURRENT_SOURCE_DIR}/Include/*.hpp")
file(GLOB_RECURSE BREAD_EDITOR_SOURCES "${CMAKE_CURRENT_SOURCE_DIR}/Source/*.cpp")

add_library(bread-editor STATIC ${BREAD_EDITOR_SOURCES} ${BREAD_EDITOR_HEADERS})

target_include_directories(bread-editor
  PUBLIC
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/Include>
    $<INSTALL_INTERFACE:include/bread-editor>
)

target_compile_features(bread-editor PUBLIC cxx_std_17)

# Dependencies
target_link_libraries(bread-editor PUBLIC bread-framework bread-graphics bread-resource)

install(TARGETS bread-editor EXPORT bread-editorTargets
  ARCHIVE DESTINATION lib
  LIBRARY DESTINATION lib
  RUNTIME DESTINATION bin
)
install(DIRECTORY Include/ DESTINATION include)
