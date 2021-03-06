#include <fstream>
#include <iomanip>
#include <iostream>
#include <string>

#include "wicked-def.h"
#include "helpers.h"
#include "orbital_space.h"
#include "walgebraicterm.h"
#include "wequationterm.h"
#include "wdiag_operator.h"
#include "wdiag_operator_sum.h"
#include "wdiag_theorem.h"
#include "wsqoperator.h"
#include "wtensor.h"
#include "wsum.h"

using namespace std;

void srcc();
void mr();

int main(int argc, const char *argv[]) {

  osi = std::make_shared<OrbitalSpaceInfo>();
  osi->add_space("o", RDMType::Occupied, {"i", "j", "k", "l", "m", "n"});
  osi->add_space("v", RDMType::Unoccupied, {"a", "b", "c", "d", "e", "f"});

  auto T1 = make_operator("t", {"o->v"});
  auto Vooov = make_operator("v", {"ov->oo"});

  WDiagTheorem wdt;

  // 1/2 [[Vooov,T1],T1]
  auto comm = commutator(commutator(Vooov, T1), T1);
  comm *= scalar_t(1, 2);
  auto bch = bch_series(Vooov, T1, 2);

  cout << "\n comm:\n" << comm << endl;
  cout << "\n bch:\n" << bch << endl;

//  wdt.set_print(WDiagPrint::Basic);
  auto sum = wdt.contract_sum(1, bch, 2, 2);
  cout << sum << endl;
  //  WSum val;
  //  for (const auto &eq : sum.to_manybody_equation("r")) {
  //      cout << eq << endl;
  ////    val.add(eq.rhs(), eq.rhs_factor());
  //  }

  //  cout << "\n" << val << endl;
  //  //  auto val_test = string_to_sum("-1 t^{o1}_{v0} t^{o2}_{v1}
  //  v^{o0,v1}_{o1,o2}");
  return 0;
}

void srcc() {
  osi = std::make_shared<OrbitalSpaceInfo>();
  osi->add_space("o", RDMType::Occupied, {"i", "j", "k", "l", "m", "n"});
  osi->add_space("v", RDMType::Unoccupied, {"a", "b", "c", "d", "e", "f"});
  // Convenience variables
  std::string o = "o";
  std::string v = "v";

  auto opT1 = make_diag_operator("t1", {v}, {o});
  auto opT2 = make_diag_operator("t2", {v, v}, {o, o});

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

  auto opR1 = make_diag_operator("r1", {o}, {v});
  auto opR2 = make_diag_operator("r2", {o, o}, {v, v});

  bool singles = false;
  bool doubles = false;

  WDiagTheorem wdt;
  WSum terms;

  rational r1_6(1, 6);
  rational r1_2(1, 2);

  //  terms.add(wdt.contract(1, {opH1ov, opT1}, 0, 0));
  //  auto e2 = wdt.contract(1, {opH2oovv, opT2}, 0, 0);
  //  auto e3 = wdt.contract(r1_2, {opH2oovv, opT1, opT1}, 0, 0);

  if (singles) {
    // R1 F
    terms.add_sum(wdt.contract(1, {opR1, opH1vo}, 0, 0));

    // R1 [F,T1]
    terms.add_sum(wdt.contract(1, {opR1, opH1vv, opT1}, 0, 0));
    terms.add_sum(wdt.contract(1, {opR1, opH1oo, opT1}, 0, 0));

    // R1 [V,T1]
    terms.add_sum(wdt.contract(1, {opR1, opH2ovov, opT1}, 0, 0));

    // R1 [F,T2]
    terms.add_sum(wdt.contract(1, {opR1, opH1ov, opT2}, 0, 0));

    // R1 [V,T2]
    terms.add_sum(wdt.contract(1, {opR1, opH2ovvv, opT2}, 0, 0));
    terms.add_sum(wdt.contract(1, {opR1, opH2ooov, opT2}, 0, 0));

    // R1 [[F,T1],T1]
    terms.add_sum(wdt.contract(r1_2, {opR1, opH1ov, opT1, opT1}, 0, 0));
    terms.add_sum(wdt.contract(-1, {opR1, opT1, opH1ov, opT1}, 0, 0));
    terms.add_sum(wdt.contract(r1_2, {opR1, opT1, opT1, opH1ov}, 0, 0));

    // R1 [[V,T1],T1]
    terms.add_sum(wdt.contract(r1_2, {opR1, opH2ooov, opT1, opT1}, 0, 0));
    terms.add_sum(wdt.contract(-1, {opR1, opT1, opH2ooov, opT1}, 0, 0));
    terms.add_sum(wdt.contract(r1_2, {opR1, opT1, opT1, opH2ooov}, 0, 0));

    terms.add_sum(wdt.contract(r1_2, {opR1, opH2ovvv, opT1, opT1}, 0, 0));
    terms.add_sum(wdt.contract(-1, {opR1, opT1, opH2ovvv, opT1}, 0, 0));
    terms.add_sum(wdt.contract(r1_2, {opR1, opT1, opT1, opH2ovvv}, 0, 0));
    // this term is correct but it gives two 1/2 contributions

    // R1 1/3! [[[V,T1],T1],T1] = 1/6 R1 (V T1 T1 T1 - 3 T1 V T1 T1 + 3 T1 T1 V
    // T1
    // - T1 T1 T1 V)
    terms.add_sum(
        wdt.contract(+r1_6, {opR1, opH2oovv, opT1, opT1, opT1}, 0, 0));
    terms.add_sum(
        wdt.contract(-r1_2, {opR1, opT1, opH2oovv, opT1, opT1}, 0, 0));
    terms.add_sum(
        wdt.contract(+r1_2, {opR1, opT1, opT1, opH2oovv, opT1}, 0, 0));
    terms.add_sum(
        wdt.contract(-r1_6, {opR1, opT1, opT1, opT1, opH2oovv}, 0, 0));
    // this term is correct but it gives two 1/2 contributions

    // R1 [[V,T1],T2] = R1 (V T1 T2 - T1 V T2 - T2 V T1 + T2 T1 V)
    terms.add_sum(wdt.contract(1, {opR1, opH2oovv, opT1, opT2}, 0, 0));
    terms.add_sum(wdt.contract(-1, {opR1, opT1, opH2oovv, opT2}, 0, 0));
    terms.add_sum(wdt.contract(-1, {opR1, opT2, opH2oovv, opT1}, 0, 0));
    terms.add_sum(wdt.contract(1, {opR1, opT2, opT1, opH2oovv}, 0, 0));
  }

  WDiagOperatorSum T1 = make_operator("T1", {"o->v"});
  WDiagOperatorSum T2 = make_operator("T2", {"oo->vv"});
  WDiagOperatorSum F = make_operator("F", {"o->o", "ov->vv", "v->o", "v->v"});
  WDiagOperatorSum V =
      make_operator("V", {"oo->oo", "oo->ov", "oo->vv", "ov->oo", "ov->ov",
                          "ov->vv", "vv->oo", "vv->ov", "vv->vv"});

  WDiagOperatorSum VT1T1 = commutator(commutator(V, T1), T1);
  WDiagOperatorSum VT1T1T1 = commutator(commutator(commutator(V, T1), T1), T1);
  auto res = wdt.contract_sum(scalar_t(1, 6), VT1T1T1, 0, 2);
  cout << res << endl;
  for (const auto &eq : res.to_manybody_equation("R")) {
    cout << "  " << eq.ambit() << endl;
  }

  //  cout << T1 << endl;
  //  cout << F << endl;
  //  cout << T1F << endl;

  if (doubles) {
    // R2 V
    terms.add_sum(wdt.contract(1, {opR2, opH2vvoo}, 0, 0));

    //    // R2 [F,T2]
    //    terms.add_sum(wdt.contract(1, {opR2, opH1vv, opT2}, 0, 0));

    //    // R2 [V,T2]
    //    terms.add_sum(wdt.contract(1, {opR2, opH2oooo, opT2}, 0, 0));

    //    // R2 [V,T2]
    //    terms.add_sum(wdt.contract(1, {opR2, opH2vvvv, opT2}, 0, 0));

    //    // R2 [V,T2]
    //    terms.add_sum(wdt.contract(1, {opR2, opH2ovov, opT2}, 0, 0));
  }

  //  cout << "Sum of all terms:" << endl;
  //  cout << terms << endl;
}

void mr() {
  osi = std::make_shared<OrbitalSpaceInfo>();
  osi->default_spaces();

  // Convenience variables
  std::string c = "c";
  std::string a = "a";
  std::string v = "v";

  {
    // internal excitations
    auto opT1aa = make_diag_operator("T1", {a}, {a});
    auto opT2aaaa = make_diag_operator("T2", {a, a}, {a, a});

    auto opT1ca = make_diag_operator("T1", {a}, {c});
    auto opT1cv = make_diag_operator("T1", {v}, {c});
    auto opT1av = make_diag_operator("T1", {v}, {a});

    auto opT2ccaa = make_diag_operator("T2", {a, a}, {c, c});
    auto opT2ccav = make_diag_operator("T2", {a, v}, {c, c});
    auto opT2ccvv = make_diag_operator("T2", {v, v}, {c, c});
    auto opT2caaa = make_diag_operator("T2", {a, a}, {c, a});
    auto opT2caav = make_diag_operator("T2", {a, v}, {c, a});
    auto opT2cavv = make_diag_operator("T2", {v, v}, {c, a});
    auto opT2aaav = make_diag_operator("T2", {a, v}, {a, a});
    auto opT2aavv = make_diag_operator("T2", {v, v}, {a, a});

    std::vector<WDiagOperator> opT = {opT1ca,   opT1cv,   opT1av,   opT2ccaa,
                                      opT2ccav, opT2ccvv, opT2caaa, opT2caav,
                                      opT2cavv, opT2aaav, opT2aavv};

    auto opH1aa = make_diag_operator("H1", {a}, {a});
    auto opH1ac = make_diag_operator("H1", {c}, {a});
    auto opH2aaaa = make_diag_operator("H2", {a, a}, {a, a});
    auto opH2cccc = make_diag_operator("H2", {c, c}, {c, c});
    auto opH2cccv = make_diag_operator("H2", {c, c}, {c, v});
    auto opH2ccvv = make_diag_operator("H2", {c, c}, {v, v});
    auto opH2cvcc = make_diag_operator("H2", {c, v}, {c, c});
    auto opH2vvcc = make_diag_operator("H2", {v, v}, {c, c});

    cout << opH1aa;
    WDiagTheorem wdt;
    //    wdt.contract(1.0, {opH2aaaa, opT2aavv});
    WSum terms;
    auto c1 = wdt.contract(1, {opT1aa, opH1aa}, 0, 0);
    auto c2 = wdt.contract(-1, {opH1aa, opT1aa}, 0, 0);
    //    for (const auto &c1_term : c1) {
    //      terms.add(c1_term);
    //    }
    //    for (const auto &c2_term : c2) {
    //      terms.add(c2_term);
    //    }
    cout << terms.str() << endl;

    //    auto Hsr = {opH2cccc, opH2cccv, opH2ccvv, opH2cvcc, opH2vvcc};
    //    for (const auto& H : Hsr){
    //        wdt.contract(1.0, {H, opT2ccvv});
    //    }
  }
}
