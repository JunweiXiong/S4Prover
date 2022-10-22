#ifndef ATOMGENERATOR_H
#define ATOMGENERATOR_H

#include "../Formula/Formula.h"
#include "../Atom/Atom.h"

using namespace std;

class atomGenerator{
    int a = 0;

public:
    int c = 0;

    shared_ptr<Formula> static generate();

};

#endif