#include <iostream>

#define PRINT_DEBUG 1

#include "wicked.h"
#include "test.h"

using namespace std;

WDiagTheorem wdt;

bool test_energy1() {
  auto T1 = make_operator("t", {"o->v"});
  auto Fov = make_operator("f", {"v->o"});

  // <F T1>
  auto val = wdt.contract_sum(1, Fov * T1, 0, 0);
  auto val_test = string_to_sum("f^{v0}_{o0} t^{o0}_{v0}");
  return (val == val_test);
}

bool test_energy2() {
  auto T2 = make_operator("t", {"oo->vv"});
  auto Voovv = make_operator("v", {"vv->oo"});

  // <V T2>
  auto val = wdt.contract_sum(1, Voovv * T2, 0, 0);
  auto val_test = string_to_sum("1/4 t^{o0,o1}_{v0,v1} v^{v0,v1}_{o0,o1}");
  return (val == val_test);
}

bool test_energy3() {
  auto T1 = make_operator("t", {"o->v"});
  auto Voovv = make_operator("v", {"vv->oo"});

  // 1/2 <V T1 T1>
  auto val = wdt.contract_sum(scalar_t(1, 2), Voovv * T1 * T1, 0, 0);
  auto val_test =
      string_to_sum("1/2 t^{o0}_{v0} t^{o1}_{v1} v^{v0,v1}_{o0,o1}");
  return (val == val_test);
}

bool test_r1_1() {
  auto Fvo = make_operator("f", {"o->v"});

  // F
  auto sum = wdt.contract_sum(1, Fvo, 2, 2);
  WSum val;
  for (const auto &eq : sum.to_manybody_equation("r")) {
    val.add(eq.rhs(), eq.rhs_factor());
  }
  auto val_test = string_to_sum("f^{o0}_{v0}").canonicalize();

  TEST_DEBUG_PRINT(cout << "Result: " << val << endl;)
  TEST_DEBUG_PRINT(cout << "Test:   " << val_test << endl;)
  return (val == val_test);
}

bool test_r1_2() {
  auto T1 = make_operator("t", {"o->v"});
  auto Fvv = make_operator("f", {"v->v"});

  // [Fvv,T1]
  auto sum = wdt.contract_sum(1, Fvv * T1, 2, 2);
  WSum val;
  for (const auto &eq : sum.to_manybody_equation("r")) {
    val.add(eq.rhs(), eq.rhs_factor());
  }
  auto val_test = string_to_sum("f^{v1}_{v0} t^{o0}_{v1}");

  TEST_DEBUG_PRINT(cout << "Result: " << val << endl;)
  TEST_DEBUG_PRINT(cout << "Test:   " << val_test << endl;)

  return (val == val_test);
}

bool test_r1_3() {
  auto T1 = make_operator("t", {"o->v"});
  auto Foo = make_operator("f", {"o->o"});

  // [Foo,T1]
  auto sum = wdt.contract_sum(1, Foo * T1, 2, 2);
  WSum val;
  for (const auto &eq : sum.to_manybody_equation("r")) {
    val.add(eq.rhs(), eq.rhs_factor());
  }
  auto val_test = string_to_sum("-1 f^{o0}_{o1} t^{o1}_{v0}");
  TEST_DEBUG_PRINT(cout << "Result: " << val << endl;)
  TEST_DEBUG_PRINT(cout << "Test:   " << val_test << endl;)
  return (val == val_test);
}

bool test_r1_4() {
  auto T1 = make_operator("t", {"o->v"});
  auto Vovov = make_operator("v", {"ov->ov"});

  // [Vovov,T1]
  auto sum = wdt.contract_sum(1, Vovov * T1, 2, 2);
  WSum val;
  for (const auto &eq : sum.to_manybody_equation("r")) {
    val.add(eq.rhs(), eq.rhs_factor());
  }
  auto val_test = string_to_sum("-1 t^{o1}_{v1} v^{o0,v1}_{o1,v0}");
  TEST_DEBUG_PRINT(cout << "Result: " << val << endl;)
  TEST_DEBUG_PRINT(cout << "Test:   " << val_test << endl;)
  return (val == val_test);
}

bool test_r1_5() {
  auto T2 = make_operator("t", {"oo->vv"});
  auto Fvo = make_operator("f", {"v->o"});

  // [Fvo,T2]
  auto sum = wdt.contract_sum(1, Fvo * T2, 2, 2);
  WSum val;
  for (const auto &eq : sum.to_manybody_equation("r")) {
    val.add(eq.rhs(), eq.rhs_factor());
  }
  auto val_test = string_to_sum("1 f^{v1}_{o1} t^{o0,o1}_{v0,v1}");
  TEST_DEBUG_PRINT(cout << "Result: " << val << endl;)
  TEST_DEBUG_PRINT(cout << "Test:   " << val_test << endl;)
  return (val == val_test);
}

bool test_r1_6() {
  auto T2 = make_operator("t", {"oo->vv"});
  auto Vovvv = make_operator("v", {"vv->ov"});

  // [Vovvv,T2]
  auto sum = wdt.contract_sum(1, Vovvv * T2, 2, 2);
  WSum val;
  for (const auto &eq : sum.to_manybody_equation("r")) {
    val.add(eq.rhs(), eq.rhs_factor());
  }
  auto val_test = string_to_sum("-1/2 t^{o0,o1}_{v1,v2} v^{v1,v2}_{o1,v0}");
  TEST_DEBUG_PRINT(cout << "Result: " << val << endl;)
  TEST_DEBUG_PRINT(cout << "Test:   " << val_test << endl;)
  return (val == val_test);
}

test_return_t test_r1_7() {
  auto T2 = make_operator("t", {"oo->vv"});
  auto Vooov = make_operator("v", {"ov->oo"});

  // [Vooov,T2]
  auto sum = wdt.contract_sum(1, Vooov * T2, 2, 2);
  WSum val;
  for (const auto &eq : sum.to_manybody_equation("r")) {
    val.add(eq.rhs(), eq.rhs_factor());
  }
  auto val_test = string_to_sum("-1/2 t^{o1,o2}_{v0,v1} v^{o0,v1}_{o1,o2}");
  TEST_DEBUG_PRINT(cout << "Result: " << val << endl;)
  TEST_DEBUG_PRINT(cout << "Test:   " << val_test << endl;)
  bool pass = (val == val_test);

  return make_return_t(TestPass, pass, {"CCSD T1 Residual [Vooov,T2]"});
}

test_return_t test_r1_8() {
  auto T1 = make_operator("t", {"o->v"});
  auto Fov = make_operator("f", {"v->o"});

  // [Fov,T2]
  auto sum = wdt.contract_sum(scalar_t(1, 2),
                              commutator(commutator(Fov, T1), T1), 2, 2);
  WSum val;
  for (const auto &eq : sum.to_manybody_equation("r")) {
    val.add(eq.rhs(), eq.rhs_factor());
  }
  auto val_test = string_to_sum("-1 f^{v1}_{o1} t^{o1}_{v0} t^{o0}_{v1}");
  TEST_DEBUG_PRINT(cout << "Result: " << val << endl;)
  TEST_DEBUG_PRINT(cout << "Test:   " << val_test << endl;)
  bool pass = (val == val_test);

  return make_return_t(TestPass, pass, {"CCSD T1 Residual [Fov,T2]"});
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
      std::make_tuple(TestPass, test_r1_1, "CCSD T1 Residual F"),
      std::make_tuple(TestPass, test_r1_2, "CCSD T1 Residual [Fvv,T1]"),
      std::make_tuple(TestPass, test_r1_3, "CCSD T1 Residual [Foo,T1]"),
      std::make_tuple(TestPass, test_r1_4, "CCSD T1 Residual [Vovov,T1]"),
      std::make_tuple(TestPass, test_r1_5, "CCSD T1 Residual [Fvo,T2]"),
      std::make_tuple(TestPass, test_r1_6, "CCSD T1 Residual [Vovvv,T2]"),
  };

  auto test_functions2 = {test_r1_7, test_r1_8};

  // Run the tests
  bool success = wicked_test(test_functions);
  success = wicked_test(test_functions2);

  return success ? EXIT_SUCCESS : EXIT_FAILURE;
}
