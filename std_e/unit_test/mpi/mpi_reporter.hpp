#pragma once

// #include <doctest/doctest.h>
#include <fstream>
#include <string>
#include "mpi.h"


#include <vector>
#include <mutex>

namespace doctest {

namespace {

// https://stackoverflow.com/a/11826666/1583122
struct NullBuffer : std::streambuf {
  int overflow(int c) { return c; }
};
class NullStream : public std::ostream {
  public:
    NullStream()
      : std::ostream(&nullBuff)
    {}
  private:
    NullBuffer nullBuff;
};


/* \brief Overload the ConsoleReporter of doctest
 *        This one allows to manage the execution of test in a parallel framework
 *        All results are collected by rank 0
 */
struct MpiFileReporter : public ConsoleReporter {
  std::ofstream logfile_stream;

  MpiFileReporter(const ContextOptions& co)
    : ConsoleReporter(co,logfile_stream)
  {
    int rank = 0;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);

    std::string logfile_name = "doctest_" + std::to_string(rank) + ".log";

    logfile_stream = std::ofstream(logfile_name.c_str(), std::fstream::out);
  }
};


struct MpiConsoleReporter : public ConsoleReporter {
private:
  NullStream null_stream;

  std::ostream& replace_by_null_if_not_rank_0(std::ostream* os) {
    int rank = 0;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    if (rank==0) {
      return *os;
    } else {
      return null_stream;
    }
  }

public:
  MpiConsoleReporter(const ContextOptions& co)
    : ConsoleReporter(co,replace_by_null_if_not_rank_0(co.cout))
  {}

  std::string file_line_to_string(const char* file, int line,
                                  const char* tail = ""){
    std::stringstream s;
    s << skipPathFromFilename(file)
    << (opt.gnu_file_line ? ":" : "(")
    << (opt.no_line_numbers ? 0 : line) // 0 or the real num depending on the option
    << (opt.gnu_file_line ? ":" : "):") << tail;
    return s.str();
  }

  void test_run_end(const TestRunStats& p) override {
    ConsoleReporter::test_run_end(p);

    const bool anythingFailed = p.numTestCasesFailed > 0 || p.numAssertsFailed > 0;

    // -----------------------------------------------------
    // > Gather information in rank 0
    int n_rank, rank;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &n_rank);

    int g_numAsserts         = 0;
    int g_numAssertsFailed   = 0;
    int g_numTestCasesFailed = 0;

    MPI_Reduce(&p.numAsserts        , &g_numAsserts        , 1, MPI_INT, MPI_SUM, 0, MPI_COMM_WORLD);
    MPI_Reduce(&p.numAssertsFailed  , &g_numAssertsFailed  , 1, MPI_INT, MPI_SUM, 0, MPI_COMM_WORLD);
    MPI_Reduce(&p.numTestCasesFailed, &g_numTestCasesFailed, 1, MPI_INT, MPI_SUM, 0, MPI_COMM_WORLD);

    std::vector<int> numAssertsFailedByRank;
    if(rank == 0){
      numAssertsFailedByRank.resize(n_rank);
    }

    MPI_Gather(&p.numAssertsFailed, 1, MPI_INT, numAssertsFailedByRank.data(), 1, MPI_INT, 0, MPI_COMM_WORLD);

    if(rank == 0) {
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

  void test_case_end(const CurrentTestCaseStats& st) override {
    if (is_mpi_test_case()) {
      // function called by every rank at the end of a test
      // if failed assertions happended, they have been sent to rank 0
      // here rank zero gathers them and prints them all

      int rank;
      MPI_Comm_rank(MPI_COMM_WORLD, &rank);

      // Compute the number of assert with fail among all procs
      int nb_fail_asserts_glob = 0;
      MPI_Reduce(&st.numAssertsFailedCurrentTest, &nb_fail_asserts_glob, 1, MPI_INT, MPI_SUM, 0, MPI_COMM_WORLD);

      if(rank == 0) {
        MPI_Status status;
        MPI_Status status_recv;

        std::vector<std::pair<int,std::string>> msgs(nb_fail_asserts_glob);

        for (int i=0; i<nb_fail_asserts_glob; ++i) {
          MPI_Probe(MPI_ANY_SOURCE, MPI_ANY_TAG, MPI_COMM_WORLD, &status);

          int count;
          MPI_Get_count(&status, MPI_BYTE, &count);

          std::string recv_msg(count,'\0');
          MPI_Recv(recv_msg.data(), count, MPI_BYTE, status.MPI_SOURCE,
                   status.MPI_TAG, MPI_COMM_WORLD, &status_recv);

          msgs[i] = {status.MPI_SOURCE,recv_msg};
        }

        std::sort(begin(msgs),end(msgs),[](const auto& x, const auto& y){ return x.first < y.first; });

        // print
        if (nb_fail_asserts_glob>0) {
          separator_to_stream();
          file_line_to_stream(tc->m_file.c_str(), tc->m_line, "\n");
          if(tc->m_test_suite && tc->m_test_suite[0] != '\0')
            s << Color::Yellow << "TEST SUITE: " << Color::None << tc->m_test_suite << "\n";
          if(strncmp(tc->m_name, "  Scenario:", 11) != 0)
            s << Color::Yellow << "TEST CASE:  ";
          s << Color::None << tc->m_name << "\n\n";
          for(const auto& msg : msgs) {
            s << msg.second;
          }
          s << "\n";
        }
      }
    }

    ConsoleReporter::test_case_end(st);
  }

  bool is_mpi_test_case() const {
    return tc->m_description != nullptr 
        && std::string(tc->m_description) == std::string("MPI_TEST_CASE");
  }

  void log_assert(const AssertData& rb) override {
    if (!is_mpi_test_case()) {
      ConsoleReporter::log_assert(rb);
    } else {
      int rank;
      MPI_Comm_rank(MPI_COMM_WORLD, &rank);


      if(!rb.m_failed && !opt.success)
        return;

      std::lock_guard<std::mutex> lock(mutex);

      std::stringstream failure_msg;
      failure_msg << Color::Red << "On rank [" << rank << "] : " << Color::None;
      failure_msg << file_line_to_string(rb.m_file, rb.m_line, " ");

      if((rb.m_at & (assertType::is_throws_as | assertType::is_throws_with)) ==0){
        failure_msg << Color::Cyan
                    << assertString(rb.m_at)
                    << "( " << rb.m_expr << " ) "
                    << Color::None

                    << (!rb.m_failed ? "is correct!\n" : "is NOT correct!\n")
                    << "  values: "
                    << assertString(rb.m_at)
                    << "( " << rb.m_decomp.c_str() << " )\n";
      }

      std::string failure_str = failure_msg.str();
      int failure_msg_size = failure_str.size();

      MPI_Send(failure_str.c_str(), failure_msg_size, MPI_BYTE,
               0, rb.m_line, MPI_COMM_WORLD); // Tag = file line
    }
  }
}; // MpiConsoleReporter

// "1" is the priority - used for ordering when multiple reporters/listeners are used
REGISTER_REPORTER("MpiConsoleReporter", 1, MpiConsoleReporter);
REGISTER_REPORTER("MpiFileReporter", 1, MpiFileReporter);

} // anonymous
} // doctest
