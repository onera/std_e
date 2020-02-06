#include "doctest/doctest.h"
#include "std_e/future/span.hpp"

#include <vector>
#include <algorithm>
// #include <iostream>

TEST_CASE("Span size") {
  constexpr auto sp1size = std_e::span_size<1>();
  constexpr auto sp2size = std_e::span_size<1>();
  CHECK( sp1size == sp2size );
}

TEST_CASE("Span constructor") {

  std::vector<double>       a1  = {0 , 1, 2, 3, 4};
  const std::vector<double> ca1 = {0 , 1, 2, 3, 4};
  std::vector<double>       a2  = {-1, 6, 3,-1,-4};
  const std::vector<double> ca2 = {-1, 6, 3,-1,-4};

  double* pta1        = a1.data();
  const double* ptca1 = ca1.data();

  SUBCASE("Span from ptr static ") {
    auto spc1  = std_e::make_span<3>(pta1);
    auto cspc1 = std_e::make_span<3>(ptca1);

    CHECK( spc1.size()  == 3 );
    CHECK( cspc1.size() == 3 );

    CHECK( spc1[0] == a1[0] );
    CHECK( spc1[1] == a1[1] );
    CHECK( spc1[2] == a1[2] );

    CHECK( cspc1[0] == ca1[0] );
    CHECK( cspc1[1] == ca1[1] );
    CHECK( cspc1[2] == ca1[2] );

    CHECK( spc1.data()  == pta1  );
    CHECK( cspc1.data() == ptca1 );

    CHECK( spc1.begin()  == pta1  );
    CHECK( cspc1.begin() == ptca1 );

    CHECK( spc1.end()  == pta1 +3 );
    CHECK( cspc1.end() == ptca1+3 );
  }

  SUBCASE("Span from ptr") {
    auto sp1 = std_e::make_span(pta1, 5);
    auto csp1 = std_e::make_span(ptca1, 5);

    CHECK( sp1.size()  == a1.size() );
    CHECK( csp1.size() == ca1.size() );

    SUBCASE("Span from ptr with offset") {
      auto spo1  = std_e::make_span(pta1, 3, 2);
      auto cspo1 = std_e::make_span(ptca1, 3, 2);

      CHECK( spo1.size()  == 2 );
      CHECK( cspo1.size() == 2 );

      CHECK( spo1.begin()  == pta1+3  );
      CHECK( cspo1.begin() == ptca1+3 );

      CHECK( spo1.data()  == pta1+3  );
      CHECK( cspo1.data() == ptca1+3 );

      CHECK( spo1.end()  == pta1+5  );
      CHECK( cspo1.end() == ptca1+5 );

      CHECK( spo1[0] == a1[3] );
      CHECK( spo1[1] == a1[4] );

      CHECK( cspo1[0] == ca1[3] );
      CHECK( cspo1[1] == ca1[4] );
    }
  }

  SUBCASE("Span from container") {
    auto sd1  = std_e::make_span(a1);
    auto csd1 = std_e::make_span(ca1);

    CHECK( sd1[0] == a1[0] );
    CHECK( sd1[1] == a1[1] );
    CHECK( sd1[2] == a1[2] );
    CHECK( sd1[3] == a1[3] );
    CHECK( sd1[4] == a1[4] );

    CHECK( csd1[0] == ca1[0] );
    CHECK( csd1[1] == ca1[1] );
    CHECK( csd1[2] == ca1[2] );
    CHECK( csd1[3] == ca1[3] );
    CHECK( csd1[4] == ca1[4] );

    CHECK( sd1.size()  == a1.size() );
    CHECK( csd1.size() == ca1.size() );

    CHECK( sd1.data()  == pta1  );
    CHECK( csd1.data() == ptca1 );

    CHECK( sd1.begin()  == pta1  );
    CHECK( csd1.begin() == ptca1 );
  }

  SUBCASE("Span comparison") {
    auto st1   = std_e::make_span(a1);
    auto st2   = std_e::make_span(a1);
    auto stc1  = std_e::make_span(ca1);
    auto stc2  = std_e::make_span(ca1);

    auto st3   = std_e::make_span(a2);
    auto stc3  = std_e::make_span(ca2);

    CHECK( st1  == st2 );
    CHECK( stc1 == st2 );
    CHECK( stc1 == stc2 );

    CHECK( st1 != st3   );
    CHECK( stc1 != stc3 );
  }

}
