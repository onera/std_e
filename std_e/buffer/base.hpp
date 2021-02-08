#pragma once


#include <cstddef>


namespace std_e {


using allocator_function =   void* (*) (size_t);
using deallocator_function = void  (*) (void* );


//concept Buffer {
//  auto is_owner() const -> bool = 0;
//  auto release() -> bool = 0;
//  auto deallocator() const -> deallocator_function = 0;
//
//  auto data()       ->       void* = 0;
//  auto data() const -> const void* = 0;
//};

//concept Buffer_allocator {
//  static allocator_function alloc_function;
//  static deallocator_function dealloc_function;
//
//  static allocate(size_t n) -> void*;
//  static auto deallocate(void* ptr) -> void;
//};


} // std_e
