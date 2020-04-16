#include "std_e/unit_test/doctest.hpp"
#include "std_e/operation/operation_kind.hpp"
#include "std_e/operation/operation_functor.hpp"
#include <array>


TEST_CASE("Example use of operation_functor for generic treatment of operators") {
  int i=42;
  int j=43;
  constexpr std::array<std_e::operation_kind,3> op_kinds = {
    std_e::operation_kind::plus,
    std_e::operation_kind::minus,
    std_e::operation_kind::multiplies
  };

  std::array<int,3> results = {
    std_e::operation_functor<op_kinds[0]>(i,j),
    std_e::operation_functor<op_kinds[1]>(i,j),
    std_e::operation_functor<op_kinds[2]>(i,j)
  };

  std::array<int,3> expected_results = {
    42+43,
    42-43,
    42*43
  };
  CHECK( results == expected_results );
}

TEST_CASE("operation_functor for int") {
  int i=42;
  int j=43;

  constexpr auto op_identity  = std_e::operation_functor<std_e::operation_kind::identity>;
  constexpr auto op_plus = std_e::operation_functor<std_e::operation_kind::plus>;
  constexpr auto op_minus = std_e::operation_functor<std_e::operation_kind::minus>;
  constexpr auto op_multiplies = std_e::operation_functor<std_e::operation_kind::multiplies>;
  constexpr auto op_divides = std_e::operation_functor<std_e::operation_kind::divides>;
  constexpr auto op_sqrt = std_e::operation_functor<std_e::operation_kind::sqrt>;
  constexpr auto op_abs = std_e::operation_functor<std_e::operation_kind::abs>;
  constexpr auto op_min = std_e::operation_functor<std_e::operation_kind::min>;
  constexpr auto op_max = std_e::operation_functor<std_e::operation_kind::max>;

  CHECK ( op_identity(i) == i );

  CHECK ( op_plus(i) == 42 );
  CHECK ( op_plus(i,j) == 42+43 );

  CHECK ( op_minus(i) == -42 );
  CHECK ( op_minus(i,j) == 42-43 );

  CHECK ( op_multiplies(i,j) == 42*43 );
  CHECK ( op_divides(i,j) == 42/43 );

  CHECK ( op_sqrt(i) == std::sqrt(42) );
  CHECK ( op_abs(i) == 42 );
  CHECK ( op_abs(-i) == 42 );

  CHECK ( op_min(i,j) == 42 );
  CHECK ( op_max(i,j) == 43 );
}


struct tensor_like_type_for_test {
  double d0;
  double d1;
};
auto
operator==(const tensor_like_type_for_test& x, const tensor_like_type_for_test& y) {
  return x.d0==y.d0 && x.d1==y.d1;
}

auto
operator|(const tensor_like_type_for_test& x, const tensor_like_type_for_test& y) -> double {
  return x.d0*y.d0 + x.d1*y.d1;
}
auto
tensor_prod(const tensor_like_type_for_test& x, const tensor_like_type_for_test& y) -> double {
  return x.d0*y.d0 * x.d1*y.d1; // its really a fake tensor_prod!
}
auto
t(const tensor_like_type_for_test& x) -> tensor_like_type_for_test {
  return {x.d1,x.d0};
}
auto
tr(const tensor_like_type_for_test& x) -> double {
  return x.d0 + x.d1;
}

TEST_CASE("operation_functor for user-defined type") {
  tensor_like_type_for_test x = {42.,43.};
  tensor_like_type_for_test y = {44.,45.};

  constexpr auto op_pipe = std_e::operation_functor<std_e::operation_kind::pipe>;
  //constexpr auto op_tensor_prod = std_e::operation_functor<std_e::operation_kind::tensor_prod>;
  constexpr auto op_t = std_e::operation_functor<std_e::operation_kind::t>;
  constexpr auto op_tr = std_e::operation_functor<std_e::operation_kind::tr>;

  CHECK ( op_pipe(x,y) == 42.*44. + 43.*45. );
  // CHECK ( op_tensor_prod(x,y) == tensor_order(x,y) ); // TODO uncomment
  CHECK ( op_t(x) == tensor_like_type_for_test{43.,42.} );
  CHECK ( op_tr(x) == 42.+43. );
}
