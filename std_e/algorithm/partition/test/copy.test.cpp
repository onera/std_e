#include "std_e/unit_test/doctest.hpp"
#include "std_e/algorithm/partition/copy.hpp"
#include "std_e/data_structure/multi_range/multi_range.hpp"


using std::vector;


namespace {


struct v_int_v_double {
  std::vector<int> vi;
  std::vector<double> vd;
};

TEST_CASE("pivot_partition_copy") {
  vector<int>    vi = {1000, 800,   8, 1500,  50, -7 ,   0};
  vector<double> vd = {10.5, 8.2, 0.8, 3.14, 2.7, 7.3, 0.1};

  const vector<int> pivots = {0,120,1000};

  vector<v_int_v_double> res(pivots.size()+1);
  auto proj = [](const auto& x){
    auto [i,d] = x;
    return i;
  };
  auto f_copy = [&res](int index, const auto& x) {
    auto [i,d] = x;
    res[index].vi.push_back(i);
    res[index].vd.push_back(d);
  };
  std_e::pivot_partition_copy(std_e::view_as_multi_range(vi,vd),pivots,proj,f_copy);

  CHECK( res[0].vi == vector{-7 } );
  CHECK( res[0].vd == vector{7.3} );
  CHECK( res[1].vi == vector{  8,  50,   0} );
  CHECK( res[1].vd == vector{0.8, 2.7, 0.1} );
  CHECK( res[2].vi == vector{800} );
  CHECK( res[2].vd == vector{8.2} );
  CHECK( res[3].vi == vector{1000, 1500} );
  CHECK( res[3].vd == vector{10.5, 3.14} );
}

TEST_CASE("interval_partition_copy") {
  vector<int>    vi = {1000, 800,   8, 1500,  50, -7 ,   0};
  vector<double> vd = {10.5, 8.2, 0.8, 3.14, 2.7, 7.3, 0.1};

  const std_e::interval_vector<int> intervals = {0,120,1000};

  int n_interval = intervals.size()-1;
  vector<v_int_v_double> res(n_interval);
  vector<int> below_interval;
  vector<int> above_interval;
  auto proj = [](const auto& x){
    auto [i,d] = x;
    return i;
  };
  auto f_copy = [&below_interval,&above_interval,&res,n_interval](int index, const auto& x) {
    auto [i,d] = x;
    if (index==-1) {
      below_interval.push_back(i);
    } else if (index==n_interval) {
      above_interval.push_back(i);
    } else {
      res[index].vi.push_back(i);
      res[index].vd.push_back(d);
    }
  };
  std_e::interval_partition_copy(std_e::view_as_multi_range(vi,vd),intervals,proj,f_copy);

  CHECK( below_interval == vector{-7} );
  CHECK( res[0].vi == vector{  8,  50,   0} );
  CHECK( res[0].vd == vector{0.8, 2.7, 0.1} );
  CHECK( res[1].vi == vector{800} );
  CHECK( res[1].vd == vector{8.2} );
  CHECK( above_interval == vector{1000,1500} );
}


} // anonymous
