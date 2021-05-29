#include "std_e/unit_test/doctest.hpp"

#include "std_e/graph/adjacency_graph/__unused/rearranging_view.hpp"

#include <algorithm>
#include <iterator>

using namespace std_e;
template<class I, class Pred> inline auto
// requires I is bidirectional iterator
bidirectional_partition(I first, I last, Pred p) -> I {
  if (first == last) return last;

  auto from_begin = first;
  auto from_end = --last;

  while (from_begin != from_end) {
    while ( (from_begin != from_end) &&  p(*from_begin) ) { ++from_begin; }
    while ( (from_begin != from_end) && !p(*from_end)   ) { --from_end; }
    std::iter_swap(from_begin,from_end);
  }
  if (p(*from_begin)) { ++from_begin; }
  return from_begin;
}


template<class I> auto
// requires I is iterator
swap_pivot_with_first(I first, I) {
  return *first;
  // nothing to swap since the chosen pivot is first
}

template<class I, class P> auto
// requires I is bidirectional iterator
quick_sort(I first, I last, P pred) -> void {
  I next = std::next(first);

  if (first == last) return;
  if (next  == last) return;

  auto pivot = swap_pivot_with_first(first,last);
  auto less_than_pivot = [pred,&pivot](const auto& value){ return pred(value , pivot); };

  auto partition_point = bidirectional_partition(next,last,less_than_pivot);
  I one_before_partition_point = std::prev(partition_point);

  std::iter_swap(first,one_before_partition_point);
  quick_sort(first, one_before_partition_point,pred);
  quick_sort(partition_point,last,pred);
}



TEST_CASE("rearranging_view") {
  std::vector<char> v = {'D','C','A','B'};

  SUBCASE("rearranging_reference") {
    std::vector<int> perm = {2,3,1,0}; // permutation to sort vector v
    rearranging_reference ref0(&v,perm[0]);
    rearranging_reference ref1(&v,perm[1]);
    rearranging_reference ref2(&v,perm[2]);
    rearranging_reference ref3(&v,perm[3]);

    SUBCASE("underlying_ref") {
      CHECK( ref0.underlying_ref() == 'A' );
      CHECK( ref1.underlying_ref() == 'B' );
      CHECK( ref2.underlying_ref() == 'C' );
      CHECK( ref3.underlying_ref() == 'D' );

      ref0.underlying_ref() = 'E';
      CHECK( v == std::vector{'D','C','E','B'} );
    }

    SUBCASE("assign") {
      ref0 = ref1;

      CHECK( ref0.underlying_ref() == 'B' );
      CHECK( ref1.underlying_ref() == 'B' );
      CHECK( ref2.underlying_ref() == 'C' );
      CHECK( ref3.underlying_ref() == 'D' );

      CHECK( v == std::vector{'D','C','A','B'} ); // did not change
      CHECK( perm == std::vector{3,3,1,0} ); // changed
    }

    SUBCASE("swap") {
      swap(ref0,ref1);

      CHECK( ref0.underlying_ref() == 'B' );
      CHECK( ref1.underlying_ref() == 'A' );
      CHECK( ref2.underlying_ref() == 'C' );
      CHECK( ref3.underlying_ref() == 'D' );

      CHECK( v == std::vector{'D','C','A','B'} ); // did not change
      CHECK( perm == std::vector{3,2,1,0} ); // changed
    }
  }

  SUBCASE("rearranging_view operations") {
    rearranging_view rear_v(v);
    SUBCASE("swap for temporary refs") {
      swap(rear_v[0],rear_v[1]);
      CHECK( rear_v[0].underlying_ref() == 'C' );
      CHECK( rear_v[1].underlying_ref() == 'D' );
      CHECK( rear_v[2].underlying_ref() == 'A' );
      CHECK( rear_v[3].underlying_ref() == 'B' );
    }

    // DOES NOT WORK
    SUBCASE("sort") {
      // 0. Does not work (because of proxy references?)
      //std::sort(begin(rear_v),end(rear_v),[](const auto& x, const auto& y){ return x.underlying_ref() < y.underlying_ref(); });

      // 1. Works (because the different implentation seems OK to handle proxy references?)
      quick_sort(begin(rear_v),end(rear_v),[](const auto& x, const auto& y){ return x.underlying_ref() < y.underlying_ref(); });

      // 2. Does not compile (how to make it compile with proxy references?)
      //std::ranges::sort(begin(rear_v),end(rear_v),[](const auto& x, const auto& y){ return x.underlying_ref() < y.underlying_ref(); });
      //std::ranges::partition(begin(rear_v),end(rear_v),[](const auto& x){ ELOG(x.underlying_ref()); return x.underlying_ref() < 'C'; });

      CHECK( rear_v[0].underlying_ref() == 'A' );
      CHECK( rear_v[1].underlying_ref() == 'B' );
      CHECK( rear_v[2].underlying_ref() == 'C' );
      CHECK( rear_v[3].underlying_ref() == 'D' );
    }
  }
}
