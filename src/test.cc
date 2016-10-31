#include <iostream>
#include "test.h"

#define ANSI_COLOR_RED "\x1b[31m"
#define ANSI_COLOR_GREEN "\x1b[32m"
#define ANSI_COLOR_YELLOW "\x1b[33m"
#define ANSI_COLOR_BLUE "\x1b[34m"
#define ANSI_COLOR_MAGENTA "\x1b[35m"
#define ANSI_COLOR_CYAN "\x1b[36m"
#define ANSI_COLOR_RESET "\x1b[0m"

using namespace std;

bool wicked_test(std::vector<std::tuple<TestResult, bool (*)(), const char *>>
                     test_functions) {

  std::vector<std::tuple<std::string, TestResult, double>> results;

  printf(ANSI_COLOR_RESET);

  printf("\n %-68s %s", "Description", "Result");
  printf("\n %s", std::string(79, '-').c_str());

  bool success = true;
  for (auto test_function : test_functions) {
    printf("\n %-68s", std::get<2>(test_function));
    double result = 0.0;
    TestResult tresult = TestPass, report_result = TestPass;
    std::string exception;
    try {
      result = std::get<1>(test_function)();

      // Did the test pass based on returned value?
      tresult = (result == true) ? TestPass : TestFail;
      // Was the tresult the expected result? If so color green else red.
      report_result =
          tresult == std::get<0>(test_function) ? TestPass : TestFail;
    } catch (std::exception &e) {
      // was an exception expected?
      tresult = TestException;
      report_result =
          tresult == std::get<0>(test_function) ? TestPass : TestException;

      if (report_result == TestException) {
        exception = e.what();
      }
    }
    switch (report_result) {
    case TestPass:
      printf(ANSI_COLOR_GREEN);
      break;
    case TestFail:
      printf(ANSI_COLOR_RED);
      break;
    default:
      printf(ANSI_COLOR_YELLOW);
    }
    switch (tresult) {
    case TestPass:
      printf(" Passed" ANSI_COLOR_RESET);
      break;
    case TestFail:
      printf(" Failed" ANSI_COLOR_RESET);
      break;
    default:
      printf(" Exception" ANSI_COLOR_RESET);
    }

    if (report_result == TestException)
      printf("\n    Unexpected: %s", exception.c_str());
    if (report_result != TestPass)
      success = false;
  }
  printf("\n %s", std::string(79, '-').c_str());
  printf("\n Tests: %s\n", success ? "All passed" : "Some failed");

  return success;
}
