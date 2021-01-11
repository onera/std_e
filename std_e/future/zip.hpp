#pragma once


// Basic zip range functionality
// Does not depend on ranges-v3, pipes or Boost
// But very limited
#include <utility>


namespace std_e {




template<class T0, class T1>
struct zpair : std::pair<T0,T1> {
  using base = std::pair<T0,T1>;
  using base::base;
};

template<class T0, class T1>
struct zpair<T0&,T1&> : std::pair<T0&,T1&> {
  using base          = std::pair<T0&,T1&>;
  using base::base;

  zpair(const zpair<T0,T1>& x)
    : base::first (x.first)
    , base::second(x.second)
  {}

  auto
  operator=(const zpair<T0,T1>& x) -> auto& {
    this->first  = x.first;
    this->second = x.second;
    return *this;
  }
};


template<class T0, class T1> auto
operator==(const zpair<T0,T1>& x, const zpair<T0&,T1&>& y) -> bool {
  return x.first==y.first && x.second==y.second;
}
template<class T0, class T1> auto
operator==(const zpair<T0,T1>& x, const zpair<const T0&,const T1&>& y) -> bool {
  return x.first==y.first && x.second==y.second;
}
template<class T0, class T1> auto
operator==(const zpair<T0&,T1&>& x, const zpair<T0,T1>& y) -> bool {
  return x.first==y.first && x.second==y.second;
}
template<class T0, class T1> auto
operator==(const zpair<const T0&,const T1&>& x, const zpair<T0,T1>& y) -> bool {
  return x.first==y.first && x.second==y.second;
}

template<class T0, class T1> auto
operator!=(const zpair<T0,T1>& x, const zpair<T0&,T1&>& y) -> bool {
  return !(x==y);
}
template<class T0, class T1> auto
operator!=(const zpair<T0,T1>& x, const zpair<const T0&,const T1&>& y) -> bool {
  return !(x==y);
}
template<class T0, class T1> auto
operator!=(const zpair<T0&,T1&>& x, const zpair<T0,T1>& y) -> bool {
  return !(x==y);
}
template<class T0, class T1> auto
operator!=(const zpair<const T0&,const T1&>& x, const zpair<T0,T1>& y) -> bool {
  return !(x==y);
}




template<class It_0, class It_1>
class zip_iterator {
  public:
    using T0 = typename std::iterator_traits<It_0>::value_type;
    using T1 = typename std::iterator_traits<It_1>::value_type;
    using reference_0 = typename std::iterator_traits<It_0>::reference;
    using reference_1 = typename std::iterator_traits<It_1>::reference;

  // std::iterator type traits
    using value_type = zpair<T0,T1>;
    using difference_type = typename std::iterator_traits<It_0>::difference_type;
    using iterator_category = std::forward_iterator_tag;

    using reference = zpair<reference_0,reference_1>;

    zip_iterator(It_0 it_0, It_1 it_1)
      : it_0(it_0)
      , it_1(it_1)
    {}

    auto operator*() const -> reference {
      return {*it_0,*it_1};
    }
    auto operator[](int i) const -> reference {
      return {it_0[i],it_1[i]};
    }

    auto operator++() -> auto& {
      ++it_0;
      ++it_1;
      return *this;
    }

    template<class It_0_, class It_1_> friend auto
    operator==(const zip_iterator<It_0_,It_1_>& x, const zip_iterator<It_0_,It_1_>& y);
  private:
    It_0 it_0;
    It_1 it_1;
};
template<class It_0, class It_1> auto
operator==(const zip_iterator<It_0,It_1>& x, const zip_iterator<It_0,It_1>& y) {
  return x.it_0 == y.it_0;
}
template<class It_0, class It_1> auto
operator!=(const zip_iterator<It_0,It_1>& x, const zip_iterator<It_0,It_1>& y) {
  return !(x==y);
}
template<class It_0, class It_1> auto
make_zip_iterator(It_0 it_0, It_1 it_1) {
  return zip_iterator<It_0,It_1>(it_0,it_1);
}

} /* std_e */ namespace std {
template<class It_0, class It_1>
struct iterator_traits<std_e::zip_iterator<It_0,It_1>> {
  using type = std_e::zip_iterator<It_0,It_1>;

  using value_type = typename type::value_type;
  using difference_type = typename type::difference_type;
  using iterator_category = typename type::iterator_category;
  using reference = typename type::reference;
};
} /* std */ namespace std_e {


template<class Rng0, class Rng1>
class zip_view{
  public:
    using T0 = typename Rng0::value_type;
    using T1 = typename Rng1::value_type;
    using It_0 = typename Rng0::iterator;
    using It_1 = typename Rng1::iterator;

    using value_type = zpair<T0,T1>;
    using reference = zpair<T0&,T1&>;
    using const_reference = zpair<const T0&,const T1&>;
    using iterator = zip_iterator<It_0,It_1>;

    zip_view(Rng0& r0, Rng1& r1)
      : first_0(r0.begin())
      ,  last_0(r0.end  ())
      , first_1(r1.begin())
      ,  last_1(r1.end  ())
    {}

    auto
    size_0() const -> size_t {
      return std::distance(first_0,last_0);
    }
    auto
    size_1() const -> size_t {
      return std::distance(first_1,last_1);
    }
    auto 
    size() const -> size_t {
      return std::min(size_0(),size_1());
    }

    auto
    operator[](int i) -> reference {
      return {first_0[i],first_1[i]};
    }
    auto
    operator[](int i) const -> const_reference {
      return {first_0[i],first_1[i]};
    }

    auto
    begin() -> iterator {
      return {first_0       ,first_1       };
    }
    auto
    end() -> iterator {
      return {first_0+size(),first_1+size()};
    }


  private:
    It_0 first_0;  It_0 last_0;
    It_1 first_1;  It_1 last_1;
};


template<class Rng0, class Rng1> auto
zip(Rng0& r0, Rng1& r1) {
  return zip_view<Rng0,Rng1>(r0,r1);
}


} // std_e
