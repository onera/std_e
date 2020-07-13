#include "std_e/unit_test/doctest.hpp"
#include "std_e/utils/transform_tuple.hpp"

// #include <iostream>
#include <algorithm>


template<typename T> auto my_convertion(T& t){
  // std::cout << __PRETTY_FUNCTION__ << std::endl;
  return t;
}

template<> auto my_convertion(int& t){
  // std::cout << __PRETTY_FUNCTION__ << std::endl;
  return static_cast<double>(t);
}

template<> auto my_convertion(double& t){
  // std::cout << __PRETTY_FUNCTION__ << std::endl;
  return static_cast<int>(t);
}

// template<typename T> void get_type(T&& t){
//   std::cout << __PRETTY_FUNCTION__ << std::endl;
// }


TEST_CASE("transform tuple ") {
  SUBCASE("simple transform ") {

    std::tuple<int, double, long> tp1;

    auto tf = tuple_transform([&](auto& x){
      // std::cout << __PRETTY_FUNCTION__ << std::endl;
      return my_convertion(x);
    }, tp1);

    // get_type(tp1);
    // get_type(tf);

    static_assert(std::is_same<decltype(tf), std::tuple<double, int, long>>::value);

  }

  SUBCASE("simple transform const ") {

    const std::tuple<int, double, long> tp1;

    auto tf = tuple_transform([&](auto& x){
      // std::cout << __PRETTY_FUNCTION__ << std::endl;
      return my_convertion(x);
    }, tp1);

    // get_type(tp1);
    // get_type(tf);

    // Is not the same because const and my_conversion not declared as const !
    static_assert(!std::is_same<decltype(tf), std::tuple<double, int, long>>::value);

  }

}

