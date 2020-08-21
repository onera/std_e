#include "std_e/unit_test/doctest.hpp"
#include "std_e/utils/meta.hpp"

#include <vector>
#include <algorithm>
#include <functional>
using std::vector;
using std::tuple;

TEST_CASE("reference_wrapper") {

  int  a = 1;
  auto ra = std::ref(a);
  auto cra = std::cref(a);
  // static_assert()
  CHECK( std_e::is_reference_wrapper<decltype(ra)>  == true  );
  CHECK( std_e::is_reference_wrapper<decltype(a)>   == false );
  CHECK( std_e::is_reference_wrapper<decltype(cra)> == true  );

}
