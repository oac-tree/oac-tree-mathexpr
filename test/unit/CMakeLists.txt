set(unit-tests oac-tree-mathexpr-unit-tests)

add_executable(${unit-tests})

set_target_properties(${unit-tests} PROPERTIES OUTPUT_NAME "unit-tests")
set_target_properties(${unit-tests} PROPERTIES RUNTIME_OUTPUT_DIRECTORY ${TEST_OUTPUT_DIRECTORY})

target_sources(${unit-tests}
  PRIVATE
    ${CMAKE_CURRENT_LIST_DIR}/math_tests.cpp
    ${CMAKE_CURRENT_LIST_DIR}/test_user_interface.cpp
    ${CMAKE_CURRENT_LIST_DIR}/unit_test_helper.cpp
)

target_include_directories(${unit-tests}
  PRIVATE
    ${CMAKE_CURRENT_LIST_DIR}
)

find_package(GTest REQUIRED)

target_link_libraries(${unit-tests} PUBLIC GTest::GTest GTest::Main oac-tree-mathexpr)

gtest_discover_tests(${unit-tests} DISCOVERY_MODE PRE_TEST)
