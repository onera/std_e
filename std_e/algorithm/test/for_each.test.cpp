#include "std_e/unit_test/doctest.hpp"
#include "std_e/unit_test/id_string.hpp"
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

TEST_CASE("for_each_until") {
  std::vector<int> v = {100,101,2000,3};
  auto pred = [](int i){ return i<1000; };
  auto f = [](int& i){ i -= 100; };
  for_each_until(begin(v),end(v),pred,f);
  std::vector<int> expected_v = {0,1,2000,3};
  CHECK( v == expected_v );
}


class id_comparison_generator {
  public:
    constexpr
    id_comparison_generator(int id)
      : id(id)
    {}

    constexpr auto
    operator++() {
      ++id;
      return *this;
    }
    constexpr auto
    operator*() const {
      return [id = this->id](const id_string& x){ return x.id==id; };
    }
  private:
    int id;
};

TEST_CASE("for_each_partition") {
  std::vector<id_string> v_sorted_by_id = {
    {0,"10"},
    {0,"11"},
    {0,"12"},

    {1,"20"},
    {1,"21"},
    {1,"22"},
    {1,"23"},

    {2,"30"},
    {2,"31"},
  };
  for_each_partition(
    begin(v_sorted_by_id),end(v_sorted_by_id),
    id_comparison_generator(0),
    [](auto f, auto l){ return std::reverse(f,l); }
  );

  std::vector<id_string> expected_res = {
    {0,"12"},
    {0,"11"},
    {0,"10"},

    {1,"23"},
    {1,"22"},
    {1,"21"},
    {1,"20"},

    {2,"31"},
    {2,"30"},
  };

  CHECK( v_sorted_by_id == expected_res );
}


TEST_CASE("for_each_equivalent_ref") {
  std::vector<id_string> v_sorted_by_id = {
    {0,"10"},
    {0,"11"},
    {0,"12"},

    {1,"20"},
    {1,"21"},
    {1,"22"},
    {1,"23"},

    {2,"30"},
    {2,"31"},
  };
  auto replace_value_if_same_id = [](id_string& x, id_string& x_eq){
    if (x.id==x_eq.id) {
      x.s = x_eq.s;
      return true;
    }
    return false;
  };
  for_each_equivalent_ref(begin(v_sorted_by_id),end(v_sorted_by_id), replace_value_if_same_id);

  std::vector<id_string> expected_res = {
    {0,"10"},
    {0,"10"},
    {0,"10"},
         
    {1,"20"},
    {1,"20"},
    {1,"20"},
    {1,"20"},
         
    {2,"30"},
    {2,"30"},
  };

  CHECK( v_sorted_by_id == expected_res );
}
