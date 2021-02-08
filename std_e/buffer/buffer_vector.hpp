#pragma once


#include <iterator>
#include <memory>
#include <vector>
#include <algorithm>
#include "std_e/future/contract.hpp"
#include "std_e/buffer/allocator.hpp"
#include "std_e/buffer/owning_buffer.hpp"


namespace std_e {


// custom std allocator {
/**
  Standard-compliant allocator
  Can be used as a std::vector allocator.
  Allocation/Deallocation is delegated to the Buffer_allocator.
*/
template<class T, class Buffer_allocator>
class std_buffer_allocator {
  public:
  // type traits
    using value_type = T;
    using propagate_on_container_copy_assignment = std::true_type;
    using propagate_on_container_move_assignment = std::true_type;
    using propagate_on_container_swap = std::true_type;

  // ctors
    explicit
    std_buffer_allocator(Buffer_allocator&& allocator = {})
      : allocator(std::move(allocator))
      , owner(std::make_shared<bool>(true))
    {}

  // alloc/dealloc
    auto
    allocate(size_t n) -> T* {
      return (T*)allocator.allocate(n*sizeof(T));
    }

    auto
    deallocate(T* ptr, size_t) noexcept -> void {
      if (*owner) {
        allocator.deallocate(ptr);
      }
    }

    auto
    release_ownership() && -> deallocator_function {
      *owner = false;
      return allocator.dealloc_function();
    }
  // comparisons
    template <class T0, class T1, class A> friend auto
    operator==(const std_buffer_allocator<T0,A>& x, const std_buffer_allocator<T1,A>& y) -> bool;
  private:
    Buffer_allocator allocator;
    std::shared_ptr<bool> owner; // hack because std::vector::get_allocator() returns by value ;;!;;
};

template <class T0, class T1, class A> auto
operator==(const std_buffer_allocator<T0,A>& x, const std_buffer_allocator<T1,A>& y) -> bool {
  return x.allocator==y.allocator;
}
template <class T0, class T1, class A> auto
operator!=(const std_buffer_allocator<T0,A>& x, const std_buffer_allocator<T1,A>& y) -> bool {
  return !(x==y);
}
// custom std allocator }


template<class T, class A = buffer_mallocator> using buffer_vector = std::vector<T, std_buffer_allocator<T,A>>;


// creation {
template<class T, class A = buffer_mallocator> auto
make_buffer_vector(A alloc = {}) {
  return buffer_vector<T,A>(std_buffer_allocator<T,A>(std::move(alloc)));
}

template<class T, class A = buffer_mallocator> auto
make_buffer_vector(size_t n, A alloc = {}) {
  return buffer_vector<T,A>(n,std_buffer_allocator<T,A>(std::move(alloc)));
}

template<class T, class A = buffer_mallocator> auto
make_buffer_vector(std::initializer_list<T> l, A alloc = {}) {
  return buffer_vector<T,A>(l,std_buffer_allocator<T,A>(std::move(alloc)));
}

template<class It, class A = buffer_mallocator> auto
make_buffer_vector(It first, It last, A alloc = {}) {
  using T = typename std::iterator_traits<It>::value_type;
  return buffer_vector<T,A>(first,last,std_buffer_allocator<T,A>(std::move(alloc)));
}
// creation }



template<class T, class A> auto
to_buffer(buffer_vector<T,A>&& x) {
  auto dealloc = x.get_allocator().release_ownership();
  return owning_buffer(x.data(),dealloc);
}


} // std_e
