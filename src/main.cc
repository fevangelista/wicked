#include <iostream>
#include <fstream>
#include <string>
#include <iomanip>

#include "orbital_space.h"
#include "woperator.h"

using namespace std;

int main (int argc, const char * argv[])
{
    std::cout << "pyWicked" << std::endl;

    osi = std::make_shared<OrbitalSpaceInfo>();
    osi->default_spaces();

    auto opT1ca = WOperator("T1",{1,0,0},{0,1,0});
    auto opT1cv = WOperator("T1",{1,0,0},{0,0,1});
    auto opT1av = WOperator("T1",{0,1,0},{0,0,1});

    auto opT2ccaa = WOperator("T2",{2,0,0},{0,2,0});
    auto opT2ccav = WOperator("T2",{2,0,0},{0,1,1});
    auto opT2ccvv = WOperator("T2",{2,0,0},{0,0,2});
    auto opT2caaa = WOperator("T2",{1,1,0},{0,2,0});
    auto opT2caav = WOperator("T2",{1,1,0},{0,1,1});
    auto opT2cavv = WOperator("T2",{1,1,0},{0,0,2});
    auto opT2aaav = WOperator("T2",{0,2,0},{0,1,1});
    auto opT2aavv = WOperator("T2",{0,2,0},{0,0,2});

    auto opH1cc = WOperator("H1",{1,0,0},{1,0,0});
    auto opH1ca = WOperator("H1",{1,0,0},{0,1,0});
    auto opH1cv = WOperator("H1",{1,0,0},{0,0,1});
    auto opH1ac = WOperator("H1",{0,1,0},{1,0,0});
    auto opH1aa = WOperator("H1",{0,1,0},{0,1,0});
    auto opH1av = WOperator("H1",{0,1,0},{0,0,1});
    auto opH1vc = WOperator("H1",{0,0,1},{1,0,0});
    auto opH1va = WOperator("H1",{0,0,1},{0,1,0});
    auto opH1vv = WOperator("H1",{0,0,1},{0,0,1});

    auto opH2cccc = WOperator("H2",{2,0,0},{2,0,0});
    auto opH2ccca = WOperator("H2",{2,0,0},{1,1,0});
    auto opH2cccv = WOperator("H2",{2,0,0},{1,0,1});
    auto opH2ccaa = WOperator("H2",{2,0,0},{0,2,0});
    auto opH2ccav = WOperator("H2",{2,0,0},{0,1,1});
    auto opH2ccvv = WOperator("H2",{2,0,0},{0,0,2});

    auto opH2cacc = WOperator("H2",{1,1,0},{2,0,0});
    auto opH2caca = WOperator("H2",{1,1,0},{1,1,0});
    auto opH2cacv = WOperator("H2",{1,1,0},{1,0,1});
    auto opH2caaa = WOperator("H2",{1,1,0},{0,2,0});
    auto opH2caav = WOperator("H2",{1,1,0},{0,1,1});
    auto opH2cavv = WOperator("H2",{1,1,0},{0,0,2});

    auto opH2cvcc = WOperator("H2",{1,0,1},{2,0,0});
    auto opH2cvca = WOperator("H2",{1,0,1},{1,1,0});
    auto opH2cvcv = WOperator("H2",{1,0,1},{1,0,1});
    auto opH2cvaa = WOperator("H2",{1,0,1},{0,2,0});
    auto opH2cvav = WOperator("H2",{1,0,1},{0,1,1});
    auto opH2cvvv = WOperator("H2",{1,0,1},{0,0,2});

    auto opH2aacc = WOperator("H2",{0,2,0},{2,0,0});
    auto opH2aaca = WOperator("H2",{0,2,0},{1,1,0});
    auto opH2aacv = WOperator("H2",{0,2,0},{1,0,1});
    auto opH2aaaa = WOperator("H2",{0,2,0},{0,2,0});
    auto opH2aaav = WOperator("H2",{0,2,0},{0,1,1});
    auto opH2aavv = WOperator("H2",{0,2,0},{0,0,2});

    auto opH2avcc = WOperator("H2",{0,1,1},{2,0,0});
    auto opH2avca = WOperator("H2",{0,1,1},{1,1,0});
    auto opH2avcv = WOperator("H2",{0,1,1},{1,0,1});
    auto opH2avaa = WOperator("H2",{0,1,1},{0,2,0});
    auto opH2avav = WOperator("H2",{0,1,1},{0,1,1});
    auto opH2avvv = WOperator("H2",{0,1,1},{0,0,2});

    auto opH2vvcc = WOperator("H2",{0,0,2},{2,0,0});
    auto opH2vvca = WOperator("H2",{0,0,2},{1,1,0});
    auto opH2vvcv = WOperator("H2",{0,0,2},{1,0,1});
    auto opH2vvaa = WOperator("H2",{0,0,2},{0,2,0});
    auto opH2vvav = WOperator("H2",{0,0,2},{0,1,1});
    auto opH2vvvv = WOperator("H2",{0,0,2},{0,0,2});

    std::vector<WOperator> opT = {opT1ca,opT1cv,opT1av,
                opT2ccaa,opT2ccav,opT2ccvv,opT2caaa,opT2caav,opT2cavv,opT2aaav,opT2aavv};

    std::vector<WOperator> opH = {opH1cc, opH1ca, opH1cv,
                                  opH1ac, opH1aa, opH1av,
                                  opH1vc, opH1va, opH1vv,
                                  opH2cccc, opH2ccca, opH2cccv,
                                  opH2ccaa, opH2ccav, opH2ccvv,
                                  opH2cacc, opH2caca, opH2cacv,
                                  opH2caaa, opH2caav, opH2cavv,
                                  opH2cvcc, opH2cvca, opH2cvcv,
                                  opH2cvaa, opH2cvav, opH2cvvv,
                                  opH2aacc, opH2aaca, opH2aacv,
                                  opH2aaaa, opH2aaav, opH2aavv,
                                  opH2avcc, opH2avca, opH2avcv,
                                  opH2avaa, opH2avav, opH2avvv,
                                  opH2vvcc, opH2vvca, opH2vvcv,
                                  opH2vvaa, opH2vvav, opH2vvvv};

    for (auto op : opT){
        std::cout << op.str() << std::endl;
    }

    for (auto op : opH){
        std::cout << op.str() << std::endl;
    }
    return 0;
}
