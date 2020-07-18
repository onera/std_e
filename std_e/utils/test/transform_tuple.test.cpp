#include "std_e/unit_test/doctest.hpp"
#include "std_e/utils/transform_tuple.hpp"

// #include <iostream>
#include <algorithm>


template<typename T> auto my_conversion(T& t){
  // std::cout << __PRETTY_FUNCTION__ << std::endl;
  return t;
}

template<> auto my_conversion(int& t){
  // std::cout << __PRETTY_FUNCTION__ << std::endl;
  return static_cast<double>(t);
}

template<> auto my_conversion(double& t){
  // std::cout << __PRETTY_FUNCTION__ << std::endl;
  return static_cast<int>(t);
}

// template<typename T> void get_type(T&& t){
//   std::cout << __PRETTY_FUNCTION__ << std::endl;
// }


TEST_CASE("transform tuple") {
  SUBCASE("simple transform") {

    std::tuple<int, double, long> tp1;

    auto tf = tuple_transform([&](auto& x){
      // std::cout << __PRETTY_FUNCTION__ << std::endl;
      return my_conversion(x);
    }, tp1);

    // get_type(tp1);
    // get_type(tf);

    static_assert(std::is_same_v<decltype(tf), std::tuple<double, int, long>>);
  }

  // TODO Bruno https://external-preview.redd.it/ahuyfujKIPS1NWE-JbrVAaJ_D9PR-Vav-LMJuJmVU8c.jpg?auto=webp&s=b1f1f14dcf43d63865e92ebefa1762aea1e027fb
  SUBCASE("simple transform const") {

    const std::tuple<int, double, long> tp1;

    auto tf = tuple_transform([&](auto& x){
      // std::cout << __PRETTY_FUNCTION__ << std::endl;
      return my_conversion(x);
    }, tp1);

    // get_type(tp1);
    // get_type(tf);

    // Is not the same because const and my_conversion not declared as const !
    static_assert(!std::is_same_v<decltype(tf), std::tuple<double, int, long>>);
  }
}

