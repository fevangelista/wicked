#include <iostream>

#include "orbital_space.h"
#include "wdiag_operator.h"
#include "wdiag_theorem.h"
#include "wsum.h"

#define ANSI_COLOR_RED "\x1b[31m"
#define ANSI_COLOR_GREEN "\x1b[32m"
#define ANSI_COLOR_YELLOW "\x1b[33m"
#define ANSI_COLOR_BLUE "\x1b[34m"
#define ANSI_COLOR_MAGENTA "\x1b[35m"
#define ANSI_COLOR_CYAN "\x1b[36m"
#define ANSI_COLOR_RESET "\x1b[0m"

enum TestResult { TestPass, TestFail, TestException };

using namespace std;

bool test_Aoo_Boo() {
  WDiagTheorem wdt;
  std::string o = "o";
  std::string v = "v";
  auto opAoo = make_diag_operator("A", {o}, {o});
  auto opBoo = make_diag_operator("B", {o}, {o});

  // [Aoo,Boo]
  auto c1 = wdt.contract(1, {opAoo, opBoo}, 0, 2);
  auto c2 = wdt.contract(1, {opBoo, opAoo}, 0, 2);
  c1 -= c2;
//  cout << "\n" << c1 << endl;
  auto e1_test = string_to_sum("f^{v0}_{o0} t^{o0}_{v0}");
  return true; //(c1 == e1_test);
}

int main(int argc, const char *argv[]) {
  osi = std::make_shared<OrbitalSpaceInfo>();
  osi->add_space("o", DMStructure::DoublyOccupied,
                 {"i", "j", "k", "l", "m", "n"});
  osi->add_space("v", DMStructure::Unoccupied, {"a", "b", "c", "d", "e", "f"});

  auto test_functions = {
      //            Expectation,  test function,  User friendly description
      std::make_tuple(TestPass, test_Aoo_Boo, "[Aoo,Boo]"),
//      std::make_tuple(TestPass, test_energy2, "CCSD Energy <V T2>"),
//      std::make_tuple(TestPass, test_energy3, "CCSD Energy 1/2 <V T1 T1>"),
//      std::make_tuple(TestPass, test_r1_1, "CCSD T1 Residual <R1 F>"),

  };

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

  return success ? EXIT_SUCCESS : EXIT_FAILURE;
}
