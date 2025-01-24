# mathexpr library compatibility
add_library(sequencer-mathexpr SHARED)

set_target_properties(sequencer-mathexpr PROPERTIES
  EXPORT_NAME sequencer-mathexpr
  SOVERSION ${LIBSOVERSION}
  VERSION ${LIBVERSION}
  LIBRARY_OUTPUT_DIRECTORY ${PLUGIN_RUNTIME_DIRECTORY}
)

target_sources(sequencer-mathexpr
  PRIVATE
  dummy.cpp
)

target_link_libraries(sequencer-mathexpr PUBLIC oac-tree-mathexpr)

install(TARGETS sequencer-mathexpr DESTINATION ${PLUGIN_PATH})
