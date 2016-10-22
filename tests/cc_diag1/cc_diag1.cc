#include <fstream>
#include <iomanip>
#include <iostream>
#include <string>

#include "wicked-def.h"
#include "helpers.h"
#include "orbital_space.h"
#include "wdiag_operator.h"
#include "wdiag_theorem.h"
#include "wsum.h"

using namespace std;

void srcc();
void mr();

int main(int argc, const char *argv[]) {
  //  for (int i : range(19)) {
  //    cout << i << endl;
  //  }
  //  std::vector<MyInt> vec; //{1, 4, 9, 19, 12, 21};

  //  cout << "\nAdding 1" << endl;
  //  vec.push_back(MyInt(1));

  //  cout << "\nAdding 4" << endl;
  //  vec.push_back(MyInt(4));

  //  cout << "\nAdding 9" << endl;
  //  vec.push_back(MyInt(9));

  //  cout << "\nBegin loop" << endl;
  //  for (auto kv : enumerate(vec)) {
  //    cout << kv.first << " " << kv.second.get() << endl;
  //  }
  srcc();
  //    mr();

  return 0;
}

void srcc() {
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

  bool singles = false;
  bool doubles = true;

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

  cout << "Sum of all terms:" << endl;
  cout << terms << endl;
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

/*
 *   auto opT1ca = WDiagOperator("T1", {1, 0, 0}, {0, 1, 0});
  auto opT1cv = WDiagOperator("T1", {1, 0, 0}, {0, 0, 1});
  auto opT1av = WDiagOperator("T1", {0, 1, 0}, {0, 0, 1});

  auto opT2ccaa = WDiagOperator("T2", {2, 0, 0}, {0, 2, 0});
  auto opT2ccav = WDiagOperator("T2", {2, 0, 0}, {0, 1, 1});
  auto opT2ccvv = WDiagOperator("T2", {2, 0, 0}, {0, 0, 2});
  auto opT2caaa = WDiagOperator("T2", {1, 1, 0}, {0, 2, 0});
  auto opT2caav = WDiagOperator("T2", {1, 1, 0}, {0, 1, 1});
  auto opT2cavv = WDiagOperator("T2", {1, 1, 0}, {0, 0, 2});
  auto opT2aaav = WDiagOperator("T2", {0, 2, 0}, {0, 1, 1});
  auto opT2aavv = WDiagOperator("T2", {0, 2, 0}, {0, 0, 2});

  auto opH1cc = WDiagOperator("H1", {1, 0, 0}, {1, 0, 0});
  auto opH1ca = WDiagOperator("H1", {1, 0, 0}, {0, 1, 0});
  auto opH1cv = WDiagOperator("H1", {1, 0, 0}, {0, 0, 1});
  auto opH1ac = WDiagOperator("H1", {0, 1, 0}, {1, 0, 0});
  auto opH1aa = WDiagOperator("H1", {0, 1, 0}, {0, 1, 0});
  auto opH1av = WDiagOperator("H1", {0, 1, 0}, {0, 0, 1});
  auto opH1vc = WDiagOperator("H1", {0, 0, 1}, {1, 0, 0});
  auto opH1va = WDiagOperator("H1", {0, 0, 1}, {0, 1, 0});
  auto opH1vv = WDiagOperator("H1", {0, 0, 1}, {0, 0, 1});

  auto opH2cccc = WDiagOperator("H2", {2, 0, 0}, {2, 0, 0});
  auto opH2ccca = WDiagOperator("H2", {2, 0, 0}, {1, 1, 0});
  auto opH2cccv = WDiagOperator("H2", {2, 0, 0}, {1, 0, 1});
  auto opH2ccaa = WDiagOperator("H2", {2, 0, 0}, {0, 2, 0});
  auto opH2ccav = WDiagOperator("H2", {2, 0, 0}, {0, 1, 1});
  auto opH2ccvv = WDiagOperator("H2", {2, 0, 0}, {0, 0, 2});

  auto opH2cacc = WDiagOperator("H2", {1, 1, 0}, {2, 0, 0});
  auto opH2caca = WDiagOperator("H2", {1, 1, 0}, {1, 1, 0});
  auto opH2cacv = WDiagOperator("H2", {1, 1, 0}, {1, 0, 1});
  auto opH2caaa = WDiagOperator("H2", {1, 1, 0}, {0, 2, 0});
  auto opH2caav = WDiagOperator("H2", {1, 1, 0}, {0, 1, 1});
  auto opH2cavv = WDiagOperator("H2", {1, 1, 0}, {0, 0, 2});

  auto opH2cvcc = WDiagOperator("H2", {1, 0, 1}, {2, 0, 0});
  auto opH2cvca = WDiagOperator("H2", {1, 0, 1}, {1, 1, 0});
  auto opH2cvcv = WDiagOperator("H2", {1, 0, 1}, {1, 0, 1});
  auto opH2cvaa = WDiagOperator("H2", {1, 0, 1}, {0, 2, 0});
  auto opH2cvav = WDiagOperator("H2", {1, 0, 1}, {0, 1, 1});
  auto opH2cvvv = WDiagOperator("H2", {1, 0, 1}, {0, 0, 2});

  auto opH2aacc = WDiagOperator("H2", {0, 2, 0}, {2, 0, 0});
  auto opH2aaca = WDiagOperator("H2", {0, 2, 0}, {1, 1, 0});
  auto opH2aacv = WDiagOperator("H2", {0, 2, 0}, {1, 0, 1});
  auto opH2aaaa = WDiagOperator("H2", {0, 2, 0}, {0, 2, 0});
  auto opH2aaav = WDiagOperator("H2", {0, 2, 0}, {0, 1, 1});
  auto opH2aavv = WDiagOperator("H2", {0, 2, 0}, {0, 0, 2});

  auto opH2avcc = WDiagOperator("H2", {0, 1, 1}, {2, 0, 0});
  auto opH2avca = WDiagOperator("H2", {0, 1, 1}, {1, 1, 0});
  auto opH2avcv = WDiagOperator("H2", {0, 1, 1}, {1, 0, 1});
  auto opH2avaa = WDiagOperator("H2", {0, 1, 1}, {0, 2, 0});
  auto opH2avav = WDiagOperator("H2", {0, 1, 1}, {0, 1, 1});
  auto opH2avvv = WDiagOperator("H2", {0, 1, 1}, {0, 0, 2});

  auto opH2vvcc = WDiagOperator("H2", {0, 0, 2}, {2, 0, 0});
  auto opH2vvca = WDiagOperator("H2", {0, 0, 2}, {1, 1, 0});
  auto opH2vvcv = WDiagOperator("H2", {0, 0, 2}, {1, 0, 1});
  auto opH2vvaa = WDiagOperator("H2", {0, 0, 2}, {0, 2, 0});
  auto opH2vvav = WDiagOperator("H2", {0, 0, 2}, {0, 1, 1});
  auto opH2vvvv = WDiagOperator("H2", {0, 0, 2}, {0, 0, 2});

  std::vector<WDiagOperator> opT = {opT1ca,   opT1cv,   opT1av,   opT2ccaa,
                                opT2ccav, opT2ccvv, opT2caaa, opT2caav,
                                opT2cavv, opT2aaav, opT2aavv};

  std::vector<WDiagOperator> opH = {
      opH1cc,   opH1ca,   opH1cv,   opH1ac,   opH1aa,   opH1av,   opH1vc,
      opH1va,   opH1vv,   opH2cccc, opH2ccca, opH2cccv, opH2ccaa, opH2ccav,
      opH2ccvv, opH2cacc, opH2caca, opH2cacv, opH2caaa, opH2caav, opH2cavv,
      opH2cvcc, opH2cvca, opH2cvcv, opH2cvaa, opH2cvav, opH2cvvv, opH2aacc,
      opH2aaca, opH2aacv, opH2aaaa, opH2aaav, opH2aavv, opH2avcc, opH2avca,
      opH2avcv, opH2avaa, opH2avav, opH2avvv, opH2vvcc, opH2vvca, opH2vvcv,
      opH2vvaa, opH2vvav, opH2vvvv};
      */
