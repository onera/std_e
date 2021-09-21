#include "std_e/unit_test/doctest.hpp"

#include "std_e/log.hpp"


class null_sender {
  public:
};

template<class Sender, class F>
class then_sender {
  private:
    Sender s;
    F f;
  public:
    template<class S0, class F0>
    then_sender(S0&& s, F0&& f)
      : s(FWD(s))
      , f(FWD(f))
    {}
    auto
    dependent_sender() -> Sender& {
      return s;
    }
    auto
    continuation() -> F& {
      return f;
    }
};

template<class Sender>
struct is_sender__impl {
  static constexpr bool value = true;
};
template<>
struct is_sender__impl<null_sender> {
  static constexpr bool value = true;
};
template<class S, class F>
struct is_sender__impl<then_sender<S,F>> {
  static constexpr bool value = true;
};

template<class Sender> constexpr bool is_sender = is_sender__impl<std::remove_cvref<Sender>>::value;


template<class Sender, class F>
requires is_sender<Sender>
auto
then(Sender&& s, F&& f) {
  return then_sender<Sender,F>(FWD(s),FWD(f));
}

//  return ranges::make_pipeable([](auto&& rng) {
//    return to_cgns_vector(FWD(rng));
//  });
//
//template<
//  class Sender, class F,
//  std::enable_if_t<is_sender<Sender>,int> =0
//> auto
//operator|(Sender&& s, F&& f) {
//  return then(FWD(s),FWD(f));
//}

class receiver {
  public:
    //template<class Sender> auto
    //execute(Sender& s) {
    //}
    //
    auto
    execute(null_sender& s) {
    }
    template<class S, class F> auto
    execute(then_sender<S,F>& s) {
      this->execute(s.dependent_sender());
      s.continuation()();
    }
};

constexpr auto print_hello = [](){
  LOG("hello there");
};

auto
test_sender(auto&& s) { // -> sender {
  return then(s,print_hello);
}

TEST_CASE("sender and execution") {
  null_sender s;
  auto s0 = test_sender(s);
  receiver r;
  r.execute(s0);
}
