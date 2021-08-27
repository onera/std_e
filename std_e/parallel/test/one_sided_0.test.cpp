#include "doctest/extensions/doctest_mpi.h"
#include "std_e/parallel/mpi.hpp"

#include "std_e/log.hpp"

#include <stdio.h>
#include <stdlib.h>

//#include <chrono>
//#include <thread>
//std::this_thread::sleep_for(std::chrono::milliseconds(x));
using namespace std_e;

//MPI_TEST_CASE("all_gather",2) {
//  // Get my rank
//  int my_rank = rank(test_comm);
//
//  // Create the window
//  int window_buffer = 0;
//  MPI_Win window;
//  MPI_Win_create(&window_buffer, sizeof(int), sizeof(int), MPI_INFO_NULL, test_comm, &window);
//  if(my_rank == 1)
//  {
//      printf("[MPI process 1] Value in my window_buffer before MPI_Put: %d.\n", window_buffer);
//  }
//  MPI_Win_fence(0, window);
//
//  if(my_rank == 0)
//  {
//      // Push my value into the first integer in MPI process 1 window
//      int my_value = 12345;
//      MPI_Put(&my_value, 1, MPI_INT, 1, 0, 1, MPI_INT, window);
//      printf("[MPI process 0] I put data %d in MPI process 1 window via MPI_Put.\n", my_value);
//  }
//
//  // Wait for the MPI_Put issued to complete before going any further
//  MPI_Win_fence(0, window);
//
//  if(my_rank == 1)
//  {
//      printf("[MPI process 1] Value in my window_buffer after MPI_Put: %d.\n", window_buffer);
//  }
//
//  // Destroy the window
//  MPI_Win_free(&window);
//
//
//  LOG("Moai")
//}
//
