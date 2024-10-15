#pragma once


#include <cstdint>
#include <cstddef>
#include <algorithm>
#include <initializer_list>


namespace std_e {


template<class T>
class dynarray {
  public:
  // type traits
    using value_type      =       T ;
    using       iterator  =       T*;
    using const_iterator  = const T*;
    using       reference = const T&;
    using const_reference = const T&;

    using size_type       = size_t;
    using difference_type = ptrdiff_t;

  // ctors and assignment
    dynarray()
      : ptr(nullptr)
      , sz(0)
    {}
    dynarray(size_t sz)
      : sz(sz)
    {
      this->ptr = allocate(sz);
    }
    ~dynarray() {
      deallocate(this->ptr);
    }

    dynarray(const dynarray& old) 
      : dynarray(old.sz)
    {
      std::copy(old.ptr, old.ptr+old.sz, ptr);
    }

    dynarray& operator=(const dynarray& old) {
      if (sz != old.sz) {
        sz = old.sz;
        deallocate(this->ptr);
        this->ptr = allocate(sz);
      }
      std::copy(old.ptr, old.ptr+old.sz, ptr);
      return *this;
    }

    dynarray(dynarray&& old)
      : ptr(old.ptr)
      , sz(old.sz)
    {
      old.ptr = nullptr;
      old.sz = 0;
    }
    dynarray& operator=(dynarray&& old)
    {
      std::swap(ptr, old.ptr);
      std::swap(sz, old.sz);
      deallocate(old.ptr);
      old.ptr = nullptr;
      return *this;
    }

    template<class Input_it, class S, std::enable_if_t< !std::is_integral_v<Input_it> , int > =0>
    dynarray(Input_it first, S last)
      : dynarray(last - first)
    {
      std::copy(first, last, ptr);
    }

    dynarray(const std::initializer_list<T>& l)
      : dynarray(l.begin(), l.end())
    {}

    template<class I, std::enable_if_t< std::is_integral_v<I> , int > =0>
    dynarray(I sz, const T& x)
      : dynarray(sz)
    {
      std::fill(ptr, ptr+sz, x);
    }

    auto
    resize(size_t new_sz) -> void {
      T* new_ptr = allocate(new_sz);
      size_t min_sz = std::min(this->sz,new_sz); 
      std::copy_n(this->ptr, min_sz, new_ptr);

      deallocate(this->ptr);
      this->ptr = new_ptr;
      this->sz = new_sz;
    }

  // size
    auto size () const -> size_t   { return sz; }

  // iterators and pointers
    auto data () const -> const T* { return ptr; }
    auto data ()       ->       T* { return ptr; }
    auto begin() const -> const T* { return ptr; }
    auto begin()       ->       T* { return ptr; }
    auto end  () const -> const T* { return ptr+sz; }
    auto end  ()       ->       T* { return ptr+sz; }

  // access
    auto operator[](int i) const -> const T& { return ptr[i]; }
    auto operator[](int i)       ->       T& { return ptr[i]; }
    auto front()           const -> const T& { return ptr[0]; }
    auto front()                 ->       T& { return ptr[0]; }
    auto back ()           const -> const T& { return ptr[sz-1]; }
    auto back ()                 ->       T& { return ptr[sz-1]; }


    constexpr auto operator==(const dynarray& x) const {
      return std::equal(begin(), end(), x.begin(), x.end());
    }
    constexpr auto operator<=>(const dynarray& x) const {
      return std::lexicographical_compare_three_way(begin(), end(), x.begin(), x.end());
    }
  private:
  // static helper functions
    static auto allocate(size_t n) -> T* {
      return new T[n];
    }
    static auto deallocate(T* p) -> void {
      delete[] p;
    }
  // data members
    T* ptr;
    size_t sz;
};

template<class T> auto
to_string(const dynarray<T>& v) {
  return range_to_string(v);
}
template<class T> auto begin(const dynarray<T>& v) { return v.begin(); }
template<class T> auto begin(      dynarray<T>& v) { return v.begin(); }
template<class T> auto end  (const dynarray<T>& v) { return v.end(); }
template<class T> auto end  (      dynarray<T>& v) { return v.end(); }


template<class Rng> auto make_dynarray(const Rng& x) { 
  using T = typename Rng::value_type;
  return dynarray<T>(x.begin(), x.end());
}

} // std_e
