#pragma once


#include <cstddef>


namespace std_e {


using allocator_function =   void* (*) (size_t);
using deallocator_function = void  (*) (void* );


//concept Buffer {
//  auto is_owner() const -> bool = 0;
//  auto release() -> deallocator_function = 0;
//
//  auto data()       ->       void* = 0;
//  auto data() const -> const void* = 0;
//};

//concept Buffer_allocator {
//  auto alloc_function() -> allocator_function ;
//  auto dealloc_function() -> deallocator_function ;
//
//  auto allocate(size_t n) -> void*;
//  auto deallocate(void* ptr) -> void;
//};


} // std_e
