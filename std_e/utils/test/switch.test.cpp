#include "std_e/unit_test/doctest.hpp"

#include "std_e/utils/switch.hpp"
#include <vector>

using namespace std;

namespace {
  struct callable_with_dispatching_tag_arguments {
    auto operator()(std::integral_constant<int,42>,const vector<int>& v, double d) -> std::string {
      return "case 42 with " + std::to_string(v[0]) + std::to_string(v[1]) + '/' + std::to_string(int(d));
    }
    auto operator()(std::integral_constant<int,43>,const vector<int>& v, double d) -> std::string {
      return "case 43 with " + std::to_string(v[0]) + std::to_string(v[1]) + '/' + std::to_string(int(d));
    }
  };
}

TEST_CASE("switch_") {
  using dispatch_indices = std::integer_sequence<int,42,43>;

  callable_with_dispatching_tag_arguments f;
  vector<int> v = {7,8};
  double d = 101;

  // switch_ is created with run-time parameter 42,
  // but 42 is promoted to the compile-time argument std::integral_constant<int,42> when passed to function f
  auto case_42 = std_e::switch_<dispatch_indices>(42).apply(f,v,d);
  CHECK( case_42 == "case 42 with 78/101" );

  auto case_43 = std_e::switch_<dispatch_indices>(43).apply(f,v,d);
  CHECK( case_43 == "case 43 with 78/101" );

  CHECK_THROWS_AS(std_e::switch_<dispatch_indices>(44),std_e::case_is_not_available_error);
}
