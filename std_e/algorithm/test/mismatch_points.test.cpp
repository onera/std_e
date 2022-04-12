#if __cplusplus > 201703L
#include "std_e/unit_test/doctest.hpp"
#include "std_e/algorithm/mismatch_points.hpp"
#include <vector>

using namespace std;

TEST_CASE("mismatch_points") {
  SUBCASE("standard") {
    vector<int> v                    = {2,2,2,4,4,1,1,1,1,2,2};
    vector<int> expected_mismatch_is = {0,    3  ,5      ,9  ,11};

    vector<int> mismatch_is = std_e::mismatch_indices(v);
    CHECK( mismatch_is == expected_mismatch_is );
  }

  SUBCASE("uniform") {
    vector<int> v                    = {2,2,2};
    vector<int> expected_mismatch_is = {0    ,3};

    vector<int> mismatch_is = std_e::mismatch_indices(v);
    CHECK( mismatch_is == expected_mismatch_is );
  }
  SUBCASE("empty") {
    vector<int> v                    = {};
    vector<int> expected_mismatch_is = {0};

    vector<int> mismatch_is = std_e::mismatch_indices(v);
    CHECK( mismatch_is == expected_mismatch_is );
  }
}
#endif // C++>17
