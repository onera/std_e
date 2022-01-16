#include "std_e/unit_test/doctest.hpp"
#include "std_e/__attempt/reference_range/reference_range.hpp"


#include <algorithm>


using namespace cgns;

//namespace {

struct S {
  int i;
  S(int i)
    : i(i)
  {}

  S() = default;
  S(const S&) = delete;
  S& operator=(const S&) = delete;
  S(S&&) = default;
  S& operator=(S&&) = default;
};

template<typename _Out, typename _Tp>
concept partial_indirectly_writable = requires(_Out&& __o, _Tp&& __t)
{
  *__o = std::forward<_Tp>(__t);
};

//TEST_CASE("tree_range") {
//  S s0 {0};
//  S s1 {1};
//
//  Ref_range<S> sr;
//  sr.emplace_back(s0);
//  sr.emplace_back(s1);
//
//  SUBCASE("partition") {
//    auto greater_name_than_0 = [](const S& x){ return x.i > 0; };
//    std::partition(begin(sr),end(sr),greater_name_than_0);
//    std::ranges::partition(sr,greater_name_than_0);
//  }
//  SUBCASE("sort") {
//    // Does not work, for good reasons
//    using _Tp = std::reference_wrapper<S>;
//    using _Out = cgns::Ref_range_iterator<S, __gnu_cxx::__normal_iterator<std::reference_wrapper<S>*, std::vector<std::reference_wrapper<S> > > >;
//    static_assert(partial_indirectly_writable<_Out,_Tp>);
//
//    // Does not work, same reasons
//    auto less_name = [](const S& x, const S& y){ return x.i < y.i; };
//    std::sort(begin(sr),end(sr),less_name);
//
//    // Does not work, same reasons
//    std::ranges::sort(sr,less_name);
//  }
//}

//}
