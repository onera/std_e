#pragma once

#include "std_e/unit_test/doctest_pybind.hpp"
#include "std_e/unit_test/doctest_mpi.hpp"

#define CREATE_PYBIND_MPI_TEST_CASE(name,nb_procs,func) \
  static void func(DOCTEST_UNUSED int test_rank, DOCTEST_UNUSED int test_nb_procs, DOCTEST_UNUSED MPI_Comm test_comm, DOCTEST_UNUSED std::integral_constant<int,nb_procs>); \
  TEST_CASE(name * doctest::description("PYBIND_MPI_TEST_CASE")) { \
    std_e::throw_if_no_python_interpreter(std::string("PYBIND_MPI_TEST_CASE(\"")+name+"\")"); \
    doctest::execute_mpi_test_case<nb_procs>(func); \
  } \
  static void func(DOCTEST_UNUSED int test_rank, DOCTEST_UNUSED int test_nb_procs, DOCTEST_UNUSED MPI_Comm test_comm, DOCTEST_UNUSED std::integral_constant<int,nb_procs> test_nb_procs_as_int_constant)
  // DOC: test_rank, test_nb_procs, and test_comm are available UNDER THESE SPECIFIC NAMES in the body of the unit test
  // DOC: test_nb_procs_as_int_constant is equal to test_nb_procs, but as a compile time value
  //          (used in CHECK-like macros to assert the checked rank exists)

#define PYBIND_MPI_TEST_CASE(name,n_proc) \
  CREATE_PYBIND_MPI_TEST_CASE(name,n_proc,DOCTEST_ANONYMOUS(DOCTEST_PYBIND_MPI_FUNC))
