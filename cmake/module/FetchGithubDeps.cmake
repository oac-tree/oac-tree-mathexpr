# Collection of functions used for handling dependencies

if(NOT COA_FETCH_DEPS)
  return()
endif()

message(STATUS "Building dependencies from source")

include(FetchContent)

FetchContent_Declare(
  sup-mathexpr
  GIT_REPOSITORY https://github.com/oac-tree/sup-mathexpr.git
  GIT_TAG develop
  OVERRIDE_FIND_PACKAGE
)
