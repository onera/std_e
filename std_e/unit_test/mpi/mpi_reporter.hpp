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

  void printRegisteredReporters() {
    printVersion();
    auto printReporters = [&, this] (const reporterMap& reporters, const char* type) {
      if(reporters.size()) {
        s << color_string(Color::Cyan) << "[doctest] " << color_string(Color::None) << "listing all registered " << type << "\n";
        for(auto& curr : reporters){
          s << "priority: " << std::setw(5) << curr.first.first
            << " name: " << curr.first.second << "\n";
        }
      }
    };
    printReporters(getListeners(), "listeners");
    printReporters(getReporters(), "reporters");
  }

  /* Adapted from doctest */
  std::string getSuccessOrFailColor(bool success, assertType::Enum at) {
      return color_string(success ? Color::BrightGreen :
                          (at & assertType::is_warn) ? Color::Yellow : Color::Red);
  }


  /* Adapted from doctest */
  void successOrFailColoredStringToStream(bool success, assertType::Enum at,
                                          const char* success_str = "SUCCESS") {

    s << getSuccessOrFailColor(success, at)
      << getSuccessOrFailString(success, at, success_str) << ": ";
  }

  /* Adapted from doctest */
  void report_query(const QueryData& in) override {

    if(opt.version) {
      printVersion();
    } else if(opt.help) {
      printHelp();
    } else if(opt.list_reporters) {
      printRegisteredReporters();
    } else if(opt.count || opt.list_test_cases) {
      if(opt.list_test_cases) {
        s << "[doctest] " << "listing all test case names\n";
        separator_to_stream();
      }

      for(unsigned i = 0; i < in.num_data; ++i){
        s << in.data[i]->m_name << "\n";
      }

      separator_to_stream();

      s << "[doctest] " << "unskipped test cases passing the current filters: "
        << g_cs->numTestCasesPassingFilters << "\n";

    } else if(opt.list_test_suites) {
      s << "[doctest] " << "listing all test suites\n";
      separator_to_stream();

      for(unsigned i = 0; i < in.num_data; ++i) {
        s << in.data[i]->m_test_suite << "\n";
      }

      separator_to_stream();

      s << "[doctest] "
        << "unskipped test cases passing the current filters: "
        << g_cs->numTestCasesPassingFilters << "\n";
      s << "[doctest] "
        << "test suites with unskipped test cases passing the current filters: "
        << g_cs->numTestSuitesPassingFilters << "\n";
    }
  }

  /* Adapted from doctest */
  virtual void file_line_to_stream(const char* file, int line,
                                  const char* tail = "") {
    s << color_string(Color::LightGrey) << skipPathFromFilename(file) << (opt.gnu_file_line ? ":" : "(")
      << (opt.no_line_numbers ? 0 : line) // 0 or the real num depending on the option
      << (opt.gnu_file_line ? ":" : "):") << tail;
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

  /* Adapted from doctest */
  void test_run_start() override {
    s << "[doctest] " << "doctest version is \"" << DOCTEST_VERSION_STR << "\"\n";
    s << "[doctest] " << "run with \"--" DOCTEST_OPTIONS_PREFIX_DISPLAY "help\" for options\n";
  }

  void test_run_end(const TestRunStats& p) override {

    separator_to_stream();
    s << std::dec;

    const bool anythingFailed = p.numTestCasesFailed > 0 || p.numAssertsFailed > 0;
    s << color_string(Color::Cyan) << "[doctest] " << color_string(Color::None) << "test cases: " << std::setw(6)
      << p.numTestCasesPassingFilters << " | "
      << ((p.numTestCasesPassingFilters == 0 || anythingFailed) ? color_string(Color::None) :
                                                                  color_string(Color::Green))
      << std::setw(6) << p.numTestCasesPassingFilters - p.numTestCasesFailed << " passed"
      << color_string(Color::None) << " | " << (p.numTestCasesFailed > 0 ? color_string(Color::Red) : color_string(Color::None))
      << std::setw(6) << p.numTestCasesFailed << " failed" << color_string(Color::None) << " | ";
    if(opt.no_skipped_summary == false) {
        const int numSkipped = p.numTestCases - p.numTestCasesPassingFilters;
        s << (numSkipped == 0 ? color_string(Color::None) : color_string(Color::Yellow)) << std::setw(6) << numSkipped
          << " skipped" << color_string(Color::None);
    }
    s << "\n";
    s << color_string(Color::Cyan) << "[doctest] " << color_string(Color::None) << "assertions: " << std::setw(6)
      << p.numAsserts << " | "
      << ((p.numAsserts == 0 || anythingFailed) ? color_string(Color::None) : color_string(Color::Green))
      << std::setw(6) << (p.numAsserts - p.numAssertsFailed) << " passed" << color_string(Color::None)
      << " | " << (p.numAssertsFailed > 0 ? color_string(Color::Red) : color_string(Color::None)) << std::setw(6)
      << p.numAssertsFailed << " failed" << color_string(Color::None) << " |\n";
    s << color_string(Color::Cyan) << "[doctest] " << color_string(Color::None)
      << "Status: " << (p.numTestCasesFailed > 0 ? color_string(Color::Red) : color_string(Color::Green))
      << ((p.numTestCasesFailed > 0) ? "FAILURE!" : "SUCCESS!") << color_string(Color::None) << std::endl;

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
  void test_case_start(const TestCaseData& in) override {
    hasLoggedCurrentTestStart = false;
    tc = &in;
    subcasesStack.clear();
    currentSubcaseLevel = 0;
  }

  /* Adapted from doctest */
  void test_case_reenter(const TestCaseData& /*in*/) override {
    subcasesStack.clear();
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
