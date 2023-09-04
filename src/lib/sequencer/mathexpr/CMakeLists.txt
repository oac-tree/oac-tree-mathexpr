add_library(sequencer-mathexpr SHARED)

target_compile_features(sequencer-mathexpr PRIVATE cxx_std_11)

set_target_properties(sequencer-mathexpr PROPERTIES
    EXPORT_NAME sequencer-mathexpr
    SOVERSION ${SUP_SEQUENCER_PLUGIN_MATHEXPR_SOVERSION}
    VERSION ${SUP_SEQUENCER_PLUGIN_MATHEXPR_BUILDVERSION}
    LIBRARY_OUTPUT_DIRECTORY ${PLUGIN_RUNTIME_DIRECTORY}
)

target_sources(sequencer-mathexpr
    PRIVATE
        ${CMAKE_CURRENT_LIST_DIR}/math_instruction.cpp
        ${CMAKE_CURRENT_LIST_DIR}/variable_handler.cpp
)

target_include_directories(sequencer-mathexpr PUBLIC
  $<INSTALL_INTERFACE:include>
  $<BUILD_INTERFACE:${CMAKE_CURRENT_LIST_DIR}/../..>
)

target_link_libraries(sequencer-mathexpr PUBLIC sequencer::sequencer sup-mathexpr)

install(TARGETS sequencer-mathexpr DESTINATION ${PLUGIN_PATH})

