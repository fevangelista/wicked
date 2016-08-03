#include <iostream>
#include <fstream>
#include <string>
#include <iomanip>

#include "orbital_space.h"

using namespace std;

int main (int argc, const char * argv[])
{
    osi = std::make_shared<OrbitalSpaceInfo>();
    osi->default_spaces();

    return 0;
}
