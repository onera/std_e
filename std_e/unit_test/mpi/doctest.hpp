#pragma once

#ifdef DOCTEST_CONFIG_IMPLEMENT

#include "doctest/doctest.h"
#include "std_e/unit_test/mpi/mpi_reporter.hpp"

#else

#include "mpi.h"
#include <numeric>
#include <vector>
#include "doctest/doctest.h"
#include <cassert>
#include <iostream> // TODO

namespace doctest {

inline
int mpi_world_nb_procs() {
  int n;
  MPI_Comm_size(MPI_COMM_WORLD, &n);
  return n;
}

struct mpi_sub_comm {
  int nb_procs;
  int rank;
  MPI_Comm comm;

  mpi_sub_comm(int nb_procs) noexcept
    : nb_procs(nb_procs)
    , rank(-1)
    , comm(MPI_COMM_NULL)
  {
    if (nb_procs>mpi_world_nb_procs()) {
      FAIL_CHECK("Unable to run test: need "+std::to_string(nb_procs) + " procs"
                +" but program launched with only "+std::to_string(doctest::mpi_world_nb_procs()) + ".");
    } else {
      MPI_Group world_group;
      MPI_Comm_group(MPI_COMM_WORLD, &world_group);

      // Prepare array to create the group that include only processes [0, nb_procs)
      std::vector<int> test_procs(nb_procs);
      std::iota(begin(test_procs), end(test_procs), 0);

      // Create sub_group and sub_comm
      MPI_Group test_group;
      MPI_Group_incl(world_group, nb_procs, test_procs.data(), &test_group);
      MPI_Comm_create_group(MPI_COMM_WORLD, test_group, 0, &comm);
      // If not in test_group we have comm==MPI_COMM_NULL

      // Get rank of the process
      if(comm != MPI_COMM_NULL){
        MPI_Comm_rank(comm, &rank);
        int comm_world_rank;
        MPI_Comm_rank(MPI_COMM_WORLD, &comm_world_rank);
        assert(rank==comm_world_rank);
      }

      MPI_Group_free(&world_group);
      MPI_Group_free(&test_group);
    }
  }

  ~mpi_sub_comm() {
    if(comm != MPI_COMM_NULL){
      MPI_Comm_free(&comm);
    }
  }
};


template<class F>
void execute_mpi_test_case(F func, int nb_procs) {
  mpi_sub_comm sub(nb_procs);
  if (sub.comm != MPI_COMM_NULL) {
    func(sub.rank,sub.nb_procs,sub.comm);
  };
}

} // doctest

#define DOCTEST_MPI_WARN(rank_to_test, ...)  if(rank_to_test == test_rank) DOCTEST_WARN(__VA_ARGS__)
#define DOCTEST_MPI_CHECK(rank_to_test, ...)  if(rank_to_test == test_rank) DOCTEST_CHECK(__VA_ARGS__)
#define DOCTEST_MPI_REQUIRE(rank_to_test, ...)  if(rank_to_test == test_rank) DOCTEST_REQUIRE(__VA_ARGS__)
#define DOCTEST_MPI_WARN_FALSE(rank_to_test, ...)  if(rank_to_test == test_rank) DOCTEST_WARN_FALSE   (__VA_ARGS__)
#define DOCTEST_MPI_CHECK_FALSE(rank_to_test, ...)  if(rank_to_test == test_rank) DOCTEST_CHECK_FALSE  (__VA_ARGS__)
#define DOCTEST_MPI_REQUIRE_FALSE(rank_to_test, ...)  if(rank_to_test == test_rank) DOCTEST_REQUIRE_FALSE(__VA_ARGS__)

#define DOCTEST_CREATE_MPI_TEST_CASE(name,nb_procs,func) \
  void func(int test_rank, int test_nb_procs, MPI_Comm test_comm); \
  TEST_CASE(name) { \
    doctest::execute_mpi_test_case(func,nb_procs); \
  } \
  void func(int test_rank, int test_nb_procs, MPI_Comm test_comm)
  // DOC: test_rank, test_nb_procs, and test_comm are available UNDER THESE SPECIFIC NAMES in the body of the unit test

#define DOCTEST_MPI_TEST_CASE(name,nb_procs) \
  DOCTEST_CREATE_MPI_TEST_CASE(name,nb_procs,DOCTEST_ANONYMOUS(DOCTEST_MPI_FUNC))


// == SHORT VERSIONS OF THE MACROS
#if !defined(DOCTEST_CONFIG_NO_SHORT_MACRO_NAMES)
#define MPI_WARN           DOCTEST_MPI_WARN
#define MPI_CHECK          DOCTEST_MPI_CHECK
#define MPI_REQUIRE        DOCTEST_MPI_REQUIRE
#define MPI_WARN_FALSE     DOCTEST_MPI_WARN_FALSE
#define MPI_CHECK_FALSE    DOCTEST_MPI_CHECK_FALSE
#define MPI_REQUIRE_FALSE  DOCTEST_MPI_REQUIRE_FALSE

#define MPI_TEST_CASE      DOCTEST_MPI_TEST_CASE
#endif // DOCTEST_CONFIG_NO_SHORT_MACRO_NAMES


#endif // DOCTEST_CONFIG_IMPLEMENT
