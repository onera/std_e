#include "std_e/unit_test/doctest.hpp"


#include "std_e/base/system/mem_usage.hpp"
#include "std_e/base/system/profiling.hpp"


using namespace std_e;


//TEST_CASE("remaining_system_memory_in_bytes") {
//  // 0. Test
//  long free_mem_before_alloc = remaining_system_memory_in_bytes();
//
//  long n_bytes = 1'000'000;
//  std::vector<char> v(n_bytes);
//
//  long free_mem_after_alloc = remaining_system_memory_in_bytes();
//  escape(&v); // do not optimize v away before this line
//
//  // 1. Checks
//  // WARNING:
//  //   (1) This function only gives the available memory of the whole system.
//  //       So if another program happens to have allocated memory between
//  //         the two calls to remaining_system_memory_in_bytes,
//  //         `alloc_memory` does not only measure the allocation of `v`
//  //         but all of what happened in the system.
//  //       Hence, the following check may not be satisfied (non-deterministic)
//  //   (2) The amount of memory allocated depends on what has already been allocated
//  //         but is currently free to use
//  //       This is deterministic but implementation-specified
//  //         (basically, depends on the malloc implementation),
//  long alloc_memory = free_mem_before_alloc - free_mem_after_alloc;
//  CHECK( std::abs(alloc_memory - n_bytes)/double(n_bytes) < 0.2 );
//}


TEST_CASE("resident_memory_in_bytes") {
  // 0. Test
  long rss_mem0 = resident_memory_in_bytes();

  long n_bytes = 1'000'000;
  std::vector<char> v(n_bytes);

  long rss_mem1 = resident_memory_in_bytes();
  escape(&v); // do not optimize v


  v.resize(0);
  v.shrink_to_fit();
  long rss_mem2 = resident_memory_in_bytes();

  escape(&v); // do not optimize v

  // 1. Checks
  long delta01 = rss_mem1-rss_mem0;
  long delta12 = rss_mem2-rss_mem1;
  long delta02 = rss_mem2-rss_mem0;


  // Check the allocation is approximately what was asked
  // NOTE: we choose 50% error for the test to pass (to be safe)
  //       with greater n_bytes values, the relative error would be less than 50% (fortunately!)
  //       but we don't want to do that too much in a regular unit test
  CHECK( delta01 > 0 ); // memory was created
  CHECK( std::abs(delta01 - n_bytes)/double(n_bytes) < 0.5 );

  // Check the deallocation is approximately of the same size (but <0)
  CHECK( delta12 < 0 ); // memory was reclaimed
  CHECK( std::abs(-delta12 - n_bytes)/double(n_bytes) < 0.5 );

  // Check the net memory allocation is approximately null
  CHECK( std::abs(delta02)/double(n_bytes) < 0.5 );

}
