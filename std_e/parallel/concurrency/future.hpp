#pragma once

//#include 


template<class T>
class future {
  private:
    MPI_Request req;
    T x;

  public:
    auto
    obj() -> T& {
      return x;
    }
    auto
    wait() -> T {
      MPI_Status status;
      int err = MPI_Wait(&req,&status);
      if (err!=0) throw mpi_exception(err,std::string("in function \"")+__func__+"\"");
      return std::move(x);
    }
};
