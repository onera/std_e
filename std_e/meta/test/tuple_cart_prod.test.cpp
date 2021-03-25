#include "std_e/unit_test/doctest.hpp"

#include "std_e/meta/tuple_product.hpp"

using namespace std_e;

//using A = char;
class A {
  public:
  A() = default;
  A(const A&) = delete;
  A& operator=(const A&) = delete;
};
using B = int;
using C = long;
using D = double;


TEST_CASE("tuple cart prod") {
  using std::tuple;
  using tuple_0 = tuple<A,B>;
  using tuple_1 = tuple<C,D>;
  using res_type = std_e::tuple_types_product<tuple_0,tuple_1>;

  static_assert(std::is_same_v< res_type , tuple<tuple<A,C>,tuple<A,D>,tuple<B,C>,tuple<B,D>> >);
}
