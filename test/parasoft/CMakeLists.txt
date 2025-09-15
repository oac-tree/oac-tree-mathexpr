if(NOT COA_PARASOFT_INTEGRATION)
  return()
endif()

include(COASetupParasoft)

# Add parasoft project for oac-tree-mathexpr-target
# This generates the build time information needed by Parasoft for analysis
cpptest_add_executable(oac-tree-mathexpr-cpptest
  CPPTEST_PROJECT_LOC ${CMAKE_CURRENT_BINARY_DIR}
  CPPTEST_PROJECT_FOLDERS
  oac-tree-mathexpr=${CMAKE_SOURCE_DIR}/src/lib/oac-tree/mathexpr
  TARGETS oac-tree-mathexpr
)
get_target_property(OAC_TREE_MATHEXPR_INCLUDE_DIRECTORIES oac-tree-mathexpr INCLUDE_DIRECTORIES)
target_include_directories(oac-tree-mathexpr-cpptest PUBLIC ${OAC_TREE_MATHEXPR_INCLUDE_DIRECTORIES})

target_link_libraries(oac-tree-mathexpr-cpptest
  PUBLIC
  oac-tree-mathexpr
)
