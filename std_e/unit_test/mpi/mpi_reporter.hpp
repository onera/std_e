#pragma once

// #include <doctest/doctest.h>
#include <fstream>
#include <string>
#include "mpi.h"


#include <vector>
#include <mutex>


namespace doctest {

// ===========================================================================
struct doctest_logger {
  std::ofstream log_file;
};

// ===========================================================================
doctest_logger init_all_doctest_logs() {
  // std::cout << __PRETTY_FUNCTION__ << std::endl;

  int is_mpi_init = -1;
  MPI_Initialized(&is_mpi_init);
  // fmt::print("is_mpi_init::{}\n", is_mpi_init);

  int i_rank = 0;
  if(is_mpi_init){
    MPI_Comm_rank(MPI_COMM_WORLD, &i_rank);
  }

  std::string filename = "doctest_" + std::to_string(i_rank) + ".log";

  return doctest_logger{std::ofstream(filename.c_str(), std::fstream::out)};
}

// ===========================================================================
static auto& get_doctest_logs(){
  // See C++ Magic Statics (N2660)
  static doctest_logger cur_log = init_all_doctest_logs();
  return cur_log;
}


namespace {
/* \brief Overload the Ireporter of doctest
 *        This one allows to manage the execution of test in a parallel framework
 *        All results are collecteted by rank 0
 */
struct mpi_reporter : public ConsoleReporter {
  int world_rank; // Store the current world_rank : can be different than test_rank

  mpi_reporter(const ContextOptions& co)
    : ConsoleReporter(co,get_doctest_logs().log_file)
  {
    MPI_Comm_rank(MPI_COMM_WORLD, &world_rank);
  }


  std::string file_line_to_string(const char* file, int line,
                                  const char* tail = ""){
    std::stringstream s;
    s << Color::Red
    << "On rank [" << world_rank << "] : "
    << Color::White
    << skipPathFromFilename(file)
    << (opt.gnu_file_line ? ":" : "(")
    << (opt.no_line_numbers ? 0 : line) // 0 or the real num depending on the option
    << (opt.gnu_file_line ? ":" : "):") << tail << Color::None;
    return s.str();
  }

  void test_run_end(const TestRunStats& p) override {
    ConsoleReporter::test_run_end(p);

    const bool anythingFailed = p.numTestCasesFailed > 0 || p.numAssertsFailed > 0;

    // -----------------------------------------------------
    // > Gather information in rank 0
    int n_rank, i_rank;
    MPI_Comm_rank(MPI_COMM_WORLD, &i_rank);
    MPI_Comm_size(MPI_COMM_WORLD, &n_rank);

    int g_numAsserts         = 0;
    int g_numAssertsFailed   = 0;
    int g_numTestCasesFailed = 0;

    MPI_Reduce(&p.numAsserts        , &g_numAsserts        , 1, MPI_INT, MPI_SUM, 0, MPI_COMM_WORLD);
    MPI_Reduce(&p.numAssertsFailed  , &g_numAssertsFailed  , 1, MPI_INT, MPI_SUM, 0, MPI_COMM_WORLD);
    MPI_Reduce(&p.numTestCasesFailed, &g_numTestCasesFailed, 1, MPI_INT, MPI_SUM, 0, MPI_COMM_WORLD);

    std::vector<int> numAssertsFailedByRank;
    if(i_rank == 0){
      numAssertsFailedByRank.resize(n_rank);
    }

    MPI_Gather(&p.numAssertsFailed, 1, MPI_INT, numAssertsFailedByRank.data(), 1, MPI_INT, 0, MPI_COMM_WORLD);

    if(i_rank == 0) {
      separator_to_stream();
      s << Color::Cyan << "[doctest] " << Color::None << "glob assertions: " << std::setw(6)
        << g_numAsserts << " | "
        << ((g_numAsserts == 0 || anythingFailed) ? Color::None : Color::Green)
        << std::setw(6) << (g_numAsserts - g_numAssertsFailed) << " passed" << Color::None
        << " | " << (g_numAssertsFailed > 0 ? Color::Red : Color::None) << std::setw(6)
        << g_numAssertsFailed << " failed" << Color::None << " |\n";

      separator_to_stream();
      if(g_numAssertsFailed > 0){

        s << Color::Cyan << "[doctest] " << Color::None << "fail on rank:" << std::setw(6) << "\n";
        for(int i = 0; i < static_cast<int>(numAssertsFailedByRank.size()); ++i){
          if( numAssertsFailedByRank[i] > 0 ){
            s << std::setw(16) << " -> On rank [" << i << "] with " << numAssertsFailedByRank[i] << " test failed" << std::endl;
          }
        }
      }
      s << Color::Cyan << "[doctest] " << Color::None
        << "Status: " << (g_numTestCasesFailed > 0 ? Color::Red : Color::Green)
        << ((g_numTestCasesFailed > 0) ? "FAILURE!" : "SUCCESS!") << Color::None << std::endl;
    }
  }

  /* Adapted from doctest */
  void test_case_end(const CurrentTestCaseStats& st) override {

    MPI_Barrier(MPI_COMM_WORLD);
    /*
     *  Tt le monde appel cette fonction normalement à la fin du test
     *  Au cours du test on a eu potentiellement des fails
     *  Le rang 0 a la responsabilité de recupérer tt les messages envoyé
     *  Tt est synchrone dans le process car les sous commincateurs sont tjr [0, ---> n_rank_test]
     *  Donc on peut raisoner sur le global sans avoir des effects de bords avec d'autres tests
     */
    int i_rank;
    MPI_Comm_rank(MPI_COMM_WORLD, &i_rank);

    if(i_rank == 0) {
      int flag = 1;
      MPI_Status status;
      MPI_Status status_recv;
      while(flag){
        // std::cout << "MPI_Probe::" << std::endl;
        MPI_Iprobe(MPI_ANY_SOURCE, MPI_ANY_TAG, MPI_COMM_WORLD, &flag, &status);
        // std::cout << "flag::" << flag << std::endl;
        // std::cout << "status.MPI_SOURCE::" << status.MPI_SOURCE << std::endl;
        // std::cout << "status.MPI_TAG   ::" << status.MPI_TAG    << std::endl;
        // std::cout << "status.count     ::" << status.count_lo   << std::endl;
        if(flag){
          int count_lo;
          MPI_Get_count(&status, MPI_BYTE, &count_lo);
          std::vector<char> recv_msg_char(count_lo);
          MPI_Recv(recv_msg_char.data(), count_lo, MPI_BYTE,
                   status.MPI_SOURCE,
                   status.MPI_TAG, MPI_COMM_WORLD, &status_recv);
          std::string recv_msg(recv_msg_char.data(), count_lo);
          s << recv_msg;
        }
      }
    }

    MPI_Barrier(MPI_COMM_WORLD);

    ConsoleReporter::test_case_end(st);
  }

  void log_assert(const AssertData& rb) override {
    // don't include successful asserts by default - this is done here
    // instead of in the framework itself because doctest doesn't know
    // if/when a reporter/listener cares about successful results
    if(!rb.m_failed && !opt.success)
      return;

    // make sure there are no races - this is done here instead of in the
    // framework itself because doctest doesn't know if reporters/listeners
    // care about successful asserts and thus doesn't lock a mutex unnecessarily
    std::lock_guard<std::mutex> lock(mutex);

    // successOrFailColoredStringToStream(!rb.m_failed, rb.m_at);
    std::string failure_msg = file_line_to_string(rb.m_file, rb.m_line, " ");

    // Desactivate for rank 0 because we reprint another at the end of the test
    if(world_rank != 0) {
      file_line_to_stream(rb.m_file, rb.m_line, " ");
      successOrFailColoredStringToStream(!rb.m_failed, rb.m_at);
    }
    if((rb.m_at & (assertType::is_throws_as | assertType::is_throws_with)) ==0){

      // Desactivate for rank 0 because we reprint another at the end of the test
      if(world_rank != 0) {
        s << Color::Cyan << assertString(rb.m_at) << "( " << rb.m_expr << " ) "
          << Color::None;
        s << (!rb.m_failed ? "is correct!\n" : "is NOT correct!\n");
        s << "  values: " << assertString(rb.m_at) << "( " << rb.m_decomp << " )\n";
      }

      failure_msg += "\033[0;36m";
      failure_msg += assertString(rb.m_at);
      failure_msg += "( ";
      failure_msg += rb.m_expr;
      failure_msg += " ) ";
      failure_msg += "\033[0m";

      failure_msg += (!rb.m_failed ? "is correct!\n" : "is NOT correct!\n");
      failure_msg += "  values: ";
      failure_msg += assertString(rb.m_at);
      failure_msg += "( ";
      failure_msg += rb.m_decomp.c_str();
      failure_msg += " )\n";

    }

    /* Reporting to rank 0 of the current fail */
    MPI_Send(failure_msg.c_str(), static_cast<int>(failure_msg.size()),
             MPI_BYTE,
             0,
             rb.m_line,  // Tag = file line
             MPI_COMM_WORLD);

  }

}; // mpi_reporter

// "1" is the priority - used for ordering when multiple reporters/listeners are used
REGISTER_REPORTER("mpi_reporter", 1, mpi_reporter);

} // anonymous
} // doctest
