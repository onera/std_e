//#include "std_e/unit_test/doctest_mpi.hpp"
//
//#include "std_e/log.hpp"
//#include "std_e/unit_test/math.hpp"
//#include "std_e/parallel/algorithm/sort.hpp"
//
//
//using namespace std_e;
//
//
//MPI_TEST_CASE("sort sinc",4) {
//  int sz_tot = 200;
//
//  double inf = -25.;
//  //double sup =  20.;
//  double sup =  30.;
//
//  std::vector<double> yg(sinc,sz_tot,inf,sup);
//
//  int sz = sz_tot/test_nb_procs;
//  std::vector<double> x(sz);
//  std::vector<double> y(sz);
//  for (int i=0; i<sz; ++i) {
//    x[i] = inf + (sup-inf)/sz_tot * (i+test_rank*sz);
//    y[i] = sinc(x[i]);
//    //y[i] = x[i];
//  }
//
//  //for (int i=0; i<sz_tot; ++i) {
//  //  std::cout << x[i] << " " << y[i] << "\n";
//  //}
//  //if (test_rank==0) ELOG(yg);
//  std::vector<double> res = std_e::partition_sort2__indirect_swaps(y,sz_tot,std::less<>{},test_comm);
//  //ELOG(res);
//  MPI_Barrier(test_comm);
//  ELOG(res.size());
//}
