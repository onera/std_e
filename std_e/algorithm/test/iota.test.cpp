#include "doctest/doctest.h"
#include "std_e/utils/doctest_utils.hpp"
#include "std_e/algorithm/iota.hpp"
#include <vector>

using namespace std_e;

TEST_CASE("exclusive_iota") {
  SUBCASE("full, start 0") {
    std::vector<int> v(4);

    std_e::exclusive_iota(begin(v),end(v),0,10);

    std::vector<int> v_expected = {0,10,20,30};
    CHECK( v == v_expected );
  }

  SUBCASE("full, start 1") {
    std::vector<int> v(4);

    std_e::exclusive_iota(begin(v),end(v),1,10);

    std::vector<int> v_expected = {1,11,21,31};
    CHECK( v == v_expected );
  }

  SUBCASE("partial, start 1") {
    std::vector<int> v = {100,101,102,103};

    std_e::exclusive_iota(begin(v)+1,end(v)-1,1,10);

    std::vector<int> v_expected = {100,1,11,103};
    CHECK( v == v_expected );
  }

  SUBCASE("empty") {
    std::vector<int> v = {100,101,102,103};

    std_e::exclusive_iota(begin(v)+2,end(v)-2,1,10);

    std::vector<int> v_expected = {100,101,102,103};
    CHECK( v == v_expected );
  }
}

TEST_CASE("exclusive_iota_n") {
  SUBCASE("full, start 0") {
    std::vector<int> v(4);

    auto last = std_e::exclusive_iota_n(begin(v),4,0,10);

    std::vector<int> v_expected = {0,10,20,30};
    CHECK( v == v_expected );
    CHECK( last == end(v) );
  }

  SUBCASE("full, start 1") {
    std::vector<int> v(4);

    auto last = std_e::exclusive_iota_n(begin(v),4,1,10);

    std::vector<int> v_expected = {1,11,21,31};
    CHECK( v == v_expected );
    CHECK( last == end(v) );
  }

  SUBCASE("partial, start 1") {
    std::vector<int> v = {100,101,102,103};

    auto last = std_e::exclusive_iota_n(begin(v)+1,2,1,10);

    std::vector<int> v_expected = {100,1,11,103};
    CHECK( v == v_expected );
    CHECK( last == begin(v)+3 );
  }

  SUBCASE("empty") {
    std::vector<int> v = {100,101,102,103};

    auto last = std_e::exclusive_iota_n(begin(v)+2,0,1,10);

    std::vector<int> v_expected = {100,101,102,103};
    CHECK( v == v_expected );
    CHECK( last == begin(v)+2 );
  }
}

TEST_CASE("inclusive_iota") {
  std::vector<int> v(4);

  std_e::inclusive_iota(begin(v),end(v),1,10);

  std::vector<int> v_expected = {11,21,31,41};
  CHECK( v == v_expected );
}
TEST_CASE("inclusive_iota_n") {
  std::vector<int> v(4);

  auto last = std_e::inclusive_iota_n(begin(v),4,1,10);

  std::vector<int> v_expected = {11,21,31,41};
  CHECK( v == v_expected );
  CHECK( last == end(v) );
}


TEST_CASE("uniform_distribution") {
  SUBCASE("empty remainder") {
    int nb_slots = 3;
    std::vector<int> v(1+nb_slots);

    uniform_distribution(begin(v),end(v),100,15);
  // distributing 15 over 3 slots gives 5 + 5 + 5
  //                                    |   |   |
  //                                    v   v   v
    std::vector<int> v_expected = {100,105,110,115};
    CHECK( v == v_expected );
  }

  SUBCASE("non-empty remainder") {
    int nb_slots = 3;
    std::vector<int> v(1+nb_slots);

    uniform_distribution(begin(v),end(v),100,17);

  // distributing 17 over 3 slots gives 6 + 6 + 5
  //                                    |   |   |
  //                                    v   v   v
    std::vector<int> v_expected = {100,106,112,117};
    CHECK( v == v_expected );
  }

  SUBCASE("no slots") {
    int nb_slots = 0;
    std::vector<int> v(1+nb_slots);

    uniform_distribution(begin(v),end(v),100,0);

  // distributing  0 over 0 slots gives nothing
  //                                    |
  //                                    v
    std::vector<int> v_expected = {100};
    CHECK( v == v_expected );
  }
}
