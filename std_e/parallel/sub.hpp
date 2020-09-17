#pragma once


#include "std_e/utils/vector.hpp"
#include "std_e/parallel/mpi.hpp"


namespace std_e {


struct mpi_sub_comm {
  private:
    MPI_Comm sub_comm = MPI_COMM_NULL;

  public:
    mpi_sub_comm(MPI_Comm origin_comm, const std::vector<int>& sub_ranks) noexcept {
      MPI_Group origin_group;
      MPI_Comm_group(origin_comm, &origin_group);

      MPI_Group sub_group;
      int nb_ranks = sub_ranks.size();
      MPI_Group_incl(origin_group, nb_ranks, sub_ranks.data(), &sub_group);

      MPI_Comm_create_group(MPI_COMM_WORLD, sub_group, 0, &sub_comm);

      MPI_Group_free(&origin_group);
      MPI_Group_free(&sub_group);
    }

    mpi_sub_comm(MPI_Comm origin_comm, int nb_ranks) noexcept
      : mpi_sub_comm(origin_comm,iota(nb_ranks))
    {}

    auto comm() const -> const MPI_Comm& {
      return sub_comm;
    }


  ~mpi_sub_comm() {
    if(sub_comm != MPI_COMM_NULL){
      MPI_Comm_free(&sub_comm);
    }
  }
};


inline auto
seq_comm_of_rank(MPI_Comm origin_comm) -> mpi_sub_comm {
  std::vector<int> ranks = {rank(origin_comm)};
  return mpi_sub_comm(origin_comm,ranks);
}


} // std_e
