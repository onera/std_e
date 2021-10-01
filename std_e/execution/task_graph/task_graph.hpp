#pragma once


#include <deque>
#include "std_e/execution/task/any_task.hpp"
#include "std_e/graph/adjacency_graph/graph.hpp"


namespace std_e {


using task_graph_range_type = range_of_ranges<int,std::deque>;
using task_graph = io_graph</*node_type=*/any_task,/*edge_type=*/void,/*container=*/task_graph_range_type>;


} // std_e
