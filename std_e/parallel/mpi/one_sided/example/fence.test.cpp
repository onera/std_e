#include "std_e/unit_test/doctest_mpi.hpp"

#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <algorithm>
#include <numeric>
#include <iostream>
#include <functional>
#include <unistd.h>
#include <chrono>
#include <sys/shm.h>
#include <sys/mman.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <stdio.h>
#include <stdlib.h>
#include "std_e/parallel/mpi/base.hpp"
#include "std_e/logging/time_logger.hpp"
#include "std_e/parallel/struct/distribution.hpp"


using namespace std_e;

TEST_SUITE_BEGIN("mpi_internode");
MPI_TEST_CASE("MPI_Win fence - multiple compute nodes",48) {
  int n_rank = test_nb_procs;

  g_num dn_elt = 2;
  auto distri = uniform_distribution(n_rank,n_rank*dn_elt);

  int err;
  int assertion = 0;

  // init res vector
  int n = distri.length();
  std::vector<int> complete_array(n);

  // window
  using T = int;
  T* ptr;
  int type_sz = sizeof(T);
  MPI_Win win;

  err = MPI_Win_allocate(dn_elt*type_sz,type_sz,MPI_INFO_NULL,test_comm,&ptr,&win);
  STD_E_ASSERT(err == MPI_SUCCESS);

  err = MPI_Win_fence(assertion,win);
  STD_E_ASSERT(err == MPI_SUCCESS);

  // init
  int rk = rank(test_comm);
  std::iota(ptr,ptr+dn_elt,rk*dn_elt);

  err = MPI_Win_fence(assertion,win);
  STD_E_ASSERT(err == MPI_SUCCESS);

  // retrieve
  for (int i=0; i<n; ++i) {
    auto [rank,offset] = rank_offset(i,distri);
    //complete_array[i] = get<int>(win,rank,offset);
    MPI_Get(
      complete_array.data()+i             ,1,MPI_INT, // origin args
                               rank,offset,1,MPI_INT, // target args
      win
    );
  }

  err = MPI_Win_fence(assertion,win);
  STD_E_ASSERT(err == MPI_SUCCESS);

  err = MPI_Win_free(&win);
  STD_E_ASSERT(err == MPI_SUCCESS);

  std::vector<int> expected_complete_array(n_rank*dn_elt);
  std::iota(expected_complete_array.begin(),expected_complete_array.end(),0);
  CHECK(complete_array == expected_complete_array);
}
TEST_SUITE_END();
