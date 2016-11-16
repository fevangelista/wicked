#include <iostream>

#include "wicked.h"
#include "test.h"

using namespace std;

test_return_t test_Aoo_Boo() {
  WDiagTheorem wdt;
  auto Acc = make_operator("a", {"c->c"});
  auto Bcc = make_operator("b", {"c->c"});

  // [Aoo,Boo]
  auto sum = wdt.contract_sum(1, commutator(Acc, Bcc), 0, 4);

  auto sum_test =
      string_to_sum("-a^{c0}_{c2} b^{c2}_{c1} { a+(c1) a-(c0) }") +
      string_to_sum("a^{c2}_{c0} b^{c1}_{c2} { a+(c0) a-(c1) }");

  cout << '\n' << sum << endl;
  cout << '\n' << sum_test << endl;

  bool pass = (sum == sum_test);
  return make_return_t(TestPass, pass, {"[Acc,Bcc]"});
}

int main(int argc, const char *argv[]) {

  // Define the orbital space
  osi = std::make_shared<OrbitalSpaceInfo>();
  osi->add_space("c", RDMType::Occupied, {"i", "j", "k", "l", "m", "n"});
  osi->add_space("a", RDMType::General, {"u", "v", "w", "x", "y", "z"});
  osi->add_space("v", RDMType::Unoccupied, {"a", "b", "c", "d", "e", "f"});

  // Assemble the tests
  auto test_functions = {test_Aoo_Boo};

  // Run the tests
  bool success = wicked_test(test_functions);

  return success ? EXIT_SUCCESS : EXIT_FAILURE;
}
