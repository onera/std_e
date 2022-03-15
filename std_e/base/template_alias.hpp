#pragma once

#include "std_e/future/span.hpp"
#include "std_e/future/span_ref.hpp"

namespace std_e {

// Clang bug workaround
//   Clang does not manage to instanciate a template<class> template parameter
//   with std::vector<T> because it is actually a std::vector<T,A> with defaulted A
//   so it rather expects a template<class,class> template parameter
// Typical error message: "template template argument has different template parameters than its corresponding template template parameter"
template<class T> using std_alloc_vector = std::vector<T>;
template<class T> using dyn_span = std_e::span<T>;
template<class T> using dyn_span_ref = std_e::span_ref<T>;

}
