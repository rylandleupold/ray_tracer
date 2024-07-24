cmake_minimum_required(VERSION 3.28.0)
project(RayTracer VERSION 0.1.0 LANGUAGES C CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_C_STANDARD_REQUIRED True)
add_executable(${PROJECT_NAME} main.cpp)

