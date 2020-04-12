## Global config ##
cmake_minimum_required(VERSION 3.12)

find_package(Git 2.14 REQUIRED)
execute_process(COMMAND git rev-parse --show-superproject-working-tree
  WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}
  OUTPUT_VARIABLE git_root_dir_with_space
)
string(STRIP "${git_root_dir_with_space}" git_root_dir)
if("${git_root_dir}" STREQUAL "")
  set(git_root_dir ".")
endif()
include(${git_root_dir}/external/project_utils/scripts/cmake/target_add_dependency.cmake)

set(CMAKE_INSTALL_RPATH_USE_LINK_PATH TRUE)
set(CMAKE_INSTALL_RPATH "${CMAKE_INSTALL_PREFIX}/lib")

## Project ##
project(
  std_e VERSION 0.1.0
  DESCRIPTION	"std extended"
  LANGUAGES	CXX
)


## Dependencies ##
include(FetchContent)
### doctest ###
FetchContent_Declare(
  doctest
  GIT_REPOSITORY https://github.com/onqtam/doctest.git
  GIT_TAG        2.3.7
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
  "${src_dir}/*.cpp"
)
set(cpp_files ${cpp_and_test_files})
list(FILTER cpp_files EXCLUDE REGEX ".*\.test\.cpp$")
set(test_files ${cpp_and_test_files})
list(FILTER test_files INCLUDE REGEX ".*\.test\.cpp$")


## Compiler flags ##
# TODO move
add_compile_options(-Wall -fmax-errors=2)
set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} -O3 -fno-math-errno -march=native -DNDEBUG -fopenmp -fPIC")


## Main library ##
add_library(${PROJECT_NAME} INTERFACE)
target_compile_features(${PROJECT_NAME} INTERFACE cxx_std_17)
target_include_directories(${PROJECT_NAME} INTERFACE 
  $<BUILD_INTERFACE:${include_dir}>
  $<INSTALL_INTERFACE:include/${PROJECT_NAME}>
)

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
# TODO main.test.cpp
add_executable(${PROJECT_NAME}_unit_tests ${test_files})
target_link_libraries(${PROJECT_NAME}_unit_tests PUBLIC ${PROJECT_NAME})
target_link_libraries(${PROJECT_NAME}_unit_tests PRIVATE doctest::doctest)
