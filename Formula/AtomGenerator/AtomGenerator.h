#ifndef ATOMGENERATOR_H
#define ATOMGENERATOR_H


#include "../Atom/Atom.h"

using namespace std;

class atomGenerator{

public:
    shared_ptr<Formula> static generate();

};

#endif