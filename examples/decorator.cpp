#include <tuple>
#include <iostream>
#include <utility>
#include <vector>
#include <array>
#include <iostream>
#include <memory>
#include <cassert>

#include "fmt/format.h"
#include "fmt/ranges.h"

// ---------------------------------------------------------------------------
template<typename F>
auto stars(F& func) {
  return [func](auto&&... args) {
    std::cout << "*******" << std::endl;
    // Faire avec type de retour !!
    func(std::forward<decltype(args)>(args)...);
    std::cout << "\n*******" << std::endl;
  };
}

// ---------------------------------------------------------------------------
void hello_impl() {
   std::cout << "hello, world!";
}

// ---------------------------------------------------------------------------
int get_impl(int i) {
  return i;
}
// ---------------------------------------------------------------------------
auto hello       = stars(hello_impl);
auto super_get   = stars(get_impl);

// ===========================================================================
int main(void)
{
  hello();
  // int ci = super_get(10);
  // assert(ci == 10);
}
