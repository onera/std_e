#include "std_e/parallel/string.hpp"
#include "std_e/logging/console_color.hpp"
#include "std_e/parallel/mpi/point_to_point/point_to_point.hpp"


namespace std_e {


auto
cat_on_rank_0(const std::string& rank_str, MPI_Comm comm) -> std::string {
  int rk = rank(comm);
  int n_rk = n_rank(comm);
  std::string cat_str;

  MPI_Barrier(comm);
  if (rk == 0) {
    cat_str += rank_str;
    for (int i=1; i<n_rk; ++i) {
      int sz;
      int tag0 = i;
      int tag1 = n_rk+i;
      _recv(&sz,1,i,tag0,comm);
      std::string recv_msg(sz,'\0');
      _recv(recv_msg.data(),sz,i,tag1,comm);
      cat_str += recv_msg;
    }
  } else {
    int sz = rank_str.size();
    int tag0 = rk;
    int tag1 = n_rk+rk;
    _send(&sz,1,0,tag0,comm);
    _send(rank_str.data(),sz,0,tag1,comm);
  }
  MPI_Barrier(comm);

  return cat_str;
}


} // std_e
