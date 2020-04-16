#include "std_e/unit_test/doctest.hpp"

#include "std_e/multi_index/multi_index_range.hpp"


using namespace std_e;


TEST_CASE("multi_index_range fortran order") {
  using MI = multi_index<int,3>;
  MI dims = {4,3,2};

  auto all_indices_range = fortran_multi_index_range(dims);

  std::vector<MI> all_indices(4*3*2);
  int k=0;
  for(const auto& is : all_indices_range) {
    all_indices[k] = is;
    ++k;
  }
  CHECK( k == 4*3*2 );

  std::vector<MI> expected_all_indices = {
    {0,0,0},
    {1,0,0},
    {2,0,0},
    {3,0,0},
    {0,1,0},
    {1,1,0},
    {2,1,0},
    {3,1,0},
    {0,2,0},
    {1,2,0},
    {2,2,0},
    {3,2,0},
    {0,0,1},
    {1,0,1},
    {2,0,1},
    {3,0,1},
    {0,1,1},
    {1,1,1},
    {2,1,1},
    {3,1,1},
    {0,2,1},
    {1,2,1},
    {2,2,1},
    {3,2,1},
  };
  CHECK( all_indices == expected_all_indices );
}


TEST_CASE("multi_index_range general order") {
  using MI = multi_index<int,3>;
  MI dims = {4,3,2};
  MI order = {1,2,0};

  auto all_indices_range = multi_index_range_with_order(dims,order);

  std::vector<MI> all_indices(4*3*2);
  int k=0;
  for(const auto& is : all_indices_range) {
    all_indices[k] = is;
    ++k;
  }
  CHECK( k == 4*3*2 );
  std::vector<MI> expected_all_indices = {
    {0,0,0},
    {0,1,0},
    {0,2,0},
    {0,0,1},
    {0,1,1},
    {0,2,1},
    {1,0,0},
    {1,1,0},
    {1,2,0},
    {1,0,1},
    {1,1,1},
    {1,2,1},
    {2,0,0},
    {2,1,0},
    {2,2,0},
    {2,0,1},
    {2,1,1},
    {2,2,1},
    {3,0,0},
    {3,1,0},
    {3,2,0},
    {3,0,1},
    {3,1,1},
    {3,2,1},
  };
  CHECK( all_indices == expected_all_indices );
}
