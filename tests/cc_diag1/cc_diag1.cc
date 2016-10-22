#include <iostream>

#include "orbital_space.h"
#include "wdiag_operator.h"
#include "wdiag_theorem.h"
#include "wsum.h"

using namespace std;

int main(int argc, const char *argv[]) {
  osi = std::make_shared<OrbitalSpaceInfo>();
  osi->add_space("o", DMStructure::DoublyOccupied,
                 {"i", "j", "k", "l", "m", "n"});
  osi->add_space("v", DMStructure::Unoccupied, {"a", "b", "c", "d", "e", "f"});
  // Convenience variables
  std::string o = "o";
  std::string v = "v";

  auto opT1 = make_diag_operator("t", {v}, {o});
  auto opT2 = make_diag_operator("t", {v, v}, {o, o});

  auto opH1oo = make_diag_operator("f", {o}, {o});
  auto opH1ov = make_diag_operator("f", {o}, {v});
  auto opH1vo = make_diag_operator("f", {v}, {o});
  auto opH1vv = make_diag_operator("f", {v}, {v});

  auto opH2oooo = make_diag_operator("v", {o, o}, {o, o});
  auto opH2ovoo = make_diag_operator("v", {o, v}, {o, o});
  auto opH2ooov = make_diag_operator("v", {o, o}, {o, v});
  auto opH2ovov = make_diag_operator("v", {o, v}, {o, v});
  auto opH2vvoo = make_diag_operator("v", {v, v}, {o, o});
  auto opH2oovv = make_diag_operator("v", {o, o}, {v, v});
  auto opH2vvov = make_diag_operator("v", {v, v}, {o, v});
  auto opH2ovvv = make_diag_operator("v", {o, v}, {v, v});
  auto opH2vvvv = make_diag_operator("v", {v, v}, {v, v});

  auto opR1 = make_diag_operator("R1", {o}, {v});
  auto opR2 = make_diag_operator("R2", {o, o}, {v, v});

  WDiagTheorem wdt;
  WSum sum;

  sum += wdt.contract(1, {opH1ov, opT1}, 0, 0);
  //  auto e2 = wdt.contract(1, {opH2oovv, opT2}, 0, 0);
  //  auto e3 = wdt.contract(r1_2, {opH2oovv, opT1, opT1}, 0, 0);

  WIndex o0(0, 0);
  WIndex v0(1, 0);

  WAlgebraicTerm e1_term;
  e1_term.add(WTensor("f", {o0}, {v0}));
  e1_term.add(WTensor("t", {v0}, {o0}));
  WSum e1_sum;
  e1_sum.add(e1_term);
  e1_sum.add(e1_term);

  e1_sum -= sum;

  cout << "Sum of all terms:" << endl;
  cout << sum << endl;
  cout << e1_sum << endl;
  auto size = e1_sum.sum().size();
  return size;
}
