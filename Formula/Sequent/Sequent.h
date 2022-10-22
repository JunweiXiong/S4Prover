#ifndef SEQUENT_H
#define SEQUENT_H

#include "../../Defines/Defines.h"
#include "../And/And.h"
#include "../FEnum/FEnum.h"
#include "../False/False.h"
#include "../Formula/Formula.h"
#include "../True/True.h"
#include "../AtomGenerator/AtomGenerator.h"
#include <functional>
#include <iostream>
#include <memory>
#include <string>
#include <unordered_set>

using namespace std;

class Sequent {
  
public:

  formula_set left_;
  formula_set right_;

  // Theta
  formula_set blackbox_;
  formula_set blackdia_;

  // Gamma
  formula_set boxL_;
  formula_set boxdiaL_;
  formula_set boxLbox_;
  formula_set boxLdia_;

  formula_set diaL_;

  // Gamma_cl
  formula_set classicL_;

  // Delta
  formula_set boxR_;

  // Delta_cl
  formula_set classicR_;



  Sequent(const formula_set &left = formula_set(), const formula_set &right = formula_set(), const formula_set &blackbox = formula_set(),
          const formula_set &blackdia = formula_set(), const formula_set &boxL = formula_set(),const formula_set &boxdiaL = formula_set(),
          const formula_set &boxLbox = formula_set(), const formula_set &boxLdia = formula_set(), const formula_set &diaL = formula_set(), 
          const formula_set &classicL = formula_set(), const formula_set &boxR = formula_set(), const formula_set &classicR = formula_set());
  ~Sequent();

  Sequent Sequent::copy();
  vector<Sequent> normalReduction();
  bool isNormal();
  bool isCluster();
  bool isRegular();

};

#endif