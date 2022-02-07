include(${PROJECT_UTILS_CMAKE_DIR}/doctest_pytest.cmake)

set(n_proc_max_test 4)
set(doctest_args -tse=mpi_internode) #Exclude internode test from ctest

if(${PROJECT_NAME}_ENABLE_MPI)
  set(unit_test_main ${PROJECT_UTILS_DIR}/src/mpi_unit_test_main.cpp)
  set(serial_run "false")
else()
  set(unit_test_main ${PROJECT_UTILS_DIR}/src/unit_test_main.cpp)
  set(serial_run "true")
endif()

file(GLOB_RECURSE unit_test_files
  CONFIGURE_DEPENDS "${src_dir}/*.test.cpp"
)
create_doctest(
  TESTED_TARGET ${PROJECT_NAME}
  LABEL         unit_tests
  SOURCES       ${unit_test_main} ${unit_test_files}
  SERIAL_RUN    ${serial_run}
  N_PROC        ${n_proc_max_test}
  DOCTEST_ARGS  ${doctest_args}
)

file(GLOB_RECURSE perfo_test_files
  CONFIGURE_DEPENDS "${PROJECT_SOURCE_DIR}/test/std_e_perfo/*.test.cpp"
)
create_doctest(
  TESTED_TARGET ${PROJECT_NAME}
  LABEL         perfo
  SOURCES       ${unit_test_main} ${perfo_test_files}
  SERIAL_RUN    ${serial_run}
  N_PROC        ${n_proc_max_test}
)
