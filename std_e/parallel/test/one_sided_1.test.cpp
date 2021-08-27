#include "doctest/extensions/doctest_mpi.h"
#include "std_e/parallel/mpi.hpp"

#include "std_e/log.hpp"

//#include <chrono>
//#include <thread>
//std::this_thread::sleep_for(std::chrono::milliseconds(x));
using namespace std_e;

//MPI_TEST_CASE("win alloc",48) {
//
//  int* base_ptr = nullptr;
//  MPI_Win win;
//
//  int sz = 10;
//  int type_sz = sizeof(int);
//  MPI_Win_allocate(sz*type_sz,type_sz,MPI_INFO_NULL,test_comm,&base_ptr,&win);
//
//  //MPI_Info info;
//  //MPI_Win_get_info(win,&info);
//  //MPI_Info_get_string(info,"
//  int* win_mem_model;
//  int flag;
//  MPI_Win_get_attr(win,MPI_WIN_MODEL,&win_mem_model,&flag);
//  if (*win_mem_model==MPI_WIN_UNIFIED) {
//    LOG("MPI_WIN_UNIFIED");
//  } else if (*win_mem_model==MPI_WIN_SEPARATE) {
//    LOG("MPI_WIN_UNIFIED");
//  } else {
//    LOG("ERROR, flag=",flag);
//  }
//
//  //if (rank(test_comm)==0) {
//  //  base_ptr[3] = 4;
//  //  base_ptr[5] = 6;
//  //}
//
//  //MPI_Win_fence(0,win);
//
//  //int c0;
//  //int c1;
//  //if (rank(test_comm)==1) {
//  //  MPI_Get(&c0, 1,MPI_INT, 0,3, 1,MPI_INT, win);
//  //  MPI_Get(&c1, 1,MPI_INT, 0,5, 1,MPI_INT, win);
//  //}
//
//  //MPI_Win_fence(0,win);
//
//
//  //if (rank(test_comm)==1) {
//  //  ELOG(c0);
//  //  ELOG(c1);
//  //}
//
//  //MPI_Win_free(&win);
//  ////std::cout << "base_ptr = " << base_ptr << "\n";
//
//}
