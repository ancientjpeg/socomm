add_library(socomm
  socomm.cpp
  socomm.h
)

get_filename_component(PARENT_DIR "${CMAKE_CURRENT_LIST_DIR}" DIRECTORY)
target_include_directories(socomm PUBLIC 
  ${PARENT_DIR}
)

include(FetchContent)

FetchContent_Declare(
  czmq
  GIT_REPOSITORY https://github.com/zeromq/czmq
  GIT_TAG        v4.2.1 
)

FetchContent_MakeAvailable(czmq)

target_link_libraries(socomm PRIVATE czmq)
