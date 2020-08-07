#pragma once

#ifdef DOCTEST_CONFIG_IMPLEMENT

#include "doctest/doctest.h"
#include "std_e/unit_test/mpi/mpi_reporter.hpp"

#else

#include <numeric>
#include <vector>
#include "doctest/doctest.h"
#include "mpi.h"
#include <iostream> // TODO

template<int nb_procs>
struct mpi_test_fixture {
  static constexpr int test_nb_procs = nb_procs;
  int test_rank;
  MPI_Comm test_comm;

  mpi_test_fixture() noexcept {
    int n_rank;
    MPI_Comm_size(MPI_COMM_WORLD, &n_rank);
    if (n_rank<test_nb_procs) {
      std::cout << "TEST SKIPPED BECAUSE NOT ENOUGHT MPI PROCS FOR IT\n"; // TODO doctest report
      test_rank = -1;
      test_comm = MPI_COMM_NULL;
    } else {
      MPI_Group world_group;
      MPI_Comm_group(MPI_COMM_WORLD, &world_group);

      // Prepare array to create the group that include only processes [0, test_nb_procs)
      std::vector<int> test_procs(test_nb_procs);
      std::iota(begin(test_procs), end(test_procs), 0);

      // Create sub_group and sub_comm
      MPI_Group test_group;
      MPI_Group_incl(world_group, test_nb_procs, test_procs.data(), &test_group);
      MPI_Comm_create_group(MPI_COMM_WORLD, test_group, 0, &test_comm);
      // If not in test_group we have test_comm==MPI_COMM_NULL

      // We need test_rank: our rank for test_comm
      if(test_comm != MPI_COMM_NULL){
        MPI_Comm_rank(test_comm, &test_rank);
      } else {
        test_rank = -1;
      }

      MPI_Group_free(&world_group);
      MPI_Group_free(&test_group);
    }
  }

  ~mpi_test_fixture() {
    if(test_comm != MPI_COMM_NULL){
      MPI_Comm_free(&test_comm);
    }
  }
};


#define DOCTEST_MPI_CHECK(rank_to_test, ...)  \
  if(rank_to_test == test_rank) { DOCTEST_ASSERT_IMPLEMENT_1(DT_CHECK, __VA_ARGS__); }

// short name
#define MPI_CHECK DOCTEST_MPI_CHECK


#define RETURN_IF_COMM_NULL \
  if(test_comm == MPI_COMM_NULL) return;


#define MPI_TEST_CASE(name,nb_procs,code) \
  TEST_CASE_FIXTURE(mpi_test_fixture<nb_procs>, name) { \
    RETURN_IF_COMM_NULL \
    code \
  }

#endif // DOCTEST_CONFIG_IMPLEMENT
