#include "doctest/doctest.h"
#include "std_e/multi_index/cx_multi_index.h"
#include "std_e/multi_index/multi_index.h"

using namespace std_e;


TEST_CASE("Concatenate cx_multi_index") {
  cx_multi_index is0 = {1,2,3,9};
  cx_multi_index is1 = {8,7,6};
  cx_multi_index is2 = {10};

  cx_multi_index is012 = std_e::concatenate_indices(is0,is1,is2);
  cx_multi_index expected_is012 = {1,2,3,9,8,7,6,10};
  CHECK( is012 == expected_is012 );
}


TEST_CASE("Concatenate multi_index") {
  multi_index<int,4> is0 = {1,2,3,9};
  multi_index<int,3> is1 = {8,7,6};
  multi_index<int,1> is2 = {10};

  multi_index<int,4+3+1> is012 = std_e::concatenate_indices(is0,is1,is2);
  multi_index<int,4+3+1> expected_is012 = {1,2,3,9,8,7,6,10};
  CHECK( is012 == expected_is012 );
}
