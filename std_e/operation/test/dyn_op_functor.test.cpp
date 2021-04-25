#include "std_e/unit_test/doctest.hpp"
#include "std_e/operation/dyn_op_functor.hpp"

using namespace std_e;

TEST_CASE("dyn_op_functor") {
  int i = 9;
  int j = 3;

  CHECK( apply_operation(operation_kind::plus,i,j) == 9+3 );
  CHECK( apply_operation(operation_kind::minus,i,j) == 9-3 );
  CHECK( apply_operation(operation_kind::divides,i,j) == 9/3 );

  CHECK( apply_operation(operation_kind::sqrt,i) == 3 );

  std::vector<int> args = {i,j};
  CHECK( apply_operation_to_args(operation_kind::plus,args) == 9+3 );
}

struct no_behavior_type {};

TEST_CASE("dyn_op_functor when functions are not implemented") {
  no_behavior_type x;
  no_behavior_type y;

  CHECK_THROWS( apply_operation(operation_kind::plus,x,y) );
  CHECK_THROWS( apply_operation(operation_kind::minus,x,y) );
  CHECK_THROWS( apply_operation(operation_kind::divides,x,y) );

  CHECK_THROWS( apply_operation(operation_kind::sqrt,x) );
}
