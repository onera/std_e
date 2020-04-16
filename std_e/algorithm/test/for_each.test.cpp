#include "std_e/unit_test/doctest.hpp"
#include "std_e/algorithm/for_each.hpp"
#include <vector>
#include <algorithm>

using namespace std_e;


TEST_CASE("for_each_if") {
  std::vector<int> v = {100,101,2000};
  std::vector<int> res;
  auto pred = [](int i){ return i%2==0; };
  auto f = [&res](int i){ res.push_back(2*i); };
  for_each_if(begin(v),end(v),pred,f);
  std::vector<int> expected_res = {200,4000};
  CHECK( res == expected_res );
}


struct lvl_value {
  int lvl;
  double d;
};
constexpr auto
operator==(const lvl_value& x, const lvl_value& y) -> bool {
  return x.lvl==y.lvl && x.d==y.d;
}
auto
to_string(const lvl_value& x) -> std::string {
  return "("+std::to_string(x.lvl) + "," + std::to_string(x.d)+")";
}


class lvl_comparison_generator {
  public:
    constexpr
    lvl_comparison_generator(int lvl)
      : lvl(lvl)
    {}

    constexpr auto
    operator++() {
      ++lvl;
      return *this;
    }
    constexpr auto
    operator*() const {
      return [lvl = this->lvl](const lvl_value& x){ return x.lvl==lvl; };
    }
  private:
    int lvl;
};

TEST_CASE("for_each_partition") {
  std::vector<lvl_value> v_sorted_by_lvl = {
    {0,10},
    {0,11},
    {0,12},

    {1,20},
    {1,21},
    {1,22},
    {1,23},

    {2,30},
    {2,31},
  };
  for_each_partition(
    begin(v_sorted_by_lvl),end(v_sorted_by_lvl),
    lvl_comparison_generator(0),
    [](auto f, auto l){ return std::reverse(f,l); }
  );

  std::vector<lvl_value> expected_res = {
    {0,12},
    {0,11},
    {0,10},
         
    {1,23},
    {1,22},
    {1,21},
    {1,20},
         
    {2,31},
    {2,30},
  };

  CHECK( v_sorted_by_lvl == expected_res );
}


TEST_CASE("for_each_equivalent_ref") {
  std::vector<lvl_value> v_sorted_by_lvl = {
    {0,10},
    {0,11},
    {0,12},

    {1,20},
    {1,21},
    {1,22},
    {1,23},

    {2,30},
    {2,31},
  };
  auto replace_value_if_same_lvl = [](lvl_value& x, lvl_value& x_eq){
    if (x.lvl==x_eq.lvl) {
      x.d = x_eq.d;
      return true;
    }
    return false;
  };
  for_each_equivalent_ref(begin(v_sorted_by_lvl),end(v_sorted_by_lvl), replace_value_if_same_lvl);

  std::vector<lvl_value> expected_res = {
    {0,10},
    {0,10},
    {0,10},
         
    {1,20},
    {1,20},
    {1,20},
    {1,20},
         
    {2,30},
    {2,30},
  };

  CHECK( v_sorted_by_lvl == expected_res );
}
