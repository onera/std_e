#pragma once


namespace std_e {


template<class I>
// requires I is integer type
class uniform_distribution_generator {
  private:
    I i;
    I spacing_round_down;
    I n_round_up;
  public:
  // ctors
    constexpr
    uniform_distribution_generator()
      : i(0)
      , spacing_round_down(0)
      , n_round_up(0)
    {}

    constexpr
    uniform_distribution_generator(I start, I spacing_round_down, I n_round_up)
      : i(start)
      , spacing_round_down(spacing_round_down)
      , n_round_up(n_round_up)
    {}

  // generator interface
    constexpr auto
    operator++() -> uniform_distribution_generator& {
      if (n_round_up>0) {
        --n_round_up;
        i += spacing_round_down+1;
      } else {
        i += spacing_round_down;
      }
      return *this;
    }
    constexpr auto
    operator++(int) -> uniform_distribution_generator {
      uniform_distribution_generator tmp(*this);
      ++*this;
      return tmp;
    }

    constexpr auto
    operator*() const -> I {
      return i;
    }

    constexpr auto operator<=>(const uniform_distribution_generator& x) const = default;
};



template<class I>
struct uniform_distribution_generator_sentinel {
  I size;
};

template<class I> constexpr auto
operator==(const uniform_distribution_generator<I>& gen, uniform_distribution_generator_sentinel<I> sentinel) {
  return *gen==sentinel.size;
}
template<class I> constexpr auto
operator==(uniform_distribution_generator_sentinel<I> sentinel, const uniform_distribution_generator<I>& gen) {
  return gen==sentinel;
}
template<class I> constexpr auto
operator!=(const uniform_distribution_generator<I>& gen, uniform_distribution_generator_sentinel<I> sentinel) {
  return !(gen==sentinel);
}
template<class I> constexpr auto
operator!=(uniform_distribution_generator_sentinel<I> sentinel, const uniform_distribution_generator<I>& gen) {
  return !(gen==sentinel);
}




template<class I>
class uniform_distribution_range {
  private:
    I start;
    I n_elt;
    I spacing_round_down;
    I n_round_up;
    I after_finish;
  public:
  // ctors
    constexpr
    uniform_distribution_range()
      : start(0)
      , n_elt(0)
      , spacing_round_down(0)
      , n_round_up(0)
      , after_finish(0)
    {}

    constexpr
    uniform_distribution_range(I start, I sz, I n_elt)
      : start(start)
      , n_elt(n_elt)
      , spacing_round_down( n_elt==0 ? 0 : sz / n_elt )
      , n_round_up        ( n_elt==0 ? 0 : sz % n_elt )
      , after_finish(start + sz + spacing_round_down)
    {
      STD_E_ASSERT(sz>0);
      STD_E_ASSERT(n_elt>0);
    }

    constexpr
    uniform_distribution_range(I sz, I n_elt)
      : uniform_distribution_range(0,sz,n_elt)
    {}

  // range interface
    constexpr auto
    size() const -> I {
      return n_elt;
    }
    constexpr auto
    begin() const -> uniform_distribution_generator<I> {
      return {start,spacing_round_down,n_round_up};
    }
    constexpr auto
    end() const -> uniform_distribution_generator_sentinel<I> {
      return {after_finish}; // we want the distribution to *include* finish, so end() has to return one past finish
    }
};

template<class I> constexpr auto
make_uniform_distribution_generator(I start, I sz, I n_elt) -> uniform_distribution_generator<I> {
  return uniform_distribution_range<I>(start,sz,n_elt).begin();
}
template<class I> constexpr auto
make_uniform_distribution_generator(I sz, I n_elt) -> uniform_distribution_generator<I> {
  return uniform_distribution_range<I>(sz,n_elt).begin();
}

} // std_e
