#include "doctest/doctest.h"
#include "std_e/utils/doctest_utils.hpp"
#include "std_e/algorithm/distribution.hpp"
#include <vector>

using namespace std_e;

TEST_CASE("uniform_distribution") {
  SUBCASE("empty remainder") {
    int nb_slots = 3;
    std::vector<int> v(1+nb_slots);

    uniform_distribution(begin(v),end(v),100,15);
  // distributing 15 over 3 slots gives 5 + 5 + 5
  //                                    |   |   |
  //                                    v   v   v
    std::vector<int> v_expected = {100,105,110,115};
    CHECK( v == v_expected );
  }

  SUBCASE("non-empty remainder") {
    int nb_slots = 3;
    std::vector<int> v(1+nb_slots);

    uniform_distribution(begin(v),end(v),100,17);

  // distributing 17 over 3 slots gives 6 + 6 + 5
  //                                    |   |   |
  //                                    v   v   v
    std::vector<int> v_expected = {100,106,112,117};
    CHECK( v == v_expected );
  }

  SUBCASE("no slots") {
    int nb_slots = 0;
    std::vector<int> v(1+nb_slots);

    uniform_distribution(begin(v),end(v),100,0);

  // distributing  0 over 0 slots gives nothing
  //                                    |
  //                                    v
    std::vector<int> v_expected = {100};
    CHECK( v == v_expected );
  }
}
