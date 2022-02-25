#pragma once

#include "std_e/unit_test/doctest_pybind.hpp"
#include "std_e/unit_test/doctest_mpi.hpp"

// Note: This is a copy-paste of what is in doctest, the only modified part is that we throw if we don't find a python interpreter
// WARNING: the description has to be "MPI_TEST_CASE"
//          because the doctest MPI console reporter searches this exact string in the description

#define CREATE_PYBIND_MPI_TEST_CASE(name,nb_procs,func) \
  static void func(DOCTEST_UNUSED int test_rank, DOCTEST_UNUSED int test_nb_procs, DOCTEST_UNUSED MPI_Comm test_comm, DOCTEST_UNUSED std::integral_constant<int,nb_procs>); \
  TEST_CASE(name * doctest::description("MPI_TEST_CASE")) { \
    std_e::throw_if_no_python_interpreter(std::string("MPI_TEST_CASE(\"")+name+"\")"); \
    doctest::execute_mpi_test_case<nb_procs>(func); \
  } \
  static void func(DOCTEST_UNUSED int test_rank, DOCTEST_UNUSED int test_nb_procs, DOCTEST_UNUSED MPI_Comm test_comm, DOCTEST_UNUSED std::integral_constant<int,nb_procs> test_nb_procs_as_int_constant)

#define PYBIND_MPI_TEST_CASE(name,n_proc) \
  CREATE_PYBIND_MPI_TEST_CASE(name,n_proc,DOCTEST_ANONYMOUS(DOCTEST_PYBIND_MPI_FUNC))
