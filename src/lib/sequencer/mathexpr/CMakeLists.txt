add_library(sequencer-mathexpr SHARED)

set_target_properties(sequencer-mathexpr PROPERTIES
  EXPORT_NAME sequencer-mathexpr
  VERSION ${LIBVERSION}
  SOVERSION ${LIBSOVERSION}
  LIBRARY_OUTPUT_DIRECTORY ${PLUGIN_RUNTIME_DIRECTORY}
)

target_sources(sequencer-mathexpr
  PRIVATE
    math_instruction.cpp
    variable_handler.cpp
)

target_include_directories(sequencer-mathexpr PUBLIC
  $<INSTALL_INTERFACE:include>
  $<BUILD_INTERFACE:${CMAKE_CURRENT_LIST_DIR}/../..>
)

target_link_libraries(sequencer-mathexpr PUBLIC sequencer::sequencer sup-mathexpr::sup-mathexpr)

install(TARGETS sequencer-mathexpr DESTINATION ${PLUGIN_PATH})

