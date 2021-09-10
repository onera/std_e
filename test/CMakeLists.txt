include(${PROJECT_UTILS_CMAKE_DIR}/doctest_pytest.cmake)

set(n_proc_max_test 4)

if(${PROJECT_NAME}_ENABLE_MPI)
  set(unit_test_main ${PROJECT_UTILS_DIR}/src/mpi_unit_test_main.cpp)
  set(serial_run "false")
else()
  set(unit_test_main ${PROJECT_UTILS_DIR}/src/unit_test_main.cpp)
  set(serial_run "true")
endif()

file(GLOB_RECURSE test_files
  CONFIGURE_DEPENDS "${src_dir}/*.test.cpp"
)

create_doctest(
  TESTED_TARGET ${PROJECT_NAME}
  LABEL         unit_tests
  SOURCES       ${unit_test_main} ${test_files}
  #SOURCES       ${test_files}
  SERIAL_RUN    ${serial_run}
  N_PROC        ${n_proc_max_test}
)
