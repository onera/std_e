#pragma once


#include <type_traits>


namespace std_e {




template<class T> concept Sender = std::remove_cvref_t<T>::enable_sender;




template<class T>
class input_sender {
  private:
    T x;
  public:
    static constexpr bool enable_sender = true;

    input_sender(T&& x)
      : x(std::move(x))
    {}
    auto
    retrieve_data() -> T&& {
      return std::move(x);
    }
};


template<Sender S, class F>
class then_sender {
  private:
    S s;
    F f;
  public:
    static constexpr bool enable_sender = true;

    then_sender(S&& s, F&& f)
      : s(std::move(s))
      , f(std::move(f))
    {}

    auto
    dependent_sender() -> S& {
      return s;
    }
    auto
    continuation() -> F& {
      return f;
    }
};

template<Sender S, class F> auto
then(S&& s, F&& f) {
  return then_sender<S,F>(std::move(s),std::move(f));
}

template<class F> auto
then(F&& f) {
  return make_pipeable([f0=std::move(f)](auto&& s) {
    return then(std::move(s),std::move(f0));
  });
}




} // std_e
