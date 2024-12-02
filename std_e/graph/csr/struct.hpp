#pragma once


#include "std_e/data_structure/multi_range/multi_range.hpp"
#include "std_e/contract/contract.hpp"


namespace std_e {


// forward declarations
template<class I> class Edge_node_view;
template<class I> class Edge_node;



template<std::ranges::range Rng>
class Edge_node_base : public multi_range2<Rng,Rng> {
  private:
  // traits
    using base = multi_range2<Rng,Rng>;
    using I = typename Rng::value_type;
  // data members
    I number_of_nodes;

  public:
  // traits
    using index_type = I;
  // ctors
    using base::base;

    template<std::ranges::range R>
    Edge_node_base(I number_of_nodes, R&& left, R&& right)
      : base(FWD(left), FWD(right))
      , number_of_nodes(number_of_nodes)
    {
      STD_E_PRECOND_LVL0( left.size() == right.size() );
      STD_E_PRECOND_LVL1( std::ranges::min(left ) >= -1 );
      STD_E_PRECOND_LVL1( std::ranges::min(right) >= -1 );
      STD_E_PRECOND_LVL1( std::ranges::max(left ) < number_of_nodes );
      STD_E_PRECOND_LVL1( std::ranges::max(right) < number_of_nodes );
    }

  // interface
    auto left () const -> const auto& { return get<0>(*this); }
    auto right() const -> const auto& { return get<1>(*this); }
    auto left ()       ->       auto& { return get<0>(*this); }
    auto right()       ->       auto& { return get<1>(*this); }
    auto n_node() const -> I { return number_of_nodes; }
    auto n_edge() const -> I { return left().size(); }

    auto copy() const { return Edge_node(n_node(), left(), right()); }
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
Edge_node_view(I n_node, Rng&& left, Rng&& right) -> Edge_node_view<I>;


// Inheritance instead of `using` to have a real type in compiler messages
template<class I>
class Edge_node : public Edge_node_base<std_e::dynarray<I>> {
  private:
    using base = Edge_node_base<std_e::dynarray<I>>;
    using base::base;

  public:
    Edge_node(const Edge_node_view<I>& x)
      : base(x.n_node(), dynarray<I>(x.left()), dynarray<I>(x.right()))
    {}
};
// deduction guideline
template<class I, class Rng>
Edge_node(I n_node, Rng&& left, Rng&& right) -> Edge_node<I>;


} // std_e
