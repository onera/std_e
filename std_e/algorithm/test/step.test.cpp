#include "std_e/unit_test/doctest.hpp"
#include "std_e/algorithm/step.hpp"
#include <vector>

using namespace std_e;

TEST_CASE("apply_step") {
  SUBCASE("normal case") {
    std::vector v(10,0);
    std::vector indices = {0 , 2, 5, 6,10};
    std::vector steps   = {10,20,30,40};
    apply_step(v,indices,steps);

    CHECK( v == std::vector{-10,-10,-20,-20,-20,-30,-40,-40,-40,-40} );
  }
  SUBCASE("degenerate case") {
    std::vector v(10,0);
    std::vector indices = {0 , 0, 5,10,10};
    std::vector steps   = {10,20,30,40};
    apply_step(v,indices,steps);

    CHECK( v == std::vector{-20,-20,-20,-20,-20,-30,-30,-30,-30,-30} );
  }
}
