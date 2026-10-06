#include "std_e/unit_test/doctest.hpp"
#include "std_e/meta/find.hpp"

using namespace std_e;

#include <vector>
#include <deque>

TEST_CASE("meta::find_index") {
  static_assert(meta::find_idx<int, int> == 0);
  static_assert(meta::find_idx<int, float,int> == 1);
  static_assert(meta::find_idx<int, int,float,int> == 0);

  // This one should result in a compilation error
  //static_assert(meta::find_idx<int, float> == 0);
}


TEST_CASE("meta::find_rng_index") {
  static_assert(meta::find_rng_idx<int, std::vector<int>> == 0);
  static_assert(meta::find_rng_idx<int, std::vector<float>,std::deque<int>> == 1);
  static_assert(meta::find_rng_idx<int, std::deque<int>,std::vector<float>,std::vector<int>> == 0);
}


template<class Rng>
struct Proj_value_type_impl {
  using type = Rng::value_type;
};
template<class Rng> using Proj_value_type = typename Proj_value_type_impl<Rng>::type;

TEST_CASE("meta::find_proj_index") {
  static_assert(meta::find_proj_idx<int, Proj_value_type, std::vector<int>> == 0);
  static_assert(meta::find_proj_idx<int, Proj_value_type, std::vector<float>,std::deque<int>> == 1);
  static_assert(meta::find_proj_idx<int, Proj_value_type, std::deque<int>,std::vector<float>,std::vector<int>> == 0);
}
