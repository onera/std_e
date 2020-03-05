#include "doctest/doctest.h"
#include "std_e/multi_array/multi_array.hpp"

using namespace std_e;

TEST_CASE("multi_arrays of rank 0 are scalars") {
  SUBCASE("fixed array") {
    fixed_multi_array<int> f;
    CHECK( f.rank() == 0 );
    CHECK( f.size() == 1 );

    f() = 42;
    CHECK( f() == 42 );
  }
  SUBCASE("dyn array") {
    // The handling of dyn arrays of rank 0 is not well supported
    // because it would require special cases
    // whereas it seems not very useful

    dyn_multi_array<int,int32_t,0> f_no_alloc;
    CHECK( f_no_alloc.rank() == 0 );
    CHECK( f_no_alloc.size() == 1 ); 

    // WARNING: nothing has been allocated!
    // The following would segfault
    // f_no_alloc() = 42; // BAD: SEGFAULT
  }
}
