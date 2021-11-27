#include "std_e/unit_test/doctest_mpi.hpp"

#include "std_e/parallel/mpi/base.hpp"
#include "std_e/parallel/struct/distribution.hpp"
#include "std_e/future/contract.hpp"
#include <thread>

using namespace std_e;
using namespace std::string_literals;
using namespace std::chrono_literals;


MPI_TEST_CASE("MPI_Win passive win_lock - multiple compute nodes",48) {
  int alloc_method = 0; // MPI_Win_allocate
  //int alloc_method = 1; // MPI_Win_create + MPI_Alloc_mem
  //int alloc_method = 2; // MPI_Win_create + malloc

  MPI_Comm comm = test_comm;
  int n_rank = std_e::n_rank(comm);
  int i_rank = rank(comm);

  int dn_value = 2;

  int* test_status;
  MPI_Win win;

  if(alloc_method == 0){
    int err_create = MPI_Win_allocate(dn_value * sizeof(int),
                                      sizeof(int),
                                      MPI_INFO_NULL,
                                      comm,
                                      &test_status,
                                      &win);
    STD_E_ASSERT(err_create == MPI_SUCCESS);
  } else if(alloc_method == 1) {
    test_status = (int*) malloc( dn_value * sizeof(int));
    int err_create = MPI_Win_create(test_status,              /* base ptr */
                                    dn_value*sizeof(int),              /* size     */
                                    sizeof(int),                      /* disp_unit*/
                                    MPI_INFO_NULL,
                                    comm,
                                    &win);
    STD_E_ASSERT(err_create == MPI_SUCCESS);
  } else if(alloc_method == 2) {
    int err_alloc = MPI_Alloc_mem(dn_value*sizeof(int), MPI_INFO_NULL, &test_status);
    STD_E_ASSERT(err_alloc == 0);
    int err_create = MPI_Win_create(test_status,              /* base ptr */
                                    dn_value*sizeof(int),              /* size     */
                                    sizeof(int),                      /* disp_unit*/
                                    MPI_INFO_NULL,
                                    comm,
                                    &win);
    STD_E_ASSERT(err_create == MPI_SUCCESS);
  } else if(alloc_method == 3) {
    int err_alloc = MPI_Alloc_mem(20*dn_value*sizeof(int), MPI_INFO_NULL, &test_status);
    STD_E_ASSERT(err_alloc == 0);
    int err_create = MPI_Win_create_dynamic(MPI_INFO_NULL, comm, &win);
    STD_E_ASSERT(err_create == MPI_SUCCESS);
    int err_attach = MPI_Win_attach(win, test_status, dn_value*sizeof(int));
    STD_E_ASSERT(err_attach == MPI_SUCCESS);
  } else {
    abort();
  }

  if (i_rank == 4) {
    std::this_thread::sleep_for(1s);
  }
  if (i_rank == 7) {
    std::this_thread::sleep_for(2s);
  }

  int err = MPI_Win_lock(MPI_LOCK_EXCLUSIVE, i_rank, MPI_MODE_NOCHECK, win);
  STD_E_ASSERT(err == 0);
  //printf(" test_status : %p \n", test_status);
  int sgn = 1;
  for(int i = 0; i < dn_value; ++i){
    test_status[i] = i_rank * sgn;
    sgn *= -1;
  }
  err = MPI_Win_unlock(i_rank, win);
  STD_E_ASSERT(err == 0);
  MPI_Barrier(comm); // This one is mandatoray

  std::vector<int> check_val(dn_value * n_rank, -10000);


  /*
   *  All rank put
   */
  err = MPI_Win_lock_all(MPI_MODE_NOCHECK, win);
  STD_E_ASSERT(err == 0);

  for(int i = 0; i < n_rank; ++i) {
    int err_get = MPI_Get(check_val.data()+dn_value*i,     /* origin_addr     */
                          dn_value,                          /* origin_count    */
                          MPI_INT,                    /* origin_datatype */
                          i,                          /* target_rank     */
                          0,                          /* target_disp     */
                          dn_value,                   /* target_count    */
                          MPI_INT,                    /* target_datatype */
                          win);                       /* win             */
    STD_E_ASSERT(err_get == 0);
  }

  // MPI_Win_flush_all(win);
  err = MPI_Win_unlock_all(win);
  //MPI_Barrier(comm);
  STD_E_ASSERT(err == 0);

  //ELOG(check_val);

  std::vector<int> expected_val(dn_value * n_rank);
  for(int i = 0; i < n_rank; ++i) {
    sgn = 1;
    for(int j = 0; j < dn_value; ++j) {
      expected_val[dn_value*i+j] = i*sgn;
      sgn *= -1;
    }
  }

  CHECK( check_val == expected_val );

  if(alloc_method == 1) {
    free(test_status);
  }
  if(alloc_method == 2) {
    MPI_Free_mem(test_status);
  }
  if(alloc_method == 3) {
    int err_detach = MPI_Win_detach(win, test_status);
    STD_E_ASSERT(err_detach == 0);
  }


  int err_free = MPI_Win_free(&win);
  STD_E_ASSERT(err_free == MPI_SUCCESS);
}
