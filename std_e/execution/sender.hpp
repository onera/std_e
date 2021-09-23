#pragma once


#include "std_e/execution/sender_concept.hpp"
#include "std_e/execution/pipeable.hpp"
#include "std_e/meta/meta.hpp"


namespace std_e {


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
    remove_rvalue_reference<S> s;
    F f;
  public:
    static constexpr bool enable_sender = true;

    then_sender(S s, F&& f)
      : s(FWD(s))
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
  return then_sender<S&&,F>(FWD(s),std::move(f));
}

template<class F> auto
then(F&& f) {
  return make_pipeable([f0=std::move(f)](auto&& s) {
    return then(FWD(s),std::move(f0));
  });
}




template<Sender S>
class split_sender {
  private:
    remove_rvalue_reference<S> s;
    bool completed = false;
    using result_type = decltype(execute(s));
    result_type res;
  public:
    static constexpr bool enable_sender = true;

    split_sender(S s)
      : s(FWD(s))
    {}

    auto
    dependent_sender() -> S& {
      return s;
    }

    auto
    has_completed() const -> bool {
      return completed;
    }
    auto
    execute_dep() {
      res = execute(s);
      completed = true;
    }

    auto
    result() & -> result_type& {
      STD_E_ASSERT(completed);
      return res;
    }
    auto
    result() && -> result_type&& {
      STD_E_ASSERT(completed);
      return std::move(res);
    }
};

template<Sender S> auto
split(S&& s) {
  return split_sender<S&&>(FWD(s));
}

inline auto
split() {
  return make_pipeable([](auto&& s) {
    return split(FWD(s));
  });
}




template<Sender... Ss>
class wait_all_sender {
  private:
    std::tuple<remove_rvalue_reference<Ss>...> ss;
  public:
    static constexpr bool enable_sender = true;

    wait_all_sender(Ss... ss)
      : ss(FWD(ss)...)
    {}

    template<int I> auto
    dependent_sender() -> auto& {
      return std::get<I>(ss);
    }
};

template<Sender... Ss> auto
wait_all(Ss&&... ss) {
  return wait_all_sender<Ss&&...>(FWD(ss)...);
}




template<Sender S, class F>
class then_comm_sender : public then_sender<S,F> {
  public:
    using base = then_sender<S,F>;
    using base::base;

    static constexpr bool enable_sender = true;
};


template<Sender S, class F> auto
then_comm(S&& s, F&& f) {
  return then_comm_sender<S&&,F>(FWD(s),std::move(f));
}

template<class F> auto
then_comm(F&& f) {
  return make_pipeable([f0=std::move(f)](auto&& s) {
    return then_comm(FWD(s),std::move(f0));
  });
}


} // std_e
