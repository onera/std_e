#define DOCTEST_CONFIG_IMPLEMENT
#include "std_e/unit_test/doctest.hpp"

int main(int argc, char** argv) {
  doctest::Context ctx;
  ctx.applyCommandLine(argc, argv);
  return ctx.run();
}
