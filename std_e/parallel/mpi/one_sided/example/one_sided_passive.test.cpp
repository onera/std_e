#include "std_e/unit_test/doctest_mpi.hpp"

#include "std_e/parallel/mpi/base.hpp"
#include "std_e/logging/time_logger.hpp"
#include "std_e/parallel/struct/distribution.hpp"
#include "std_e/log.hpp"

using namespace std_e;


MPI_TEST_CASE("MPI_Win passive mode - multiple compute nodes",48) {
  //auto logger = &std_e::get_logger("terminal");
  auto logger = &std_e::get_logger("file");

  int rk = rank(test_comm);
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

  //err = MPI_Win_lock_all(assertion,win);
  //err = MPI_Win_unlock_all(win);

  if (rk == 4) {
    sleep(4);
    //std::this_thread::sleep_for(std::chrono::milliseconds(rk*5));
  }
  // init
  std::iota(ptr,ptr+dn_elt,rk*dn_elt);

  // >= 10-20 noeuds
  //0,1,2,3
  //dn_elt = 1000000
  //rank,-rank,rank,-rank

  // barrier
  MPI_Barrier(test_comm);
  //std::this_thread::sleep_for(std::chrono::milliseconds(rk*10));
  //std::this_thread::sleep_for(std::chrono::milliseconds((n_rank-rk)*5));

  // retrieve
  {
    time_logger _(logger,"MPI lock");
    err = MPI_Win_lock_all(assertion,win);
  }
  {
    time_logger _(logger,"MPI get");
    for (int i=0; i<n; ++i) {
      auto [rank,offset] = rank_offset(i,distri);
      //complete_array[i] = get<int>(win,rank,offset);
      MPI_Get(
        complete_array.data()+i             ,1,MPI_INT, // origin args
                                 rank,offset,1,MPI_INT, // target args
        win
      );
    }
  }
  {
    time_logger _(logger,"MPI unlock");
    err = MPI_Win_unlock_all(win);
  }

  err = MPI_Win_free(&win);
  STD_E_ASSERT(err == MPI_SUCCESS);

  //ELOG(complete_array);
  std::vector<int> expected_complete_array(n_rank*dn_elt);
  std::iota(expected_complete_array.begin(),expected_complete_array.end(),0);
  CHECK(complete_array == expected_complete_array);
}

