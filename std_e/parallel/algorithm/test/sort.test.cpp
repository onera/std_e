#include "std_e/unit_test/doctest_mpi.hpp"

#include <cmath>
#include "std_e/log.hpp"
#include "std_e/parallel/algorithm/sort.hpp"


auto sinc(double x) -> double {
  if(x == 0.) return 1.;
  else return sin(x)/x;
}


MPI_TEST_CASE("sort sinc",4) {
  int sz_tot = 200;
  std::vector<double> xg(sz_tot);
  std::vector<double> yg(sz_tot);

  double inf = -25.;
  //double sup =  20.;
  double sup =  30.;
  for (int i=0; i<sz_tot; ++i) {
    xg[i] = inf + (sup-inf)/sz_tot * i;
    //yg[i] = sinc(xg[i]);
    yg[i] = xg[i];
  }

  int sz = sz_tot/test_nb_procs;
  std::vector<double> x(sz);
  std::vector<double> y(sz);
  for (int i=0; i<sz; ++i) {
    x[i] = inf + (sup-inf)/sz_tot * (i+test_rank*sz);
    y[i] = sinc(x[i]);
    //y[i] = x[i];
  }

  //for (int i=0; i<sz_tot; ++i) {
  //  std::cout << x[i] << " " << y[i] << "\n";
  //}
  //if (test_rank==0) ELOG(yg);
  std::vector<double> res = std_e::partition_sort2__indirect_swaps(y,sz_tot,std::less<>{},test_comm);
  //ELOG(res);
  MPI_Barrier(test_comm);
  ELOG(res.size());
}
