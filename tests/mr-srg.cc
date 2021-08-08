#include <iostream>

#include "test.h"
"wicked.h"

    using namespace std;

test_return_t test_Fac_Tca() {
  WickTheorem wdt;
  auto Fac = make_operator("f", {"a->c"});
  auto Tca = make_operator("t", {"c->a"});

  // [Fac,Tca]
  auto sum = wdt.contract_sum(1, commutator(Fac, Tca), 0, 2);

  auto sum_test =
      string_to_expr("eta1^{a1}_{a0} f^{a0}_{c0} t^{c0}_{a1}") +
      string_to_expr(
          "eta1^{a1}_{a0} f^{a0}_{c0} t^{c1}_{a1} { a+(c0) a-(c1) }") +
      string_to_expr("-f^{a0}_{c0} t^{c0}_{a1} { a+(a1) a-(a0) }") +
      string_to_expr(
          "f^{a0}_{c0} gamma1^{a1}_{a0} t^{c1}_{a1} { a+(c0) a-(c1) }");

  bool pass = (sum == sum_test);
  return make_return_t(TestPass, pass, {"[Fac,Tca]"});
}

test_return_t test_Faa_Tca() {
  WickTheorem wdt;
  auto Faa = make_operator("f", {"a->a"});
  auto Tca = make_operator("t", {"c->a"});

  // [Faa,Tca]
  auto sum = wdt.contract_sum(1, commutator(Faa, Tca), 0, 2);

  auto sum_test =
      string_to_expr(
          "eta1^{a2}_{a1} f^{a1}_{a0} t^{c0}_{a2} { a+(a0) a-(c0) }") +
      string_to_expr(
          "f^{a1}_{a0} gamma1^{a2}_{a1} t^{c0}_{a2} { a+(a0) a-(c0) }");

  bool pass = (sum == sum_test);
  return make_return_t(TestPass, pass, {"[Faa,Tca]"});
}

test_return_t test_Fvv_Tcv() {
  WickTheorem wdt;
  //  wdt.set_print(PrintLevel::Basic);
  auto Fvv = make_operator("f", {"v->v"});
  auto Tcv = make_operator("t", {"c->v"});

  // [Fvv,Tcv]
  auto sum = wdt.contract_sum(1, commutator(Fvv, Tcv), 0, 2);

  auto sum_test = string_to_expr("f^{v1}_{v0} t^{c0}_{v1} { a+(v0) a-(c0) }");

  bool pass = (sum == sum_test);
  return make_return_t(TestPass, pass, {"[Fvv,Tcv]"});
}

test_return_t test_Faa_Tav() {
  WickTheorem wdt;
  auto Faa = make_operator("f", {"a->a"});
  auto Tav = make_operator("t", {"a->v"});

  // [Faa,Tav]
  auto sum = wdt.contract_sum(1, commutator(Faa, Tav), 0, 2);

  auto sum_test =
      string_to_expr(
          "-eta1^{a2}_{a1} f^{a0}_{a2} t^{a1}_{v0} { a+(v0) a-(a0) }") +
      string_to_expr(
          "-f^{a0}_{a1} gamma1^{a1}_{a2} t^{a2}_{v0} { a+(v0) a-(a0) }");

  bool pass = (sum == sum_test);
  return make_return_t(TestPass, pass, {"[Faa,Tav]"});
}

int main(int argc, const char *argv[]) {

  // Define the orbital space
  osi = std::make_shared<OrbitalSpaceInfo>();
  osi->add_space("c", SpaceType::Occupied, {"i", "j", "k", "l", "m", "n"});
  osi->add_space("a", SpaceType::General, {"u", "v", "w", "x", "y", "z"});
  osi->add_space("v", SpaceType::Unoccupied, {"a", "b", "c", "d", "e", "f"});

  // Assemble the tests
  auto test_functions = {test_Fac_Tca, test_Faa_Tca, test_Fvv_Tcv,
                         test_Faa_Tav};

  // Run the tests
  bool success = wicked_test(test_functions);

  return success ? EXIT_SUCCESS : EXIT_FAILURE;
}
