add_executable(socomm_test
  broadcast_handler.test.c
)

target_link_libraries(
  socomm_test socomm  
)
