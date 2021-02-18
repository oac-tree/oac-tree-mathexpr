cmake_minimum_required(VERSION 3.5)

project(sequencer-plugin-mathexpr LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

if(CMAKE_VERSION VERSION_LESS "3.7.0")
    set(CMAKE_INCLUDE_CURRENT_DIR ON)
endif()

add_library(MathExpression SHARED)

set_target_properties(MathExpression PROPERTIES
    VERSION ${LIBVERSION}
    SOVERSION ${LIBSOVERSION}
    )

target_sources(MathExpression
    PRIVATE
        ${CMAKE_CURRENT_LIST_DIR}/exprtk.hpp
        ${CMAKE_CURRENT_LIST_DIR}/MathExpressionEngineExptrk.cpp
        ${CMAKE_CURRENT_LIST_DIR}/MathExpressionNode.cpp
        ${CMAKE_CURRENT_LIST_DIR}/MathExpressionEngineProvider.cpp
    )

target_include_directories(MathExpression
    PRIVATE
        ${CMAKE_CURRENT_LIST_DIR}
        $ENV{CODAC_ROOT}/include
        $ENV{CODAC_ROOT}/include/sup
    )

target_compile_definitions(MathExpression
    PRIVATE
        exprtk_disable_caseinsensitivity
        exprtk_disable_rtl_vecops
    )

target_link_directories(MathExpression
    PUBLIC $ENV{CODAC_ROOT}/lib
    )

target_link_libraries(MathExpression
    PRIVATE
        sequencer
    )

install(TARGETS MathExpression DESTINATION lib)