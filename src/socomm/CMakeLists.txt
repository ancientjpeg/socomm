add_library(socomm
  broadcast_handler.cpp
  broadcast_handler.h
  helpers.h
  node.cpp
  node.h
  socomm.cpp
  socomm.h
  uuid.h
)

get_filename_component(PARENT_DIR "${CMAKE_CURRENT_LIST_DIR}" DIRECTORY)
target_include_directories(socomm PUBLIC 
  ${PARENT_DIR}
)

Include(FetchContent)
FetchContent_Declare(
    libzmq
    GIT_REPOSITORY https://github.com/zeromq/libzmq.git
    GIT_TAG        v4.3.5
)
FetchContent_MakeAvailable(libzmq)

FetchContent_Declare(
  ustd
  GIT_REPOSITORY https://github.com/ancientjpeg/ustd
  GIT_TAG        main 
)

FetchContent_MakeAvailable(ustd)

target_link_libraries(socomm PUBLIC libzmq-static ustd)


