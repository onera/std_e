cmake_minimum_required(VERSION 3.12)

# https://stackoverflow.com/a/52136398/1583122
function(add_extern_package dependency)
  if(NOT TARGET ${dependency})
    add_subdirectory(extern/${dependency})
  endif()
endfunction()
