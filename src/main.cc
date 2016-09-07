#include <iostream>
#include <fstream>
#include <string>
#include <iomanip>

#include "wicked-def.h"
#include "orbital_space.h"
#include "wdiag_theorem.h"
#include "wdiag_operator.h"
#include "wterm.h"
#include "wsum.h"
#include "wtheorem.h"

using namespace std;

int main(int argc, const char *argv[]) {
  std::cout << "pyWicked" << std::endl;

  osi = std::make_shared<OrbitalSpaceInfo>();
  osi->default_spaces();

  // Convenience variables
  std::string c = "c";
  std::string a = "a";
  std::string v = "v";

  bool algebra = false;
  if (algebra) {
    // internal excitations
    WTerm opT1aa = make_operator("T1", {a}, {a});
    auto opT2aaaa = make_operator("T2", {a, a}, {a, a});

    WTerm opT1ca = make_operator("T1", {a}, {c});
    WTerm opT1cv = make_operator("T1", {v}, {c});
    WTerm opT1av = make_operator("T1", {v}, {a});

    auto opT2ccaa = make_operator("T2", {a, a}, {c, c});
    auto opT2ccav = make_operator("T2", {a, v}, {c, c});
    auto opT2ccvv = make_operator("T2", {v, v}, {c, c});
    auto opT2caaa = make_operator("T2", {a, a}, {c, a});
    auto opT2caav = make_operator("T2", {a, v}, {c, a});
    auto opT2cavv = make_operator("T2", {v, v}, {c, a});
    auto opT2aaav = make_operator("T2", {a, v}, {a, a});
    auto opT2aavv = make_operator("T2", {v, v}, {a, a});

    std::vector<WTerm> opT = {opT1ca,   opT1cv,   opT1av,   opT2ccaa,
                              opT2ccav, opT2ccvv, opT2caaa, opT2caav,
                              opT2cavv, opT2aaav, opT2aavv};

    WTerm opH1aa = make_operator("H1", {a}, {a});
    WTerm opH1ac = make_operator("H1", {c}, {a});

    WTerm opH2aaaa = make_operator("H2", {a, a}, {a, a});

//    WSum sum;
//    for (WTerm &term : opT) {
//      cout << term.str() << endl;
//      sum.add(term);
//    }

//    cout << sum.str() << endl;

    WTheorem wt;
    //  wt.theorem_pair(opH1aa, opT1ca);

    //  wt.theorem_pair(opH1ac, opT1ca);

    wt.theorem_pair(opH1aa, opT1aa);
    //  wt.theorem_pair(opH2aaaa, opT1aa);

    //  wt.theorem_pair(opH2aaaa, opT2aaaa);

    //  for (WTerm &term : opT)
    //    cout << term.latex() << endl;

    //  for (auto op : opT) {
    //    std::cout << op.str() << std::endl;
    //  }

    //  for (auto op : opH) {
    //    std::cout << op.str() << std::endl;
    //  }
  }

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
    auto c1 = wdt.contract(1, {opT1aa, opH1aa});
    auto c2 = wdt.contract(-1, {opH1aa, opT1aa});
    for (const auto& c1_term : c1){
        terms.add(c1_term);
    }
    for (const auto& c2_term : c2){
        terms.add(c2_term);
    }
    cout << terms.str() << endl;

    //    auto Hsr = {opH2cccc, opH2cccv, opH2ccvv, opH2cvcc, opH2vvcc};
    //    for (const auto& H : Hsr){
    //        wdt.contract(1.0, {H, opT2ccvv});
    //    }
  }

  return 0;
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
