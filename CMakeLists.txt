cmake_minimum_required(VERSION 3.16)
project(BREADLibrary LANGUAGES CXX)

# オプション
option(BUILD_SHARED_LIBS "Build shared libraries" OFF)
option(BUILD_TESTS "Build unit tests" ON)

# 共通設定
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# サブディレクトリ（段階的に増やす）
add_subdirectory(bread-fnd)
add_subdirectory(bread-math)

# examples（後で追加）
# add_subdirectory(examples)
