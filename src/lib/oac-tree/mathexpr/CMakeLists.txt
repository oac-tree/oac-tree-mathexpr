add_library(oac-tree-mathexpr SHARED)

set_target_properties(oac-tree-mathexpr PROPERTIES
  EXPORT_NAME oac-tree-mathexpr
  VERSION ${LIBVERSION}
  SOVERSION ${LIBSOVERSION}
  LIBRARY_OUTPUT_DIRECTORY ${PLUGIN_RUNTIME_DIRECTORY}
)

target_sources(oac-tree-mathexpr
  PRIVATE
    math_instruction.cpp
    variable_handler.cpp
)

target_include_directories(oac-tree-mathexpr PUBLIC
  $<INSTALL_INTERFACE:include>
  $<BUILD_INTERFACE:${CMAKE_CURRENT_LIST_DIR}/../..>
)

target_link_libraries(oac-tree-mathexpr PUBLIC oac-tree::oac-tree sup-mathexpr::sup-mathexpr)

install(TARGETS oac-tree-mathexpr DESTINATION ${PLUGIN_PATH})
