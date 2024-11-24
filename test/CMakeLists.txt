file(GLOB_RECURSE TEST_FILES "${CMAKE_CURRENT_LIST_DIR}/*.test.c")

foreach(test_file ${TEST_FILES})
    get_filename_component(test_name ${test_file} NAME_WE)

    set(test_target_name "socomm_${test_name}_test")

    add_executable(${test_target_name}
      ${test_file}
    )
    target_link_libraries(
      ${test_target_name} socomm  
    )

    add_test(NAME ${test_target_name} COMMAND ${test_target_name})
    message("ADD TEST ${test_target_name}")
endforeach()


