# ------------------------------------------------------------------------------
# General CMake settings
# ------------------------------------------------------------------------------
cmake_minimum_required(VERSION 3.14 FATAL_ERROR)
cmake_policy(SET CMP0074 NEW) # force find_package to take <PackageName>_ROOT variables into account

if(NOT DEFINED PROJECT_ROOT)
  set(PROJECT_ROOT ${CMAKE_SOURCE_DIR} CACHE PATH "Root directory, where the submodules are populated")
endif()

# Bootstrap project_utils
set(sub_repo_path "${PROJECT_ROOT}/external/project_utils")
file(GLOB sub_repo_files ${sub_repo_path}/*)
list(LENGTH sub_repo_files sub_repo_nb_files)
if(sub_repo_nb_files EQUAL 0)
  message(FATAL_ERROR
    "${sub_repo_path} is empty.
    Maybe you forgot to initialize it with \"git submodule update --init\""
  )
endif()
set(PROJECT_UTILS_DIR ${PROJECT_ROOT}/external/project_utils)
set(PROJECT_UTILS_CMAKE_DIR ${PROJECT_UTILS_DIR}/cmake)
list(APPEND CMAKE_MODULE_PATH "${PROJECT_UTILS_CMAKE_DIR}/find_package") # for custom Find*.cmake files
include(${PROJECT_UTILS_CMAKE_DIR}/dependency_management.cmake) # project_add_subdirectory, project_find_package and target_install

set(CMAKE_INSTALL_RPATH_USE_LINK_PATH TRUE)
set(CMAKE_INSTALL_RPATH "${CMAKE_INSTALL_PREFIX}/lib")


# ------------------------------------------------------------------------------
# Project
# ------------------------------------------------------------------------------
project(
  std_e VERSION 0.1.0
  DESCRIPTION "C++ standard library extension"
  LANGUAGES CXX
)

option(${PROJECT_NAME}_ENABLE_MPI "Enable mpi for ${PROJECT_NAME}" OFF)
option(${PROJECT_NAME}_ENABLE_CPP20 "Enable c++20 for ${PROJECT_NAME}" OFF)
option(${PROJECT_NAME}_ENABLE_TEST "Enable testing for ${PROJECT_NAME}" ON)
option(${PROJECT_NAME}_ENABLE_COVERAGE "Enable coverage for ${PROJECT_NAME}" OFF)
option(${PROJECT_NAME}_ENABLE_DOCUMENTATION "Build ${PROJECT_NAME} documentation" OFF)
option(${PROJECT_NAME}_ENABLE_PERFORMANCE_TESTS "Enable performance tests for ${PROJECT_NAME}" OFF)

## Compiler flags
### Build type
if (NOT CMAKE_BUILD_TYPE)
  set(CMAKE_BUILD_TYPE Release CACHE STRING "Build type" FORCE) # default to Release
endif()
### C++ standard
if (${PROJECT_NAME}_ENABLE_CPP20)
  set(CMAKE_CXX_STANDARD 20)
else()
  set(CMAKE_CXX_STANDARD 17)
endif()

set(CMAKE_CXX_EXTENSIONS OFF)
set(CMAKE_CXX_STANDARD_REQUIRED ON )
### fPIC
set(CMAKE_POSITION_INDEPENDENT_CODE ON)


# ------------------------------------------------------------------------------
# Dependencies
# ------------------------------------------------------------------------------
include(FetchContent)
### doctest ###
FetchContent_Declare(
  doctest
  GIT_REPOSITORY https://github.com/doctest/doctest.git
  GIT_TAG        110d305 # corrected MPI SUBCASE
)
FetchContent_MakeAvailable(doctest)


# ------------------------------------------------------------------------------
# Compilation
# ------------------------------------------------------------------------------
## Find files ##
set(src_dir ${PROJECT_SOURCE_DIR}/${PROJECT_NAME})
set(include_dir ${PROJECT_SOURCE_DIR})

file(GLOB_RECURSE cpp_and_test_files
  CONFIGURE_DEPENDS
  "${src_dir}/*.cpp"
)

set(cpp_files ${cpp_and_test_files})
list(FILTER cpp_files EXCLUDE REGEX ".*\\.test\\.cpp$")

set(test_files ${cpp_and_test_files})
list(FILTER test_files INCLUDE REGEX ".*\\.test\\.cpp$")

if(${PROJECT_NAME}_ENABLE_MPI)
  list(FILTER cpp_files EXCLUDE REGEX ".*\\.nompi\\.cpp$")
  list(FILTER test_files EXCLUDE REGEX ".*\\.nompi\\.test\\.cpp$")
else()
  list(FILTER cpp_files EXCLUDE REGEX ".*\\.mpi\\.cpp$")
  list(FILTER test_files EXCLUDE REGEX ".*\\.mpi\\.test\\.cpp$")
endif()

## Main library ##
add_library(${PROJECT_NAME} SHARED ${cpp_files})
target_include_directories(${PROJECT_NAME} PUBLIC
  $<BUILD_INTERFACE:${include_dir}>
  $<INSTALL_INTERFACE:include/${PROJECT_NAME}>
)

if(${PROJECT_NAME}_ENABLE_MPI)
  project_find_package(MPI REQUIRED COMPONENTS CXX)
  target_link_libraries(${PROJECT_NAME}
    PUBLIC
      MPI::MPI_CXX
  )
endif()


# ------------------------------------------------------------------------------
# Install
# ------------------------------------------------------------------------------
target_install(${PROJECT_NAME})


# ------------------------------------------------------------------------------
# Testing
# ------------------------------------------------------------------------------
include(CTest)
add_subdirectory(test)


# ------------------------------------------------------------------------------
# Build documentation
# ------------------------------------------------------------------------------
if(${PROJECT_NAME}_ENABLE_DOCUMENTATION)
  include(${PROJECT_UTILS_CMAKE_DIR}/build_documentation.cmake)
  build_documentation()
endif()


# ------------------------------------------------------------------------------
# Coverage TODO clean
# ------------------------------------------------------------------------------
if(${PROJECT_NAME}_ENABLE_COVERAGE)
  # TODO -> Make a correct exclude directory
  # NOTE : not working with gcc-9.2
  include(CodeCoverage)
  append_coverage_compiler_flags()

  # set(COVERAGE_EXCLUDES "/opt/tools/gcc-8.3/include/c++/8.3.0/ext"
  #                       "/opt/tools/gcc-8.3/include/c++/8.3.0/*"
  #                       "/opt/tools/gcc-8.3/include/c++/8.3.0/bits/*")
  set(COVERAGE_INCLUDES "${PROJECT_SOURCE_DIR}/src/*"
                        "${PROJECT_SOURCE_DIR}/external/*")

  setup_target_for_coverage_lcov(
    NAME ${PROJECT_NAME}_coverage
    DEPENDENCIES ${PROJECT_NAME}_unit_tests
    EXECUTABLE ctest -j ${n_cores} # Executable in PROJECT_BINARY_DIR
  )
endif()


# ------------------------------------------------------------------------------
# Summary
# ------------------------------------------------------------------------------
get_directory_property(is_top_lvl_cmakelists PARENT_DIRECTORY)
if(${CMAKE_CURRENT_SOURCE_DIR} STREQUAL ${CMAKE_SOURCE_DIR})
  include(FeatureSummary)
  feature_summary(WHAT ALL)
endif()
