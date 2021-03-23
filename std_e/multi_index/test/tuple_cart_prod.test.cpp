#include "std_e/unit_test/doctest.hpp"

#include "std_e/multi_index/multi_index_range.hpp"
#include <tuple>
#include <utility>
#include "std_e/log.hpp"

using namespace std_e;

using A = char;
using B = int;
using C = long;
using D = double;


template<int mir_size, class multi_index_range_type> constexpr auto
generate(const multi_index_range_type& mir) {
  constexpr int sz = mir_size;
  constexpr int rank = 2;
  std::array<multi_index<int,rank>,sz> res = {};
  int i=0;
  for (auto mi : mir) {
    res[i] = mi;
    ++i;
  }
  return res;
}


template<class tuple_0, class tuple_1, size_t... Is> constexpr auto
tuple_cart_prod__impl(tuple_0, tuple_1, std::index_sequence<Is...>) {
  constexpr int n0 = std::tuple_size_v<tuple_0>;
  constexpr int n1 = std::tuple_size_v<tuple_1>;
  constexpr auto mir = c_multi_index_range({n0,n1});
  constexpr auto explicit_mir = generate<n0*n1>(mir);
  return
    std::tuple<
      std::tuple<
        std::tuple_element_t<explicit_mir[Is][0],tuple_0>
        ,
        std::tuple_element_t<explicit_mir[Is][1],tuple_1>
      >...
    >{};
}

template<class tuple_0, class tuple_1> constexpr auto
tuple_cart_prod(tuple_0 x, tuple_1 y) {
  constexpr int n0 = std::tuple_size_v<tuple_0>;
  constexpr int n1 = std::tuple_size_v<tuple_1>;
  constexpr int cart_prod_size = n0*n1;

  return tuple_cart_prod__impl(x, y, std::make_index_sequence<cart_prod_size>());
}

template<class tuple_0, class tuple_1>
using tuple_type_cart_prod = decltype(tuple_cart_prod(std::declval<tuple_0>(),std::declval<tuple_1>()));

TEST_CASE("tuple cart prod") {
  using std::tuple;
  using tuple_0 = tuple<A,B>;
  using tuple_1 = tuple<C,D>;
  using res_type = tuple_type_cart_prod<tuple_0,tuple_1>;

  static_assert(std::is_same_v< res_type , tuple<tuple<A,C>,tuple<A,D>,tuple<B,C>,tuple<B,D>> >);
}
