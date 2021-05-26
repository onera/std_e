#pragma once

// From https://stackoverflow.com/a/36015150/1583122
#define NB_VA_ARGS(...) \
  NB_VA_ARGS__IMPL(0,## __VA_ARGS__,16,15,14,13,12,11,10,9,8,7,6,5,4,3,2,1,0)

#define NB_VA_ARGS__IMPL(_0,_1,_2,_3,_4,_5,_6,_7,_8,_9,_10,_11,_12,_13,_14,_15,_16,count,...) count


#define MACRO_NAME_SUFFIXED_BY_NB_VA_ARGS(macro_name_prefix,...) \
  MACRO_NAME_SUFFIXED_BY_NB_VA_ARGS__IMPL(macro_name_prefix,0,## __VA_ARGS__,16,15,14,13,12,11,10,9,8,7,6,5,4,3,2,1,0)

#define MACRO_NAME_SUFFIXED_BY_NB_VA_ARGS__IMPL(macro_name_prefix,_0,_1,_2,_3,_4,_5,_6,_7,_8,_9,_10,_11,_12,_13,_14,_15,_16,count,...) \
  macro_name_prefix##count
