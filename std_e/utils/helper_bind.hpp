#pragma once

#include <tuple>
#include <functional>

#include "std_e/utils/tuple.hpp"
#include "std_e/meta/flatten_tuple.hpp"
#include "std_e/utils/transform_tuple.hpp"


namespace std_e {

template<typename F, typename... Args>
constexpr
auto
make_bind(F&& f, Args&&... args){
  auto flat = std_e::flatten(std::forward<Args>(args)...);
  return std::apply([&](auto&&... vargs){
    return std::bind(f, std::forward<decltype(vargs)>(vargs)...);
  }, flat);
}

template<typename F, typename... Args>
constexpr
void
straight_call(F&& f, Args&&... args){
  auto flat = std_e::flatten(std::forward<Args>(args)...);
  std::apply([&](auto&&... vargs){
    f(std::forward<decltype(vargs)>(vargs)...);
  }, flat);
}

} // std_e
