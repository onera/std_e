#include "std_e/unit_test/doctest.hpp"
#include "std_e/operation/logic.hpp"

using namespace std_e;

TEST_CASE("implies") {
  CHECK( implies(true ,true ) == true  );
  CHECK( implies(true ,false) == false );
  CHECK( implies(false,true ) == true  );
  CHECK( implies(false,false) == true  );
}

TEST_CASE("equiv") {
  CHECK( equiv(true ,true ) == true  );
  CHECK( equiv(true ,false) == false );
  CHECK( equiv(false,true ) == false);
  CHECK( equiv(false,false) == true  );
}

TEST_CASE("exclusive_or") {
  CHECK( exclusive_or(true ,true ) == false );
  CHECK( exclusive_or(true ,false) == true  );
  CHECK( exclusive_or(false,true ) == true  );
  CHECK( exclusive_or(false,false) == false );
}
