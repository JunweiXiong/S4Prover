#ifndef ATOMGENERATOR_H
#define ATOMGENERATOR_H

#include "../Formula/Formula.h"
#include "../Atom/Atom.h"

using namespace std;

class AtomGenerator{

public:

    shared_ptr<Formula> static generate();

};

#endif