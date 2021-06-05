#if __cplusplus > 201703L

#include "std_e/future/make_array.hpp"
#include "std_e/unit_test/doctest.hpp"
#include "std_e/operation/dyn_op_functor.hpp"
#include <ranges>

namespace {

using namespace std;
using namespace std_e;
namespace views = std::ranges::views;

// [Sphinx Doc] apply_operation {
TEST_CASE("Use of apply_operation for algorithms calling operations") {
  array<operation_kind,3> ops =
    { operation_kind::plus, operation_kind::min, operation_kind::max };

  auto call_with_9_and_3 = [](operation_kind op) {
    return apply_operation(op,9,3);
  };
  auto results = ops | views::transform(call_with_9_and_3);
  CHECK( results[0] == 9+3 );
  CHECK( results[1] == 3 );
  CHECK( results[2] == 9 );
}
// [Sphinx Doc] apply_operation }

struct no_behavior_type {};

TEST_CASE("dyn_op_functor when functions are not implemented") {
  no_behavior_type x;
  no_behavior_type y;

  CHECK_THROWS( apply_operation(operation_kind::plus,x,y) );
  CHECK_THROWS( apply_operation(operation_kind::minus,x,y) );
  CHECK_THROWS( apply_operation(operation_kind::divides,x,y) );

  CHECK_THROWS( apply_operation(operation_kind::sqrt,x) );
}

} // anon
#endif // C++20
