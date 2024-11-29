set(TEST_HELPERS_DIR "${CMAKE_CURRENT_LIST_DIR}/helpers")
file(GLOB_RECURSE TEST_FILES "${CMAKE_CURRENT_LIST_DIR}/*.test.c")
file(GLOB_RECURSE TEST_HELPERS "${TEST_HELPERS_DIR}/*.c")

foreach(test_file ${TEST_FILES})
    get_filename_component(test_name ${test_file} NAME_WE)

    set(test_target_name "socomm_${test_name}_test")

    add_executable(${test_target_name}
      ${test_file}
      ${TEST_HELPERS}
    )

    target_link_libraries(
      ${test_target_name} socomm  
    )

    target_include_directories(
        ${test_target_name}
        PRIVATE
        ${TEST_HELPERS_DIR}
    )

    if (${SOCOMM_VERBOSE_TESTS})
        target_compile_definitions(socomm
            SOCOMM_VERBOSE_TESTS=1
        )
    endif()

    add_test(NAME ${test_target_name} COMMAND ${test_target_name})
    message("ADD TEST ${test_target_name}")

endforeach()
