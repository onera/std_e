## Global config ##
cmake_minimum_required(VERSION 3.12)

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
  graph VERSION 0.1.0
  DESCRIPTION	"graph structures and algorithms"
  LANGUAGES	CXX
)


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
target_add_dependency(${PROJECT_NAME} std_e)
target_link_libraries(${PROJECT_NAME}
  INTERFACE
    std_e::std_e
)


### Install ###
install(TARGETS ${PROJECT_NAME} EXPORT ${PROJECT_NAME}Targets
  LIBRARY DESTINATION lib
  ARCHIVE DESTINATION lib
  RUNTIME DESTINATION bin
  INCLUDES DESTINATION include
)
install(EXPORT ${PROJECT_NAME}Targets
  FILE ${PROJECT_NAME}Targets.cmake
  NAMESPACE ${PROJECT_NAME}::
  DESTINATION lib/cmake/${PROJECT_NAME}
)
install(DIRECTORY ${src_dir} DESTINATION include FILES_MATCHING PATTERN "*.hpp")

set(TARGET_NAME ${PROJECT_NAME})
string(REPLACE ";" " " TARGET_DEPENDENCIES_FIND_PACKAGE_STRING "${${PROJECT_NAME}_DEPENDENCIES_FIND_PACKAGE_STRING}")
configure_file(
  ${git_root_dir}/external/project_utils/scripts/cmake/target_config.cmake.in
  ${PROJECT_NAME}Config.cmake
  @ONLY
)
install(FILES "${CMAKE_CURRENT_BINARY_DIR}/${PROJECT_NAME}Config.cmake"
  DESTINATION lib/cmake/${PROJECT_NAME}
)

add_library(${PROJECT_NAME}::${PROJECT_NAME} ALIAS ${PROJECT_NAME})


## Unit tests ##
# TODO main.test.cpp
target_add_dependency(${PROJECT_NAME}_unit_tests doctest)
add_executable(${PROJECT_NAME}_unit_tests ${test_files})
target_link_libraries(${PROJECT_NAME}_unit_tests PUBLIC ${PROJECT_NAME})
target_link_libraries(${PROJECT_NAME}_unit_tests PRIVATE doctest::doctest)
