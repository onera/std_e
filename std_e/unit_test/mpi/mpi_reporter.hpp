#pragma once

// #include <doctest/doctest.h>
#include <fstream>
#include <string>
#include "mpi.h"


#include <vector>
#include <mutex>


using namespace doctest;

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

/* \brief Overload the Ireporter of doctest
 *        This one allows to manage the execution of test in a parallel framework
 *        All results are collecteted by rank 0
 */
struct mpi_reporter : public IReporter {
  // caching pointers/references to objects of these types - safe to do
  const ContextOptions&         opt;
  const TestCaseData*           tc;
  std::vector<SubcaseSignature> subcasesStack;
  size_t                        currentSubcaseLevel;
  bool                          hasLoggedCurrentTestStart;
  std::mutex                    mutex;
  std::ostream&         s;

  int                           world_rank;  /*! Store the current world_rank : can be different than test_rank */

  // constructor has to accept the ContextOptions by ref as a single argument
  mpi_reporter(const ContextOptions& in)
          : opt(in)
          , tc(nullptr)
          , s(get_doctest_logs().log_file)
  {
    MPI_Comm_rank(MPI_COMM_WORLD, &world_rank);
  }

  /* Adapted from doctest */
  void separator_to_stream() {
    s << color_string(Color::Yellow) << "==============================================================================" << "\n";
  }

  /* Adapted from doctest */
  const char* getSuccessOrFailString(bool success, assertType::Enum at,
                                     const char* success_str) {
    if(success)
      return success_str;
    return failureString(at);
  }
    void printVersion() {
    if(opt.no_version == false) {
      s << color_string(Color::Cyan) << "[doctest] " << color_string(Color::None) << "doctest version is \""
        << DOCTEST_VERSION_STR << "\"\n";
    }
  }

  void printIntro() {
    printVersion();
    s << color_string(Color::Cyan) << "[doctest] " << color_string(Color::None)
      << "run with \"--" DOCTEST_OPTIONS_PREFIX_DISPLAY "help\" for options\n";
  }

  void printHelp() {
    int sizePrefixDisplay = static_cast<int>(strlen(DOCTEST_OPTIONS_PREFIX_DISPLAY));
    printVersion();
    // clang-format off
    s << color_string(Color::Cyan) << "[doctest]\n" << color_string(Color::None);
    s << color_string(Color::Cyan) << "[doctest] " << color_string(Color::None);
    s << "boolean values: \"1/on/yes/true\" or \"0/off/no/false\"\n";
    s << color_string(Color::Cyan) << "[doctest] " << color_string(Color::None);
    s << "filter  values: \"str1,str2,str3\" (comma separated strings)\n";
    s << color_string(Color::Cyan) << "[doctest]\n" << color_string(Color::None);
    s << color_string(Color::Cyan) << "[doctest] " << color_string(Color::None);
    s << "filters use wildcards for matching strings\n";
    s << color_string(Color::Cyan) << "[doctest] " << color_string(Color::None);
    s << "something passes a filter if any of the strings in a filter matches\n";
#ifndef DOCTEST_CONFIG_NO_UNPREFIXED_OPTIONS
    s << color_string(Color::Cyan) << "[doctest]\n" << color_string(Color::None);
    s << color_string(Color::Cyan) << "[doctest] " << color_string(Color::None);
    s << "ALL FLAGS, OPTIONS AND FILTERS ALSO AVAILABLE WITH A \"" DOCTEST_CONFIG_OPTIONS_PREFIX "\" PREFIX!!!\n";
#endif
    s << color_string(Color::Cyan) << "[doctest]\n" << color_string(Color::None);
    s << color_string(Color::Cyan) << "[doctest] " << color_string(Color::None);
    s << "Query flags - the program quits after them. Available:\n\n";
    s << " -" DOCTEST_OPTIONS_PREFIX_DISPLAY "?,   --" DOCTEST_OPTIONS_PREFIX_DISPLAY "help, -" DOCTEST_OPTIONS_PREFIX_DISPLAY "h                      "
      << Whitespace(sizePrefixDisplay*0) <<  "prints this message\n";
    s << " -" DOCTEST_OPTIONS_PREFIX_DISPLAY "v,   --" DOCTEST_OPTIONS_PREFIX_DISPLAY "version                       "
      << Whitespace(sizePrefixDisplay*1) << "prints the version\n";
    s << " -" DOCTEST_OPTIONS_PREFIX_DISPLAY "c,   --" DOCTEST_OPTIONS_PREFIX_DISPLAY "count                         "
      << Whitespace(sizePrefixDisplay*1) << "prints the number of matching tests\n";
    s << " -" DOCTEST_OPTIONS_PREFIX_DISPLAY "ltc, --" DOCTEST_OPTIONS_PREFIX_DISPLAY "list-test-cases               "
      << Whitespace(sizePrefixDisplay*1) << "lists all matching tests by name\n";
    s << " -" DOCTEST_OPTIONS_PREFIX_DISPLAY "lts, --" DOCTEST_OPTIONS_PREFIX_DISPLAY "list-test-suites              "
      << Whitespace(sizePrefixDisplay*1) << "lists all matching test suites\n";
    s << " -" DOCTEST_OPTIONS_PREFIX_DISPLAY "lr,  --" DOCTEST_OPTIONS_PREFIX_DISPLAY "list-reporters                "
      << Whitespace(sizePrefixDisplay*1) << "lists all registered reporters\n\n";
    // ================================================================================== << 79
    s << color_string(Color::Cyan) << "[doctest] " << color_string(Color::None);
    s << "The available <int>/<string> options/filters are:\n\n";
    s << " -" DOCTEST_OPTIONS_PREFIX_DISPLAY "tc,  --" DOCTEST_OPTIONS_PREFIX_DISPLAY "test-case=<filters>           "
      << Whitespace(sizePrefixDisplay*1) << "filters     tests by their name\n";
    s << " -" DOCTEST_OPTIONS_PREFIX_DISPLAY "tce, --" DOCTEST_OPTIONS_PREFIX_DISPLAY "test-case-exclude=<filters>   "
      << Whitespace(sizePrefixDisplay*1) << "filters OUT tests by their name\n";
    s << " -" DOCTEST_OPTIONS_PREFIX_DISPLAY "sf,  --" DOCTEST_OPTIONS_PREFIX_DISPLAY "source-file=<filters>         "
      << Whitespace(sizePrefixDisplay*1) << "filters     tests by their file\n";
    s << " -" DOCTEST_OPTIONS_PREFIX_DISPLAY "sfe, --" DOCTEST_OPTIONS_PREFIX_DISPLAY "source-file-exclude=<filters> "
      << Whitespace(sizePrefixDisplay*1) << "filters OUT tests by their file\n";
    s << " -" DOCTEST_OPTIONS_PREFIX_DISPLAY "ts,  --" DOCTEST_OPTIONS_PREFIX_DISPLAY "test-suite=<filters>          "
      << Whitespace(sizePrefixDisplay*1) << "filters     tests by their test suite\n";
    s << " -" DOCTEST_OPTIONS_PREFIX_DISPLAY "tse, --" DOCTEST_OPTIONS_PREFIX_DISPLAY "test-suite-exclude=<filters>  "
      << Whitespace(sizePrefixDisplay*1) << "filters OUT tests by their test suite\n";
    s << " -" DOCTEST_OPTIONS_PREFIX_DISPLAY "sc,  --" DOCTEST_OPTIONS_PREFIX_DISPLAY "subcase=<filters>             "
      << Whitespace(sizePrefixDisplay*1) << "filters     subcases by their name\n";
    s << " -" DOCTEST_OPTIONS_PREFIX_DISPLAY "sce, --" DOCTEST_OPTIONS_PREFIX_DISPLAY "subcase-exclude=<filters>     "
      << Whitespace(sizePrefixDisplay*1) << "filters OUT subcases by their name\n";
    s << " -" DOCTEST_OPTIONS_PREFIX_DISPLAY "r,   --" DOCTEST_OPTIONS_PREFIX_DISPLAY "reporters=<filters>           "
      << Whitespace(sizePrefixDisplay*1) << "reporters to use (console is default)\n";
    s << " -" DOCTEST_OPTIONS_PREFIX_DISPLAY "o,   --" DOCTEST_OPTIONS_PREFIX_DISPLAY "out=<string>                  "
      << Whitespace(sizePrefixDisplay*1) << "output filename\n";
    s << " -" DOCTEST_OPTIONS_PREFIX_DISPLAY "ob,  --" DOCTEST_OPTIONS_PREFIX_DISPLAY "order-by=<string>             "
      << Whitespace(sizePrefixDisplay*1) << "how the tests should be ordered\n";
    s << Whitespace(sizePrefixDisplay*3) << "                                       <string> - by [file/suite/name/rand]\n";
    s << " -" DOCTEST_OPTIONS_PREFIX_DISPLAY "rs,  --" DOCTEST_OPTIONS_PREFIX_DISPLAY "rand-seed=<int>               "
      << Whitespace(sizePrefixDisplay*1) << "seed for random ordering\n";
    s << " -" DOCTEST_OPTIONS_PREFIX_DISPLAY "f,   --" DOCTEST_OPTIONS_PREFIX_DISPLAY "first=<int>                   "
      << Whitespace(sizePrefixDisplay*1) << "the first test passing the filters to\n";
    s << Whitespace(sizePrefixDisplay*3) << "                                       execute - for range-based execution\n";
    s << " -" DOCTEST_OPTIONS_PREFIX_DISPLAY "l,   --" DOCTEST_OPTIONS_PREFIX_DISPLAY "last=<int>                    "
      << Whitespace(sizePrefixDisplay*1) << "the last test passing the filters to\n";
    s << Whitespace(sizePrefixDisplay*3) << "                                       execute - for range-based execution\n";
    s << " -" DOCTEST_OPTIONS_PREFIX_DISPLAY "aa,  --" DOCTEST_OPTIONS_PREFIX_DISPLAY "abort-after=<int>             "
      << Whitespace(sizePrefixDisplay*1) << "stop after <int> failed assertions\n";
    s << " -" DOCTEST_OPTIONS_PREFIX_DISPLAY "scfl,--" DOCTEST_OPTIONS_PREFIX_DISPLAY "subcase-filter-levels=<int>   "
      << Whitespace(sizePrefixDisplay*1) << "apply filters for the first <int> levels\n";
    s << color_string(Color::Cyan) << "\n[doctest] " << color_string(Color::None);
    s << "Bool options - can be used like flags and true is assumed. Available:\n\n";
    s << " -" DOCTEST_OPTIONS_PREFIX_DISPLAY "s,   --" DOCTEST_OPTIONS_PREFIX_DISPLAY "success=<bool>                "
      << Whitespace(sizePrefixDisplay*1) << "include successful assertions in output\n";
    s << " -" DOCTEST_OPTIONS_PREFIX_DISPLAY "cs,  --" DOCTEST_OPTIONS_PREFIX_DISPLAY "case-sensitive=<bool>         "
      << Whitespace(sizePrefixDisplay*1) << "filters being treated as case sensitive\n";
    s << " -" DOCTEST_OPTIONS_PREFIX_DISPLAY "e,   --" DOCTEST_OPTIONS_PREFIX_DISPLAY "exit=<bool>                   "
      << Whitespace(sizePrefixDisplay*1) << "exits after the tests finish\n";
    s << " -" DOCTEST_OPTIONS_PREFIX_DISPLAY "d,   --" DOCTEST_OPTIONS_PREFIX_DISPLAY "duration=<bool>               "
      << Whitespace(sizePrefixDisplay*1) << "prints the time duration of each test\n";
    s << " -" DOCTEST_OPTIONS_PREFIX_DISPLAY "nt,  --" DOCTEST_OPTIONS_PREFIX_DISPLAY "no-throw=<bool>               "
      << Whitespace(sizePrefixDisplay*1) << "skips exceptions-related assert checks\n";
    s << " -" DOCTEST_OPTIONS_PREFIX_DISPLAY "ne,  --" DOCTEST_OPTIONS_PREFIX_DISPLAY "no-exitcode=<bool>            "
      << Whitespace(sizePrefixDisplay*1) << "returns (or exits) always with success\n";
    s << " -" DOCTEST_OPTIONS_PREFIX_DISPLAY "nr,  --" DOCTEST_OPTIONS_PREFIX_DISPLAY "no-run=<bool>                 "
      << Whitespace(sizePrefixDisplay*1) << "skips all runtime doctest operations\n";
    s << " -" DOCTEST_OPTIONS_PREFIX_DISPLAY "nv,  --" DOCTEST_OPTIONS_PREFIX_DISPLAY "no-version=<bool>             "
      << Whitespace(sizePrefixDisplay*1) << "omit the framework version in the output\n";
    s << " -" DOCTEST_OPTIONS_PREFIX_DISPLAY "nc,  --" DOCTEST_OPTIONS_PREFIX_DISPLAY "no-colors=<bool>              "
      << Whitespace(sizePrefixDisplay*1) << "disables colors in output\n";
    s << " -" DOCTEST_OPTIONS_PREFIX_DISPLAY "fc,  --" DOCTEST_OPTIONS_PREFIX_DISPLAY "force-colors=<bool>           "
      << Whitespace(sizePrefixDisplay*1) << "use colors even when not in a tty\n";
    s << " -" DOCTEST_OPTIONS_PREFIX_DISPLAY "nb,  --" DOCTEST_OPTIONS_PREFIX_DISPLAY "no-breaks=<bool>              "
      << Whitespace(sizePrefixDisplay*1) << "disables breakpoints in debuggers\n";
    s << " -" DOCTEST_OPTIONS_PREFIX_DISPLAY "ns,  --" DOCTEST_OPTIONS_PREFIX_DISPLAY "no-skip=<bool>                "
      << Whitespace(sizePrefixDisplay*1) << "don't skip test cases marked as skip\n";
    s << " -" DOCTEST_OPTIONS_PREFIX_DISPLAY "gfl, --" DOCTEST_OPTIONS_PREFIX_DISPLAY "gnu-file-line=<bool>          "
      << Whitespace(sizePrefixDisplay*1) << ":n: vs (n): for line numbers in output\n";
    s << " -" DOCTEST_OPTIONS_PREFIX_DISPLAY "npf, --" DOCTEST_OPTIONS_PREFIX_DISPLAY "no-path-filenames=<bool>      "
      << Whitespace(sizePrefixDisplay*1) << "only filenames and no paths in output\n";
    s << " -" DOCTEST_OPTIONS_PREFIX_DISPLAY "nln, --" DOCTEST_OPTIONS_PREFIX_DISPLAY "no-line-numbers=<bool>        "
      << Whitespace(sizePrefixDisplay*1) << "0 instead of real line numbers in output\n";
    // ================================================================================== << 79
    // clang-format on

    s << color_string(Color::Cyan) << "\n[doctest] " << color_string(Color::None);
    s << "for more information visit the project documentation\n\n";
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

  void test_case_exception(const TestCaseException& e /*in*/) override {


    file_line_to_stream(tc->m_file.c_str(), tc->m_line, " ");
    successOrFailColoredStringToStream(false, e.is_crash ? assertType::is_require :
                                                           assertType::is_check);
    s << Color::Red << (e.is_crash ? "test case CRASHED: " : "test case THREW exception: ")
      << Color::Cyan << e.error_string << "\n";

    int num_stringified_contexts = get_num_stringified_contexts();
    if(num_stringified_contexts) {
        auto stringified_contexts = get_stringified_contexts();
        s << Color::None << "  logged: ";
        for(int i = num_stringified_contexts; i > 0; --i) {
            s << (i == num_stringified_contexts ? "" : "          ")
              << stringified_contexts[i - 1] << "\n";
        }
    }
    s << "\n" << Color::None;

  }

  void subcase_start(const SubcaseSignature& subc) override {
    std::lock_guard<std::mutex> lock(mutex);
    subcasesStack.push_back(subc);
    ++currentSubcaseLevel;
    hasLoggedCurrentTestStart = false;
  }

  void subcase_end() override {
    std::lock_guard<std::mutex> lock(mutex);
    --currentSubcaseLevel;
    hasLoggedCurrentTestStart = false;
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

  void log_message(const MessageData& /*mb */) override {
    // messages too can be used in a multi-threaded context - like asserts
    std::lock_guard<std::mutex> lock(mutex);

  }

  void test_case_skipped(const TestCaseData& /*in*/) override {}
}; // mpi_reporter

// "1" is the priority - used for ordering when multiple reporters/listeners are used
REGISTER_REPORTER("mpi_reporter", 1, mpi_reporter);
