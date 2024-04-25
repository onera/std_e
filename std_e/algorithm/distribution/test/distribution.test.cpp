#if __cplusplus > 201703L
#include "std_e/unit_test/doctest.hpp"
#include "std_e/algorithm/distribution/distribution.hpp"
#include <vector>

using namespace std_e;


TEST_CASE("uniform") {
  int sz_tot = 38;
  int n_interval = 5;

  SUBCASE("uniform_distribution_point") {
    CHECK( uniform_distribution_point(sz_tot,n_interval,0) ==  0 );
    CHECK( uniform_distribution_point(sz_tot,n_interval,1) ==  8 ); // + 8
    CHECK( uniform_distribution_point(sz_tot,n_interval,2) == 16 ); // + 8
    CHECK( uniform_distribution_point(sz_tot,n_interval,3) == 24 ); // + 8
    CHECK( uniform_distribution_point(sz_tot,n_interval,4) == 31 ); // + 7
    CHECK( uniform_distribution_point(sz_tot,n_interval,5) == 38 ); // + 7
  }

  SUBCASE("uniform_distribution") {
    CHECK( uniform_distribution(n_interval,sz_tot) == distribution_vector{0,8,16,24,31,38} );
  }

  SUBCASE("uniform_distribution_") {
    CHECK( uniform_distribution_exclude_ends(n_interval,sz_tot) == distribution_vector{8,16,24,31} );
  }
}
#endif // __cplusplus > 201703L
