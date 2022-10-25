#ifndef SEARCH_H
#define SEARCH_H

#include "../Defines/Defines.h"
#include "../Formula/FEnum/FEnum.h"
#include "../Formula/False/False.h"
#include "../Formula/Formula/Formula.h"
#include "../Formula/Sequent/Sequent.h"
#include "../Prover/MinisatProver/MinisatProver.h"
#include "FormulaExtraction.h"
#include <functional>
#include <iostream>
#include <string>
#include <stdexcept>

using namespace std;

class Search {
public:

    static bool search( Sequent sequent);
    static bool searchRegular( Sequent sequent);
    static bool searchSpecial( Sequent sequent);

    static bool searchCluster( Sequent sequent); // true = valid, false = refutable

};

#endif