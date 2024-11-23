cmake_minimum_required(VERSION 3.15)
project(socomm LANGUAGES C CXX)

set(CMAKE_C_STANDARD 11)

add_subdirectory(${CMAKE_CURRENT_LIST_DIR}/src/socomm)
# add_subdirectory(${CMAKE_CURRENT_LIST_DIR}/test)
