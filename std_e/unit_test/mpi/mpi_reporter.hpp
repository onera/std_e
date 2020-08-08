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


inline std::string
color_string(Color::Enum code)
{
  std::string col = "\033";
  switch(code) { //!OCLINT missing break in switch statement / unnecessary default statement in covered switch statement
    case Color::Red:         col += "[0;31m"; break;
    case Color::Green:       col += "[0;32m"; break;
    case Color::Blue:        col += "[0;34m"; break;
    case Color::Cyan:        col += "[0;36m"; break;
    case Color::Yellow:      col += "[0;33m"; break;
    case Color::Grey:        col += "[1;30m"; break;
    case Color::LightGrey:   col += "[0;37m"; break;
    case Color::BrightRed:   col += "[1;31m"; break;
    case Color::BrightGreen: col += "[1;32m"; break;
    case Color::BrightWhite: col += "[1;37m"; break;
    case Color::Bright: // inval_id
    case Color::None:
    case Color::White:
    default:                 col += "[0m";
  }
  return col;
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
    std::string s;
    s += "\033[0;31m";
    s += "On rank ["+std::to_string(world_rank) + "] : ";
    s += "\033[0m";
    s += skipPathFromFilename(file);
    s += (opt.gnu_file_line ? ":" : "(");
    if(opt.no_line_numbers){
      s += std::to_string(0);
    } else {
       s += std::to_string(line);
    }
    s += (opt.gnu_file_line ? ":" : "):");
    s += tail;
    return s;
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
      s << color_string(Color::Cyan) << "[doctest] " << color_string(Color::None) << "glob assertions: " << std::setw(6)
        << g_numAsserts << " | "
        << ((g_numAsserts == 0 || anythingFailed) ? color_string(Color::None) : color_string(Color::Green))
        << std::setw(6) << (g_numAsserts - g_numAssertsFailed) << " passed" << color_string(Color::None)
        << " | " << (g_numAssertsFailed > 0 ? color_string(Color::Red) : color_string(Color::None)) << std::setw(6)
        << g_numAssertsFailed << " failed" << color_string(Color::None) << " |\n";

      separator_to_stream();
      if(g_numAssertsFailed > 0){

        s << color_string(Color::Cyan) << "[doctest] " << color_string(Color::None) << "fail on rank:" << std::setw(6) << "\n";
        for(int i = 0; i < static_cast<int>(numAssertsFailedByRank.size()); ++i){
          if( numAssertsFailedByRank[i] > 0 ){
            s << std::setw(16) << " -> On rank [" << i << "] with " << numAssertsFailedByRank[i] << " test failed" << std::endl;
          }
        }
      }
      s << color_string(Color::Cyan) << "[doctest] " << color_string(Color::None)
        << "Status: " << (g_numTestCasesFailed > 0 ? color_string(Color::Red) : color_string(Color::Green))
        << ((g_numTestCasesFailed > 0) ? "FAILURE!" : "SUCCESS!") << color_string(Color::None) << std::endl;
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

    // if(opt.duration ||
    //    (st.failure_flags && st.failure_flags != TestCaseFailureReason::AssertFailure))
    //     logTestStart();

    if(opt.duration) {
      s << std::setprecision(6) << std::fixed << st.seconds
        << " s: " << tc->m_name << "\n";
    }

    if(st.failure_flags & TestCaseFailureReason::Timeout) {
      s << "Test case exceeded time limit of " << std::setprecision(6)
        << std::fixed << tc->m_timeout << "!\n";
    }

    if(st.failure_flags & TestCaseFailureReason::ShouldHaveFailedButDidnt) {
      s << "Should have failed but didn't! Marking it as failed!\n";
    } else if(st.failure_flags & TestCaseFailureReason::ShouldHaveFailedAndDid) {
      s << "Failed as expected so marking it as not failed\n";
    } else if(st.failure_flags & TestCaseFailureReason::CouldHaveFailedAndDid) {
      s << "Allowed to fail so marking it as not failed\n";
    } else if(st.failure_flags & TestCaseFailureReason::DidntFailExactlyNumTimes) {
      s << "Didn't fail exactly " << tc->m_expected_failures
        << " times so marking it as failed!\n";
    } else if(st.failure_flags & TestCaseFailureReason::FailedExactlyNumTimes) {
      s << "Failed exactly " << tc->m_expected_failures
        << " times as expected so marking it as not failed!\n";
    }
    if(st.failure_flags & TestCaseFailureReason::TooManyFailedAsserts) {
      s << "Aborting - too many failed asserts!\n";
    }
    // s << color_string(Color::None); // lgtm [cpp/useless-expression]
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
        s << color_string(Color::Cyan) << assertString(rb.m_at) << "( " << rb.m_expr << " ) "
          << color_string(Color::None);
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
