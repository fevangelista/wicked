#include <iostream>

#include "wicked.h"
#include "test.h"

using namespace std;

bool test_energy1() {
  WDiagTheorem wdt;
  std::string o = "o";
  std::string v = "v";
  auto opT1 = make_diag_operator("t", {v}, {o});
  auto opFov = make_diag_operator("f", {o}, {v});

  // <F T1>
  auto e1 = wdt.contract(1, {opFov, opT1}, 0, 0);
  auto e1_test = string_to_sum("f^{v0}_{o0} t^{o0}_{v0}");
  return (e1 == e1_test);
}

bool test_energy2() {
  WDiagTheorem wdt;
  std::string o = "o";
  std::string v = "v";
  auto opT2 = make_diag_operator("t", {v, v}, {o, o});
  auto opVoovv = make_diag_operator("v", {o, o}, {v, v});

  // <V T2>
  auto energy = wdt.contract(1, {opVoovv, opT2}, 0, 0);
  auto energy_test = string_to_sum("1/4 t^{o0,o1}_{v0,v1} v^{v0,v1}_{o0,o1}");
  return (energy == energy_test);
}

bool test_energy3() {
  WDiagTheorem wdt;
  std::string o = "o";
  std::string v = "v";
  auto opT1 = make_diag_operator("t", {v}, {o});
  auto opVoovv = make_diag_operator("v", {o, o}, {v, v});

  // 1/2 <V T1 T1>
  auto energy = wdt.contract(scalar_t(1, 2), {opVoovv, opT1, opT1}, 0, 0);
  auto energy_test =
      string_to_sum("1/2 t^{o0}_{v0} t^{o1}_{v1} v^{v0,v1}_{o0,o1}");
  return (energy == energy_test);
}

bool test_r1_1() {
  WDiagTheorem wdt;
  std::string o = "o";
  std::string v = "v";
  auto opR1 = make_diag_operator("R1", {o}, {v});
  auto opFvo = make_diag_operator("f", {v}, {o});

  // <R1 F>
  auto r1 = wdt.contract(1, {opR1, opFvo}, 0, 0);
  auto r1_test = string_to_sum("R1^{v0}_{o0} f^{o0}_{v0}");
  return (r1 == r1_test);
}

int main(int argc, const char *argv[]) {

  // Define the orbital space
  osi = std::make_shared<OrbitalSpaceInfo>();
  osi->add_space("o", RDMType::Occupied, {"i", "j", "k", "l", "m", "n"});
  osi->add_space("v", RDMType::Unoccupied, {"a", "b", "c", "d", "e", "f"});

  // Assemble the tests
  auto test_functions = {
      //              Expectation,  test function,  User friendly description
      std::make_tuple(TestPass, test_energy1, "CCSD Energy <F T1>"),
      std::make_tuple(TestPass, test_energy2, "CCSD Energy <V T2>"),
      std::make_tuple(TestPass, test_energy3, "CCSD Energy 1/2 <V T1 T1>"),
      std::make_tuple(TestPass, test_r1_1,    "CCSD T1 Residual <R1 F>"),

  };

  // Run the tests
  bool success = wicked_test(test_functions);

  return success ? EXIT_SUCCESS : EXIT_FAILURE;
}
