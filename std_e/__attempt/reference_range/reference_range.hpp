#pragma once


#include <vector>
#include <type_traits>


namespace std_e {

// Note: This will never work well with swapping (hence, can't partition/sort/... a Reference_range)
// Technically, the requirement
//   requires(Ref_range_iterator<T>* o, ref_wrapper<T> x) {
//     *o = x;
//   }
// will not do the same thing if *o is of type T& or of type ref_wrapper<T>
// Fundamentally, what happens is that we would like to discriminate between reading and writing,
//   but both can't be distinguished, because they both use *
template<class T, class vector_iter_type>
class Ref_range_iterator {
  private:
    using I = typename vector_iter_type::difference_type;
    vector_iter_type v_it;
  public:
    /// std::iterator type traits
    using value_type = std::reference_wrapper<T>;
    using reference = std::reference_wrapper<T>&;
    //using reference = T&;
    using difference_type = I;
    using iterator_category = std::random_access_iterator_tag;

    Ref_range_iterator() = default;
    Ref_range_iterator(vector_iter_type v_it)
      : v_it(v_it)
    {}

    auto operator++() -> Ref_range_iterator& {
      ++v_it;
      return *this;
    }
    auto operator++(int) -> Ref_range_iterator {
      Ref_range_iterator tmp(*this);
      ++*this;
      return tmp;
    }
    auto operator--() -> Ref_range_iterator& {
      --v_it;
      return *this;
    }
    auto operator--(int) -> Ref_range_iterator {
      Ref_range_iterator tmp(*this);
      --*this;
      return tmp;
    }

    auto operator+=(I i) -> Ref_range_iterator& {
      v_it += i;
      return *this;
    }
    auto operator-=(I i) -> Ref_range_iterator& {
      v_it -= i;
      return *this;
    }
    friend
    auto operator+(Ref_range_iterator it, I i) -> Ref_range_iterator {
      Ref_range_iterator res = it;
      res += i;
      return res;
    }
    friend
    auto operator+(I i, Ref_range_iterator it) -> Ref_range_iterator {
      return it + i;
    }
    friend
    auto operator-(Ref_range_iterator it, I i) -> Ref_range_iterator {
      Ref_range_iterator res = it;
      res -= i;
      return res;
    }
    friend
    auto operator-(I i, Ref_range_iterator it) -> Ref_range_iterator {
      return it - i;
    }
    friend
    auto operator-(Ref_range_iterator it0, Ref_range_iterator it1) -> I {
      return it0.v_it - it1.v_it;
    }

    auto operator*() const -> T& {
      return *v_it;
    }
    auto operator[](I i) const -> T& {
      return v_it[i];
    }
    //auto operator[](I i) -> T& {
    //  return v_it[i];
    //}
    //auto operator[](I i) const -> const T& {
    //  return v_it[i];
    //}

    constexpr auto
    operator<=>(const Ref_range_iterator& x) const = default;
};


} // std_e
template<class T,class vector_iter_type>
struct std::iterator_traits<std_e::Ref_range_iterator<T,vector_iter_type>> {
  using type = std_e::Ref_range_iterator<T,vector_iter_type>;
  using value_type = typename type::value_type;
  using reference = typename type::reference;
  using difference_type = typename type::difference_type;
  using iterator_category = typename type::iterator_category;
};
namespace std_e {


template<class T>
class Reference_range {
  private:
  // type traits
    using vec_type = std::vector<T*>;
    using vec_iterator = typename vec_type::iterator;
  // data member
    vec_type _impl;
  public:
  // type traits
    using iterator = Ref_range_iterator<T,vec_iterator>;

  // ctors
    Reference_range() = default;
    Reference_range(size_t n)
      : _impl(n, nullptr)
    {}

  // array
    auto size() const -> int {
      return _impl.size();
    }
    auto operator[](int i) -> T& {
      return *_impl[i];
    }

  // range
    auto begin() -> iterator {
      return {_impl.begin()};
    }
    auto end() -> iterator {
      return {_impl.end()};
    }

  // container
    auto emplace_back(T& t) -> T& {
      return _impl.emplace_back(&t);
    }
    auto emplace_back(T&&) -> T& = delete; // refuse temporaries
};

template<class T> auto
begin(Reference_range<T>& x) {
  return x.begin();
}
template<class T> auto
end(Reference_range<T>& x) {
  return x.end();
}


} // std_e
