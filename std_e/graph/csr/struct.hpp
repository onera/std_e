#pragma once


#include "std_e/data_structure/multi_range/cmulti_range.hpp"
#include "std_e/contract/contract.hpp"


namespace std_e {


// forward declarations
template<class I> class Edge_node_view;
template<class I> class Edge_node;



template<std::ranges::range Rng>
class Edge_node_base : public cmulti_range<Rng,2> {
  private:
  // traits
    using base = cmulti_range<Rng,2>;
    using I = typename Rng::value_type;
  // data members
    I number_of_nodes;

  public:
  // traits
    using index_type = I;
  // ctors
    using base::base;

    template<std::ranges::range R>
    Edge_node_base(I number_of_nodes, R&& values)
      : base(FWD(values))
      , number_of_nodes(number_of_nodes)
    {
      STD_E_PRECOND_LVL1( std::ranges::min(values) >= -1 );
      STD_E_PRECOND_LVL1( std::ranges::max(values) < number_of_nodes );
    }

  // interface
    auto left () const { return range_view<0>(*this); }
    auto right() const { return range_view<1>(*this); }
    auto left ()       { return range_view<0>(*this); }
    auto right()       { return range_view<1>(*this); }
    auto n_node() const -> I { return number_of_nodes; }
    auto n_edge() const -> I { return this->size(); }

    auto copy() const { return Edge_node(n_node(), this->flat_view()); }
};

template<class I> auto
to_string(const Edge_node_base<I>& x) -> std::string {
  using std::to_string;
  using std_e::to_string;
  return
      "|V|=" + to_string(x.n_node()) + ", |E|=" + to_string(x.n_edge()) + "\n"
    + to_string(x.left()) + "\n"
    + to_string(x.right()) + "\n";
}

// Inheritance instead of `using` to have a real type in compiler messages
template<class I>
class Edge_node_view : public Edge_node_base<std_e::span<I>> {
  using base = Edge_node_base<std_e::span<I>>;
  using base::base;
};
// deduction guideline
template<class I, class Rng>
Edge_node_view(I n_node, Rng&& values) -> Edge_node_view<I>;


// Inheritance instead of `using` to have a real type in compiler messages
template<class I>
class Edge_node : public Edge_node_base<std_e::dynarray<I>> {
  private:
    using base = Edge_node_base<std_e::dynarray<I>>;
    using base::base;

  public:
    Edge_node(const Edge_node_view<I>& x)
      : base(x.n_node(), dynarray<I>(x.flat_view()))
    {}
};
// deduction guidelines
template<class I, class Rng>
Edge_node(I n_node, Rng&& values) -> Edge_node<I>;
template<class I>
Edge_node(const Edge_node_view<I>& x) -> Edge_node<I>;


} // std_e
