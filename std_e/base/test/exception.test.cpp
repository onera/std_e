#include "std_e/unit_test/doctest.hpp"


#include "std_e/base/not_implemented_exception.hpp"
#include <string>
#include <vector>
#include <algorithm>


using namespace std;

void f_test_exception(){
  throw std_e::not_implemented_exception("f_test_exception");
}

TEST_CASE("[not_implemented_exception]") {

  try {
    printf(" oooooooooooooooo \n");
    f_test_exception();
  } catch(const std_e::not_implemented_exception& e ) {
    std::string err_msg = e.what();
    CHECK(err_msg == "f_test_exception");
  }

}
