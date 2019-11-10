#include "doctest/doctest.h"
#include "std_e/utils/dereferencing_range.h"

#include <vector>
#include <algorithm>
using std::vector;
using namespace std_e;

TEST_CASE("Range<double*> -> Range<double refs> ") {
  double d0 = 42.;
  double d1 = 100.;
  double d2 = 20.;
  std::vector<double*> v = {&d0,&d1,&d2};

  SUBCASE("dereferencing_range") {
    auto it = derefencing_iterator(begin(v));

    CHECK( *it == 42. );

    ++it;
    CHECK( *it == 100. );

    ++it;
    CHECK( *it == 20. );
  }

  SUBCASE("dereferencing_range") {
    auto r = dereferencing_range(v);

    std::vector<double> res;
    std::copy(begin(r),end(r),std::back_inserter(res));

    const std::vector<double> expected_res = {42.,100.,20.};

    CHECK( res == expected_res );
  }
}

