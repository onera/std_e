#include "doctest/doctest.h"
#include "std_e/base/var_arg_reduction.h"

using namespace std_e;


struct operation_for_test {
  constexpr auto
  operator()(int i) -> int {
    return 2*i;
  }
  constexpr auto
  operator()(int i, int j) -> int {
    return 2*(i + j);
  }
};

TEST_CASE("var_arg_reduction") {
  operation_for_test op;
  int i = 42;
  int j = 43;
  int k = 44;

  int arity_1 = var_arg_reduction(op,i);
  int expected_arity_1 = 2 * 42;
  CHECK( arity_1 == expected_arity_1 );
  
  int arity_2 = var_arg_reduction(op,i,j);
  int expected_arity_2 = 2 * (42+43);
  CHECK( arity_2 == expected_arity_2 );
  
  int arity_3 = var_arg_reduction(op,i,j,k);
  int expected_arity_3 = 2* (2*(42+43)+44);
  CHECK( arity_3 == expected_arity_3 );
}

TEST_CASE("reduction_functor") {
  int i = 42;
  int j = 43;
  int k = 44;

  /// We want this to compile (it does not because operation_for_test is not n-ary)
  //operation_for_test bin_arity_op;
  //int arity_3 = bin_arity_op(i,j,k); // error

  /// Solution: wrap the operation type into a reduction functor
  reduction_functor<operation_for_test> multi_arity_op;
  int arity_3 = multi_arity_op(i,j,k); 

  int expected_arity_3 = 2* (2*(42+43)+44);
  CHECK( arity_3 == expected_arity_3 );
}
