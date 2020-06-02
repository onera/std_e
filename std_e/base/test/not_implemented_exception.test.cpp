#include "std_e/unit_test/doctest.hpp"

#include "std_e/base/not_implemented_exception.hpp"

void f_test_unimplemented(){
  throw std_e::not_implemented_exception("f_test_unimplemented");
}

TEST_CASE("not_implemented_exception") {
  CHECK_THROWS_WITH_AS(f_test_unimplemented(), "f_test_unimplemented", std_e::not_implemented_exception);
}
