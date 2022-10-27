#ifndef NORMALREDUCTION_H
#define NORMALREDUCTION_H

#include "../../Defines/Defines.h"
#include "../And/And.h"
#include "../FEnum/FEnum.h"
#include "../False/False.h"
#include "../Formula/Formula.h"
#include "../True/True.h"
#include "../AtomGenerator/AtomGenerator.h"
#include "Sequent.h"
#include <functional>
#include <iostream>
#include <memory>
#include <string>
#include <unordered_set>
#include <stack>

using namespace std;

class NormalReduction{

public:

    static vector<Sequent> reduction(Sequent sequent);

};

#endif