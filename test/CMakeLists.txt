add_executable(socomm_test
  main.test.c
)

target_link_libraries(
  socomm_test socomm  
)
