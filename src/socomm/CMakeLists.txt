add_library(socomm
  broadcast_handler.c
  helpers.c
  node.c
)

file(GLOB HEADER_FILES "${CMAKE_CURRENT_LIST_DIR}/*.h")  # Adjust the pattern to match your directory structure
set_source_files_properties(${HEADER_FILES} PROPERTIES LANGUAGE C)

get_filename_component(PARENT_DIR "${CMAKE_CURRENT_LIST_DIR}" DIRECTORY)
target_include_directories(socomm PUBLIC 
    ${PARENT_DIR}
)
target_include_directories(socomm 
    PRIVATE
    ${CMAKE_CURRENT_LIST_DIR}
    INTERFACE 
    ${CMAKE_SOURCE_DIR}/include
)

Include(FetchContent)
FetchContent_Declare(
    libzmq
    GIT_REPOSITORY https://github.com/zeromq/libzmq.git
    GIT_TAG        v4.3.5
)
FetchContent_MakeAvailable(libzmq)


target_compile_definitions(libzmq-static
    PUBLIC
    ZMQ_BUILD_DRAFT_API
)

target_link_libraries(socomm PUBLIC libzmq-static)


