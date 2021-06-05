#include "std_e/unit_test/doctest.hpp"
#include "std_e/operation/operation_kind.hpp"
#include "std_e/operation/operation_functor.hpp"
#include <array>

using namespace std;
using namespace std_e;

// [Sphinx Doc] operation_functor principle {
TEST_CASE("Principle of operation_functor") {
  int i=42;
  int j=43;
  constexpr auto op = operation_kind::plus;
  CHECK( operation_functor<op>(i,j) == 42+43 );
}
// [Sphinx Doc] operation_functor principle }

// [Sphinx Doc] operation_functor gen {
TEST_CASE("Use of operation_functor for generic treatment of operators") {
  constexpr array<operation_kind,3> ops =
    { operation_kind::plus, operation_kind::min, operation_kind::max };

  array<int,3> results = {
    operation_functor<ops[0]>(9,3),
    operation_functor<ops[1]>(9,3),
    operation_functor<ops[2]>(9,3)
  };

  CHECK( results[0] == 9+3 );
  CHECK( results[1] == 3 );
  CHECK( results[2] == 9 );
}
// [Sphinx Doc] operation_functor gen }

TEST_CASE("operation_functor for int") {
  int i=42;
  int j=43;

  constexpr auto op_identity  = operation_functor<operation_kind::identity>;
  constexpr auto op_plus = operation_functor<operation_kind::plus>;
  constexpr auto op_minus = operation_functor<operation_kind::minus>;
  constexpr auto op_multiplies = operation_functor<operation_kind::multiplies>;
  constexpr auto op_divides = operation_functor<operation_kind::divides>;
  constexpr auto op_sqrt = operation_functor<operation_kind::sqrt>;
  constexpr auto op_abs = operation_functor<operation_kind::abs>;
  constexpr auto op_min = operation_functor<operation_kind::min>;
  constexpr auto op_max = operation_functor<operation_kind::max>;

  CHECK ( op_identity(i) == i );

  CHECK ( op_plus(i) == 42 );
  CHECK ( op_plus(i,j) == 42+43 );

  //CHECK ( op_minus(i) == -42 ); // TODO
  CHECK ( op_minus(i,j) == 42-43 );

  CHECK ( op_multiplies(i,j) == 42*43 );
  CHECK ( op_divides(i,j) == 42/43 );

  CHECK ( op_sqrt(i) == sqrt(42) );
  CHECK ( op_abs(i) == 42 );
  CHECK ( op_abs(-i) == 42 );

  CHECK ( op_min(i,j) == 42 );
  CHECK ( op_max(i,j) == 43 );
}


namespace {
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
    return x.d0*y.d0 * x.d1*y.d1; // it really is a fake tensor_prod!
  }
  auto
  t(const tensor_like_type_for_test& x) -> tensor_like_type_for_test {
    return {x.d1,x.d0};
  }
  auto
  tr(const tensor_like_type_for_test& x) -> double {
    return x.d0 + x.d1;
  }
}

TEST_CASE("operation_functor for user-defined type") {
  tensor_like_type_for_test x = {42.,43.};
  tensor_like_type_for_test y = {44.,45.};

  constexpr auto op_bit_or = operation_functor<operation_kind::bit_or>;
  constexpr auto op_tensor_prod = operation_functor<operation_kind::tensor_prod>;
  constexpr auto op_t = operation_functor<operation_kind::t>;
  constexpr auto op_tr = operation_functor<operation_kind::tr>;

  CHECK ( op_bit_or(x,y) == 42.*44. + 43.*45. );
  CHECK ( op_tensor_prod(x,y) == 42.*44. * 43.*45. );
  CHECK ( op_t(x) == tensor_like_type_for_test{43.,42.} );
  CHECK ( op_tr(x) == 42.+43. );
}
