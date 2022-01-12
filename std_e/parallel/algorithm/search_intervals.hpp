#pragma once


#include <cmath>
#include <vector>
#include <tuple>
#include "std_e/algorithm/distribution/uniform.hpp"
#include "std_e/log.hpp" // TODO


namespace std_e {


// TODO DEL (replace by search_near_or_containing_interval)
template<class Interval_range0, class Interval_range1, class I> auto
search_intervals6(const Interval_range0& ticks, const Interval_range1& inter, I max_distance) {
  I n_tick = ticks.size();

  std::vector<I> first_ticks;
  std::vector<I> n_indices;
  std::vector<I> inter_indices;
  std::vector<I> index_ticks_found;

  I k = 0;
  for (I i=0; i<n_tick; ++i) {
    //ELOG(inter[0]);
    //ELOG(ticks[i]);
    //ELOG(inter.back());
    STD_E_ASSERT(inter[0] <= ticks[i] && ticks[i] < inter.back());
    if (std::abs(inter[i+1] - ticks[i]) <= max_distance) continue;

    while (not (inter[k] < ticks[i] && ticks[i] < inter[k+1]) ) {
      ++k;
    }
    // now, ticks[i] is in [inter[k],inter[k+1])

    // if inter[k] is close enought, put it in the ticks found
    if (std::abs(inter[k] - ticks[i]) < max_distance) {
      index_ticks_found.push_back(k);
    }

    // else, register the fact that ticks[i] is to be found in [inter[k],inter[k+1])
    if (inter_indices.size()>0 && inter_indices.back()==k) {
      ++n_indices.back();
    } else {
      first_ticks.push_back(i+1);
      n_indices.push_back(1);
      inter_indices.push_back(k);
    }
  }

  return std::make_tuple(first_ticks, n_indices, inter_indices);
}


template<class Interval_range0, class Interval_range1, class I = typename Interval_range0::value_type> auto
search_intervals(const Interval_range0& ticks, const Interval_range1& inter) {
  STD_E_ASSERT(inter.size()>=2);
  if (ticks.size()>0) {
    STD_E_ASSERT(inter[0] <= ticks[0] && ticks.back() < inter.back());
  }

  I n_tick = ticks.size();

  std::vector<I> inter_indices;
  std::vector<I> first_ticks;

  I k = 0;
  for (I i=0; i<n_tick; ++i) {

    while (not (inter[k] <= ticks[i] && ticks[i] < inter[k+1]) ) {
      ++k;
    }
    // so now we know ticks[i] is to be found in [inter[k],inter[k+1])

    // if k was incremented from the last interval recorded in inter_indices
    if (inter_indices.size()==0 || inter_indices.back()!=k) {
      first_ticks.push_back(i); // then record the fact that ticks[i] is in a new interval...
      inter_indices.push_back(k); // ... and that the number of this interval is k
    }
  }

  // proper ending
  first_ticks.push_back(n_tick);
  inter_indices.push_back(k+1);

  return std::make_tuple(std::move(first_ticks), std::move(inter_indices));
}

template<class I>
struct interval_to_partition2 {
  std::vector<I> far_first_ticks;
  std::vector<I> n_far_ticks;
  std::vector<I> far_inter_indices;

  std::vector<I> near_tick_indices;
  std::vector<I> near_inter_indices;
};

template<class Interval_range0, class Interval_range1, class I> auto
search_near_or_containing_interval(const Interval_range0& ticks, const Interval_range1& inter, I max_distance) -> interval_to_partition2<I> {
  auto [first_ticks, inter_indices] = search_intervals(ticks,inter);

  I n_containing_intervals = first_ticks.size()-1;

  std::vector<I> far_first_ticks;
  std::vector<I> n_far_ticks;
  std::vector<I> far_inter_indices;

  std::vector<I> near_tick_indices;
  std::vector<I> near_inter_indices;

  for (I i=0; i<n_containing_intervals; ++i) {
    I far_first_tick;
    I n_far_tick = 0;
    for (I j=first_ticks[i]; j<first_ticks[i+1]; ++j) {
      // if close to inf, record it as near
      if        ( std::abs(ticks[j]-inter[inter_indices[i]  ]) <= max_distance) {
        near_tick_indices.push_back(j);
        near_inter_indices.push_back(inter_indices[i]);
      // if close to sup, record it as near
      } else if ( std::abs(ticks[j]-inter[inter_indices[i]+1]) <= max_distance) {
        near_tick_indices.push_back(j);
        near_inter_indices.push_back(inter_indices[i]+1);
      // else record as far
      } else {
        if (n_far_tick==0) {
          far_first_tick = j;
        }
        ++n_far_tick;
      }
    }
    // if there are far ticks, record them
    if (n_far_tick>0) {
      far_first_ticks.push_back(far_first_tick);
      n_far_ticks.push_back(n_far_tick);
      far_inter_indices.push_back(inter_indices[i]);
    }
  }
  return {far_first_ticks,n_far_ticks,far_inter_indices,  near_tick_indices,near_inter_indices};
}

// TODO clean
// TODO: far_inter_indices indices that are odd (meaning the associated interval is a pivot equal range)
//         here, we want equal elements to end on the same partition, it means we force
//           the partition index (inf or sup) to be registered
//         we should allow the strategy where equal elts can end up in different ranges
//           -> then we should fill to the tick (mpi scan)
template<class Interval_range0, class Interval_range1, class I> auto
search_near_or_containing_interval2(const Interval_range0& ticks, const Interval_range1& inter, I max_distance) -> interval_to_partition2<I> {
  auto [first_ticks, inter_indices] = search_intervals(ticks,inter);

  I n_containing_intervals = first_ticks.size()-1;

  std::vector<I> far_first_ticks;
  std::vector<I> n_far_ticks;
  std::vector<I> far_inter_indices;

  std::vector<I> near_tick_indices;
  std::vector<I> near_inter_indices;


  for (I i=0; i<n_containing_intervals; ++i) {
    I far_first_tick;
    I n_far_tick = 0;
    for (I j=first_ticks[i]; j<first_ticks[i+1]; ++j) {
      // if the interval index is odd, it means it contains only equal elements
      // so we can't partition further
      // TODO refactor these tests out (function object maybe?)
      if (inter_indices[i]%2==1) {
        near_tick_indices.push_back(j);
        //LOG("equal_elts");
        I distance_inf = ticks[j] - inter[inter_indices[i]];
        I distance_sup = inter[inter_indices[i]+1] - ticks[j];
        if (distance_inf <= distance_sup) {
          near_inter_indices.push_back(inter_indices[i]);
        } else {
          near_inter_indices.push_back(inter_indices[i]+1);
        }
      // if close to inf, record it as near
      } else if ( std::abs(ticks[j]-inter[inter_indices[i]  ]) <= max_distance) {
        near_tick_indices.push_back(j);
        near_inter_indices.push_back(inter_indices[i]);
      // if close to sup, record it as near
      } else if ( std::abs(ticks[j]-inter[inter_indices[i]+1]) <= max_distance) {
        near_tick_indices.push_back(j);
        near_inter_indices.push_back(inter_indices[i]+1);
      // else record as far
      } else {
        if (n_far_tick==0) {
          far_first_tick = j;
        }
        ++n_far_tick;
      }
    }
    // if there are far ticks, record them
    if (n_far_tick>0) {
      far_first_ticks.push_back(far_first_tick);
      n_far_ticks.push_back(n_far_tick);
      far_inter_indices.push_back(inter_indices[i]);
    }
  }
  return {far_first_ticks,n_far_ticks,far_inter_indices,  near_tick_indices,near_inter_indices};
}


} // std_e
