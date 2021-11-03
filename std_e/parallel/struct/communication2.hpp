#pragma once


#include "std_e/parallel/struct/dist_array.hpp"
#include "std_e/parallel/mpi/one_sided/communication.hpp"
#include "std_e/data_structure/jagged_range.hpp"
#include "std_e/algorithm/partition_sort.hpp"
#include "std_e/algorithm/step.hpp"
#include "std_e/execution/future/future.hpp"
#include "std_e/execution/task.hpp"
#include "std_e/parallel/struct/execution_utils.hpp"


namespace std_e {


class protocol_get_indexed2 {
  private:
    protocol_win_get_indexed p;
  public:
    protocol_get_indexed2() = default;

    template<class Int_range>
    protocol_get_indexed2(const Int_range& ins, int type_sz)
      : p(ins,type_sz)
    {}

    protocol_get_indexed2(const protocol_get_indexed2&) = delete;
    protocol_get_indexed2& operator=(const protocol_get_indexed2&) = delete;
    protocol_get_indexed2(protocol_get_indexed2&&) = default;
    protocol_get_indexed2& operator=(protocol_get_indexed2&&) = default;

    auto
    number_of_elements() const -> int {
      return p.number_of_elements();
    }

    template<class T, class Range> auto
    request(const dist_array<T>& a, int rank, Range& out) const -> void {
      p.request(a.win(),rank,out);
    }
};


using protocol_rank_get_indexed2 = std::vector<protocol_get_indexed2>;

template<class TR, class IR> auto
gather_protocol_from_ranks2(const jagged_range<TR,IR,2>& indices_by_rank, int type_sz) {
  int n_rank = indices_by_rank.n_interval();
  protocol_rank_get_indexed2 protocols_by_rank(n_rank);
  for (int i=0; i<n_rank; ++i) {
    const auto& ins = indices_by_rank[i];
    protocols_by_rank[i] = protocol_get_indexed2(ins,type_sz);
  }
  return protocols_by_rank;
}

template<class T, class Range> auto
get_protocol_indexed2(const jagged_vector<int,2>& indices_by_rank, const dist_array<T>& a, Range& out) -> Range& {
  int type_sz = sizeof(T);
  auto protocols_by_rank = gather_protocol_from_ranks2(indices_by_rank,type_sz);

  int n_rank = protocols_by_rank.size();
  auto first = out.data();
  for (int i=0; i<n_rank; ++i) {
    int n_i = protocols_by_rank[i].number_of_elements();
    auto out_i = make_span(first,n_i);
    protocols_by_rank[i].request(a,i,out_i);
    first += n_i;
  }
  return out;
}



struct gather_protocol2 {
  jagged_vector<int,2> indices_by_rank;
  std::vector<int> new_to_old; // permutation
};

template<class Distribution, class Int_range> auto
create_gather_protocol2_2(const Distribution& distri, Int_range ids) {
  auto [partition_is,new_to_old] = apply_indirect_partition_sort(ids,distri);
  apply_step(ids,partition_is,distri);
  jagged_vector<int,2> indices_by_rank(std::move(ids),std::move(partition_is));

  return gather_protocol2{std::move(indices_by_rank),std::move(new_to_old)};
}

constexpr auto create_gather_protocol_fn2 = [](const auto& distri, const auto& ids) {
  return create_gather_protocol2_2(distri,ids);
};




constexpr auto get_protocol_indexed_fn2 = []<class T>(const gather_protocol2& gp, const dist_array<T>& a, std::vector<T>& res) {
  return get_protocol_indexed2(gp.indices_by_rank,a,res);
};

template<class T>
constexpr auto alloc_result_fn2 = [](const gather_protocol2& gp) {
  return std::vector<T>(gp.new_to_old.size());
};

constexpr auto apply_perm_fn2 = [](const gather_protocol2& gp, auto& local_array, auto&&) -> auto& {
  inv_permute(local_array,gp.new_to_old);
  return local_array;
};




template<class Distribution, class Int_range> auto
create_gather_protocol2(future<const Distribution&> distri, future<Int_range> ids) -> future<gather_protocol2> {
  return join(distri,ids) | then(create_gather_protocol_fn2);
}


template<class T> concept Distributed_array = true;

template<class T> auto
get_from_array(future<const gather_protocol2&> gp, future<dist_array<T>&> a) -> future<std::vector<T>> {
  auto a_open = a | then_comm(open_epoch);
  auto result = gp| then(alloc_result_fn2<T>);
  auto result_filled = join(gp,a_open,result) | then_comm(get_protocol_indexed_fn2);
  auto a_close = join(a,result_filled) | then_comm(close_epoch);
  auto final_res = join(gp,result,a_close) | then(apply_perm_fn2);
  return final_res | then(extract_result_fn);
}


} // std_e
