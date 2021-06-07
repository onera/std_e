#include "std_e/unit_test/doctest.hpp"

#include "std_e/utils/switch.hpp"
#include <numeric>
#include <type_traits>
#include <vector>

using namespace std_e;
using std::vector;
using std::integral_constant;

namespace {


// [Sphinx Doc] switch {
enum op_kind {
  sum, prod, min, max
};

struct f_with_tag_arg {
  int operator()(integral_constant<op_kind,sum>, const vector<int>& v) {
    return std::accumulate(begin(v),end(v),0,std::plus<>{});
  }
  int operator()(integral_constant<op_kind,prod>, const vector<int>& v) {
    return std::accumulate(begin(v),end(v),1,std::multiplies<>{});
  }
};

TEST_CASE("switch_") {
  using dispatch_indices = std::integer_sequence<op_kind,sum,prod>;

  f_with_tag_arg f;
  vector<int> v = {7,8};

  // switch_ is created with run-time parameter "sum",
  // but "sum" is promoted to the compile-time argument
  // std::integral_constant<op_kind,sum> when passed to function f
  auto case_sum = switch_<dispatch_indices>(sum).apply(f,v);
  CHECK( case_sum == 7+8 );

  auto case_prod = switch_<dispatch_indices>(prod).apply(f,v);
  CHECK( case_prod == 7*8 );

  CHECK_THROWS_AS(
    switch_<dispatch_indices>(min).apply(f,v),
    case_is_not_available_error
  );
}
// [Sphinx Doc] switch }


}
