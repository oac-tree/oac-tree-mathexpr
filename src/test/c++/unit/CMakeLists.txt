add_executable(unit-tests)

target_sources(unit-tests
    PRIVATE
        ${CMAKE_CURRENT_LIST_DIR}/LogUI.cpp
        ${CMAKE_CURRENT_LIST_DIR}/MathExpressionNode-tests.cpp
    )

target_include_directories(unit-tests
    PRIVATE
        ${CMAKE_CURRENT_LIST_DIR}/../../../main/c++
        $ENV{CODAC_ROOT}/include
        $ENV{CODAC_ROOT}/include/sup
    )

target_link_directories(unit-tests
    PRIVATE
        ${CMAKE_CURRENT_LIST_DIR}/../../../../target/lib
        $ENV{CODAC_ROOT}/lib
    )

find_package(GTest REQUIRED)

target_link_libraries(unit-tests MathExpression sequencer GTest::GTest GTest::Main ccs-types ccs-base ccs-common cpp-common-mathexprtk)

install(TARGETS unit-tests DESTINATION bin)