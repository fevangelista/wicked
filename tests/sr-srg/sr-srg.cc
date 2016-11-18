#include <iostream>

#include "wicked.h"
#include "test.h"

using namespace std;

bool test_Acc_Bcc() {
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

  // Define the orbital space
  osi = std::make_shared<OrbitalSpaceInfo>();
  osi->add_space("o", RDMType::Occupied, {"i", "j", "k", "l", "m", "n"});
  osi->add_space("v", RDMType::Unoccupied, {"a", "b", "c", "d", "e", "f"});

  // Assemble the tests
  auto test_functions = {
      //            Expectation,  test function,  User friendly description
      std::make_tuple(TestPass, test_Acc_Bcc, "[Aoo,Boo]"),
  };

  // Run the tests
  bool success = wicked_test(test_functions);

  return success ? EXIT_SUCCESS : EXIT_FAILURE;
}
