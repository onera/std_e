#include "doctest/doctest.h"
#include "std_e/utils/helper_bind.hpp"
#include <memory>
#include <iostream>
#include <cassert>

// Tests that don't naturally fit in the headers/.cpp files directly
// can be placed in a tests/*.cpp file. Integration tests are a good example.


void test_helper_bind_1(int& ncell, std::size_t& val){
  assert(ncell == 100);
  val *= 2;
};



void test_helper_bind_2(int& ncell, std::size_t& val, int* x, std::size_t* y){
  assert(ncell == 100);
  val  *= 2;
  x[0] *= 3;
  y[0] *= 4;
};


TEST_CASE("[helper_bind] make_bind with simple args ")
{
  int         n_cell            = 100;
  std::size_t val_can_be_change = 1;

  SUBCASE(" Without std::ref std::bind take a value ") {
    auto f_t1 = std_e::make_bind(test_helper_bind_1, n_cell, val_can_be_change);
    f_t1();
    CHECK(val_can_be_change == 1);
  }

  SUBCASE(" With std::ref std::bind take the proper address ") {
    auto f_t1 = std_e::make_bind(test_helper_bind_1, n_cell, std::ref(val_can_be_change));
    f_t1();
    CHECK(val_can_be_change == 2);
  }

}

template<typename ...Ts>
void debug_type(Ts&& ...){
  std::cout << __PRETTY_FUNCTION__ << std::endl;
}

TEST_CASE("[helper_bind] make_bind with simple args ")
{
  int         n_cell            = 100;
  std::size_t val_can_be_change = 1;

  SUBCASE(" Without std::ref std::bind take a value ") {
    auto t1   = std::make_tuple(n_cell, val_can_be_change);
    auto f_t1 = std_e::make_bind(test_helper_bind_1, t1);
    f_t1();
    CHECK(std::get<1>(t1) == 1);
  }

  SUBCASE(" With std::ref std::bind take the proper address ") {
    auto t1   = std::make_tuple(std::ref(n_cell), std::ref(val_can_be_change));
    auto f_t1 = std_e::make_bind(test_helper_bind_1, t1);

    // debug_type(t1);

    f_t1();
    // CHECK(std::get<1>(t1) == 2); // TODO See Bérenger
  }

}

TEST_CASE("[helper_bind] make_bind with 2 tuples ")
{
  int         n_cell            = 100;
  std::size_t val_can_be_change = 1;

  std::vector<int>         x(n_cell, -1);
  std::vector<std::size_t> y(n_cell, 10);

  SUBCASE(" Without std::ref std::bind take a value ") {
    auto t1   = std::make_tuple(n_cell, val_can_be_change);
    auto t2   = std::make_tuple(x.data(), y.data());
    auto f_t2 = std_e::make_bind(test_helper_bind_2, t1, t2);
    f_t2();
    CHECK(std::get<1>(t1)    == 1  );
    CHECK(std::get<0>(t2)[0] == -3 );
    CHECK(std::get<1>(t2)[0] ==  40);
    f_t2();
    CHECK(std::get<1>(t1)    == 1  );
    CHECK(std::get<0>(t2)[0] == -9 );
    CHECK(std::get<1>(t2)[0] ==  160);
  }

}


TEST_CASE("[helper_bind] straight_call with 2 tuples ")
{
  int         n_cell            = 100;
  std::size_t val_can_be_change = 1;

  std::vector<int>         x(n_cell, -1);
  std::vector<std::size_t> y(n_cell, 10);

  SUBCASE(" Without std::ref std::bind take a value ") {
    auto t1   = std::make_tuple(n_cell, val_can_be_change);
    auto t2   = std::make_tuple(x.data(), y.data());
    std_e::straight_call(test_helper_bind_2, t1, t2);

    CHECK(std::get<1>(t1)    == 1  );
    CHECK(std::get<0>(t2)[0] == -3 );
    CHECK(std::get<1>(t2)[0] ==  40);

  }

}
