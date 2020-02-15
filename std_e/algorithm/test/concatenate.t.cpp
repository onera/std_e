#include "doctest/doctest.h"

#include "std_e/algorithm/concatenate.h"
#include <vector>
#include "std_e/multi_array/multi_array.h"
#include "std_e/multi_array/utils.h"

TEST_CASE("concatenate") {
  std::vector<std::vector<int>> vs = {
    {0,1,2,3},
    {4,5}
  };

  std::vector<int> cat_vs(6);
  std_e::concatenate(begin(vs),end(vs),begin(cat_vs));

  std::vector<int> expected_cat_vs = {0,1,2,3,4,5};
  CHECK( cat_vs == expected_cat_vs );
}



TEST_CASE("concatenate on axis") {
  using md_array_type = std_e::dyn_multi_array<double,int,2>;
  std::vector<md_array_type> in_arrays = {
    {{ 0., 1., 2., 3.},
     { 6., 7., 8., 9.},
     {12.,13.,14.,15.}},
    {{ 4., 5.},
     {10.,11.},
     {16.,17.}}
  };

  md_array_type out_array(3,4+2);

  std_e::concatenate_on_axis(1,in_arrays,out_array);

  md_array_type expected_out_array =
    {{ 0., 1., 2., 3., 4., 5.},
     { 6., 7., 8., 9.,10.,11.},
     {12.,13.,14.,15.,16.,17.}};
  CHECK( out_array == expected_out_array );
}
