#include "std_e/unit_test/doctest.hpp"
#include "std_e/logging/time_logger.hpp"

#include "std_e/__attempt/quicksort/quicksort.hpp"


#include <algorithm>


using namespace std_e;


TEST_CASE("quicksort") {
  // Results
  //   Intel broadwell (onera spiro)
  //   size = 100'000'000;
  // Times
  //   handmade         : 13s
  //   std::sort        : 11s
  //   std::partial_sort: 33s
  //   std::stable_sort : 13s

  //const int size = 100'000'000;
  const int size = 100;

  {
    auto x = random_vector(size);
    auto _ = std_e::stdout_time_logger("handmade std_e quicksort");
    quicksort(x.begin(),x.end());
  }

  {
    auto x = random_vector(size);
    auto _ = std_e::stdout_time_logger("std::sort");
    std::sort(x.begin(),x.end());
  }

  {
    auto x = random_vector(size);
    auto _ = std_e::stdout_time_logger("std::partial_sort");
    std::partial_sort(x.begin(),x.end(),x.end());
  }

  {
    auto x = random_vector(size);
    auto _ = std_e::stdout_time_logger("std::stable_sort");
    std::stable_sort(x.begin(),x.end());
  }
}
