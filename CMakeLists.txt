## Global config ##
cmake_minimum_required(VERSION 3.12)

if(NOT DEFINED PROJECT_ROOT)
  set(PROJECT_ROOT ${CMAKE_SOURCE_DIR} CACHE PATH "Root directory, where the submodules are populated")
endif()

macro(check_sub_repo_exists sub_repo_name)
  set(sub_repo_path "${PROJECT_ROOT}/external/${sub_repo_name}")
  file(GLOB sub_repo_files ${sub_repo_path}/*)
  list(LENGTH sub_repo_files sub_repo_nb_files)
  if(sub_repo_nb_files EQUAL 0)
    message(FATAL_ERROR
      "${PROJECT_ROOT}/external/${sub_repo_name} is empty.
      Maybe you forgot to initialize it with \"git submodule update --init\""
    )
  endif()
endmacro()

check_sub_repo_exists(project_utils)
set(PROJECT_UTILS_DIR ${PROJECT_ROOT}/external/project_utils)
set(PROJECT_UTILS_CMAKE_DIR ${PROJECT_UTILS_DIR}/scripts/cmake)
include(${PROJECT_UTILS_CMAKE_DIR}/target_add_dependency.cmake) # target_add_dependency, target_add_thirdparty_dependency and target_install commands

set(CMAKE_INSTALL_RPATH_USE_LINK_PATH TRUE)
set(CMAKE_INSTALL_RPATH "${CMAKE_INSTALL_PREFIX}/lib")

## Project ##
project(
  std_e VERSION 0.1.0
  DESCRIPTION "std extended"
  LANGUAGES CXX
)

if (NOT DEFINED STD_E_ENABLE_MPI)
  option(STD_E_ENABLE_MPI "Enable mpi for ${PROJECT_NAME}" OFF)
endif()

## Compiler flags
### Build type
if(NOT CMAKE_BUILD_TYPE)
  set(CMAKE_BUILD_TYPE Release CACHE STRING "Build type" FORCE) # default to Release
endif()
### C++ standard
set(CMAKE_CXX_STANDARD          17 )
set(CMAKE_CXX_EXTENSIONS        OFF)
set(CMAKE_CXX_STANDARD_REQUIRED ON )
### fPIC
set(CMAKE_POSITION_INDEPENDENT_CODE ON)

## Dependencies ##
include(FetchContent)
### doctest ###
FetchContent_Declare(
  doctest
  #GIT_REPOSITORY https://github.com/onqtam/doctest.git
  GIT_REPOSITORY https://github.com/BerengerBerthoul/doctest.git
  GIT_TAG        f13a00607463ce211004be23b5aa0bdf24d3ab61
  #GIT_TAG        mpi
)
FetchContent_GetProperties(doctest)
if(NOT doctest_POPULATED)
  FetchContent_Populate(doctest)
  add_subdirectory(${doctest_SOURCE_DIR} ${doctest_BINARY_DIR})
endif()

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

if(STD_E_ENABLE_MPI)
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

if(STD_E_ENABLE_MPI)
  target_add_thirdparty_dependency(${PROJECT_NAME} MPI REQUIRED COMPONENTS CXX)
  target_link_libraries(${PROJECT_NAME}
    PUBLIC
      MPI::MPI_CXX
  )
endif()

### Install ###
install(TARGETS ${PROJECT_NAME} EXPORT ${PROJECT_NAME}Targets
  LIBRARY DESTINATION lib
  ARCHIVE DESTINATION lib
  RUNTIME DESTINATION bin
  INCLUDES DESTINATION include
)
install(EXPORT ${PROJECT_NAME}Targets
  FILE ${PROJECT_NAME}Config.cmake # no dependency
  NAMESPACE ${PROJECT_NAME}::
  DESTINATION lib/cmake/${PROJECT_NAME}
)
install(DIRECTORY ${src_dir} DESTINATION include FILES_MATCHING PATTERN "*.hpp")

add_library(${PROJECT_NAME}::${PROJECT_NAME} ALIAS ${PROJECT_NAME})


## Unit tests ##
option(STD_E_ENABLE_TEST "Enable testing for ${PROJECT_NAME}" ON)
if(STD_E_ENABLE_TEST)
  enable_testing()
  include(CTest)
  if(STD_E_ENABLE_MPI)
    set(unit_test_main ${PROJECT_UTILS_DIR}/src/mpi_unit_test_main.cpp)
  else()
    set(unit_test_main ${PROJECT_UTILS_DIR}/src/unit_test_main.cpp)
  endif()
  add_executable(${PROJECT_NAME}_unit_tests
    ${test_files}
    ${unit_test_main}
  )
  target_link_libraries(${PROJECT_NAME}_unit_tests
    PUBLIC
      ${PROJECT_NAME}
    PRIVATE
      doctest::doctest
  )
  if(STD_E_ENABLE_MPI)
    #add_test(NAME ${PROJECT_NAME}_unit_tests COMMAND "mpirun -np 4 ${PROJECT_NAME}_unit_tests")
    set(n_proc 4)
    add_test(
      NAME ${PROJECT_NAME}_unit_tests
      COMMAND ${MPIEXEC} ${MPIEXEC_NUMPROC_FLAG} ${n_proc}
              ${MPIEXEC_PREFLAGS}
              ${CMAKE_CURRENT_BINARY_DIR}/${PROJECT_NAME}_unit_tests
              ${MPIEXEC_POSTFLAGS}
    )
  else()
    add_test(NAME ${PROJECT_NAME}_unit_tests COMMAND ${CMAKE_CURRENT_BINARY_DIR}/${PROJECT_NAME}_unit_tests)
  endif()
endif()

option(STD_E_ENABLE_COVERAGE "Enable coverage for ${PROJECT_NAME}" OFF)
if(STD_E_ENABLE_COVERAGE)
  if(NOT STD_E_ENABLE_TEST)
    message(FATAL_ERROR "You need to enable testing to enable coverage: STD_E_ENABLE_TEST is OFF")
  endif()
  # TODO -> Make a correct exclude directory
  # NOTE : not working with gcc-9.2
  include(CodeCoverage)
  APPEND_COVERAGE_COMPILER_FLAGS()

  # set(COVERAGE_EXCLUDES "/opt/tools/gcc-8.3/include/c++/8.3.0/ext"
  #                       "/opt/tools/gcc-8.3/include/c++/8.3.0/*"
  #                       "/opt/tools/gcc-8.3/include/c++/8.3.0/bits/*")
  set(COVERAGE_INCLUDES "${PROJECT_SOURCE_DIR}/src/*"
                        "${PROJECT_SOURCE_DIR}/external/*")

  SETUP_TARGET_FOR_COVERAGE_LCOV(
    NAME ${PROJECT_NAME}_coverage
    DEPENDENCIES ${PROJECT_NAME}_unit_tests
    EXECUTABLE ctest -j ${n_cores} # Executable in PROJECT_BINARY_DIR
  )
endif()
# --------------------------------------------------------------------------------

# ----------------------------------------------------------------------
option(STD_E_BUILD_DOCUMENTATION "Build ${PROJECT_NAME} documentation" OFF)
if(STD_E_BUILD_DOCUMENTATION)
  # Download bootstrap style
  FetchContent_Declare(
    sphinx_bootstrap_theme
    GIT_REPOSITORY https://github.com/ryan-roemer/sphinx-bootstrap-theme.git
    GIT_TAG        v0.8.0
  )
  FetchContent_GetProperties(sphinx_bootstrap_theme)
  if(NOT sphinx_bootstrap_theme_POPULATED)
    FetchContent_Populate(sphinx_bootstrap_theme)
  endif()

  include(UseBreathe)
  add_breathe_doc(TARGET_NAME ${PROJECT_NAME}_doc
    SOURCE_DIR ${CMAKE_CURRENT_SOURCE_DIR}/doc
    DOXY_FILE  ${CMAKE_CURRENT_SOURCE_DIR}/doc/Doxyfile.in
    CONF_FILE  ${CMAKE_CURRENT_SOURCE_DIR}/doc/conf.py.in
    BUILD_DIR  ${CMAKE_CURRENT_BINARY_DIR}/_build
    CACHE_DIR  ${CMAKE_CURRENT_BINARY_DIR}/_doctrees
    HTML_DIR   ${CMAKE_CURRENT_BINARY_DIR}/html
    COMMENT "HTML documentation"
  )

  install(DIRECTORY   ${CMAKE_CURRENT_BINARY_DIR}/html
          DESTINATION ${CMAKE_INSTALL_PREFIX}/share/doc)
endif()
# ----------------------------------------------------------------------

# ----------------------------------------------------------------------
include(FeatureSummary)
FEATURE_SUMMARY(WHAT ALL)
# ----------------------------------------------------------------------
