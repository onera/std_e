#pragma once


#include <cstdint>
#include <cstddef>
#include <cstdio>
#include <algorithm>
#include <initializer_list>


namespace std_e {


struct mallocator {
  static auto allocate(size_t n) -> void* {
    return malloc(n);
  }
  static auto deallocate(void* p) -> void {
    free(p);
  }

  template<class T>
  static auto fill(T* first, T* last, const T& x) {
    std::fill(first, last, x);
  }
  //template<class T>
  //static auto copy(const T* first, const T* last, T* d_first) {
  template<class In_it, class S, class Out_it>
  static auto copy(In_it first, S last, Out_it d_first) {
    std::copy(first, last, d_first);
  }
};

//struct aligned_mallocator {
//  static auto allocate(size_t n) -> void* {
//    void* p;
//    posix_memalign(&p, 64, n);
//    return p;
//  }
//  static auto deallocate(void* p) -> void {
//    free(p);
//  }
//};

template<class T, class Allocator = mallocator> // TODO rename Allocator, it also provide fill and copy
class dynarray {
  public:
  // type traits
    using value_type      =       T ;
    using       iterator  =       T*;
    using const_iterator  = const T*;
    using       pointer   =       T*;
    using const_pointer   = const T*;
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
      Allocator::copy(old.ptr, old.ptr+old.sz, ptr);
    }

    dynarray& operator=(const dynarray& old) {
      if (sz != old.sz) {
        sz = old.sz;
        deallocate(this->ptr);
        this->ptr = allocate(sz);
      }
      Allocator::copy(old.ptr, old.ptr+old.sz, ptr);
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
      Allocator::copy(first, last, ptr);
    }

    dynarray(const std::initializer_list<T>& x)
      : dynarray(x.begin(), x.end())
    {}

    dynarray(const std::ranges::range auto& x)
      : dynarray(x.begin(), x.end())
    {}
    dynarray& operator=(const std::ranges::range auto& x)
    {
      if (sz != x.sz) {
        sz = x.sz;
        deallocate(this->ptr);
        this->ptr = allocate(sz);
      }
      Allocator::copy(x.ptr, x.ptr+x.sz, ptr);
      return *this;
    }

    template<class I, std::enable_if_t< std::is_integral_v<I> , int > =0>
    dynarray(I sz, const T& x)
      : dynarray(sz)
    {
      Allocator::fill(ptr, ptr+sz, x);
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
      return (T*)Allocator::allocate(n * sizeof(T));
    }
    static auto deallocate(T* p) -> void {
      Allocator::deallocate(p);
    }
  // data members
    T* ptr;
    size_t sz;
};

template<class T, class A> auto
to_string(const dynarray<T,A>& v) {
  return range_to_string(v);
}
template<class T, class A> auto begin(const dynarray<T,A>& v) { return v.begin(); }
template<class T, class A> auto begin(      dynarray<T,A>& v) { return v.begin(); }
template<class T, class A> auto end  (const dynarray<T,A>& v) { return v.end(); }
template<class T, class A> auto end  (      dynarray<T,A>& v) { return v.end(); }


template<class Rng> auto make_dynarray(const Rng& x) { 
  using T = typename Rng::value_type;
  return dynarray<T>(x.begin(), x.end());
}

} // std_e
