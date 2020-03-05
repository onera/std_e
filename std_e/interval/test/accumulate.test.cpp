#include "doctest/doctest.h"


#include "std_e/interval/accumulate.hpp"


TEST_CASE("apply_accumulate") {
  std_e::interval<int> r = {0,5};

  auto square = [](auto i){ return i*i; };
  auto add = [](auto x, auto y){ return x+y; };
  auto sum_of_squares = apply_accumulate(r,square,10,add);

  CHECK( sum_of_squares == 10 + 0*0 + 1*1 + 2*2 + 3*3 + 4*4 );
}

TEST_CASE("sum formula") {
  auto sum_of_squares = 
    std_e::sum(/*i=*/std_e::interval<int>{0,5},            [](auto i){return
                                               i*i;
    });

  CHECK(sum_of_squares == 0*0 + 1*1 + 2*2 + 3*3 + 4*4 );
}

TEST_CASE("prod formula") {
  auto prod_of_plus_2= 
    std_e::prod(/*i=*/std_e::interval<int>{0,5},            [](auto i){return 
                                                i+2;
    });

  CHECK( prod_of_plus_2 == (0+2) * (1+2) * (2+2) * (3+2) * (4+2) );
}


TEST_CASE("index_with_fortran_memory_layout") {
  std::array<int32_t,2> dims = {3,2};

  // in 2D (i.e., for matrices), fortran order means column by column
  CHECK( std_e::index_with_fortran_memory_layout({0,0},dims) == 0 );
  CHECK( std_e::index_with_fortran_memory_layout({1,0},dims) == 1 );
  CHECK( std_e::index_with_fortran_memory_layout({2,0},dims) == 2 );

  CHECK( std_e::index_with_fortran_memory_layout({0,1},dims) == 3 );
  CHECK( std_e::index_with_fortran_memory_layout({1,1},dims) == 4 );
  CHECK( std_e::index_with_fortran_memory_layout({2,1},dims) == 5 );
}

TEST_CASE("index_with_c_memory_layout") {
  std::array<int32_t,2> dims = {3,2};

  // in 2D (i.e., for matrices), fortran order means column by column
  CHECK( std_e::index_with_c_memory_layout({0,0},dims) == 0 );
  CHECK( std_e::index_with_c_memory_layout({0,1},dims) == 1 );

  CHECK( std_e::index_with_c_memory_layout({1,0},dims) == 2 );
  CHECK( std_e::index_with_c_memory_layout({1,1},dims) == 3 );

  CHECK( std_e::index_with_c_memory_layout({2,0},dims) == 4 );
  CHECK( std_e::index_with_c_memory_layout({2,1},dims) == 5 );
}
