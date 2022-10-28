#include "Formula/And/And.h"
#include "Formula/Atom/Atom.h"
#include "Formula/Box/Box.h"
#include "Formula/Diamond/Diamond.h"
#include "Formula/FEnum/FEnum.h"
#include "Formula/False/False.h"
#include "Formula/Formula/Formula.h"
#include "Formula/Not/Not.h"
#include "Formula/Or/Or.h"
#include "Formula/True/True.h"
#include "Formula/AtomGenerator/AtomGenerator.h"
#include "Formula/Sequent/Sequent.h"
#include "Formula/Sequent/NormalReduction.h"
#include "ParseFormula/ParseFormula.h"
#include "ParseFormulaNew/ParseFormulaNew.h"
#include "Prover/MinisatProver/MinisatProver.h"
#include "S4Prover/Search.h"
#include <argp.h>
#include <chrono>
#include <fstream>
#include <functional>
#include <iostream>
#include <memory>
#include <minisat/core/Solver.h>
#include <string>
#include <unordered_set>

using namespace std;


int main(int argc, char *argv[]){
  string filename = argv[1];

  shared_ptr<Formula> formula = ParseFormula(&filename).parseFormula();

  formula = Not::create(formula);
  formula = formula->s4reductionRecursive();

  // cout << formula->toString() << endl;
  
  Sequent sequent;
  sequent.right_.insert(formula);

  // cout << "regular sequent " << endl;
  // cout << sequent.toString() << endl;

  // cout << "start reduction" << endl;
  vector<Sequent> normal_sequents = NormalReduction::reduction(sequent);
  // cout << "end reduction" << endl;

  // for (Sequent s : normal_sequents){
  //   cout << "normal sequent " << endl;
  //   cout << s.toString() << endl;
  // }

  // cout << "normal sequent number: " << normal_sequents.size() << endl;


  // cout << "start search" << endl;
  bool valid = true;
  int counter = 0;
  for (Sequent s : normal_sequents){
    // cout << s.blackbox_.size()<< " " << s.blackdia_.size() << " " << s.boxL_.size() << " " << 
    // s.boxdiaL_.size() << " " << s.boxLbox_.size() << " " << s.boxLdia_.size() <<  "  diaB " << s.diaL_.size() <<  " " <<
    // s.classicL_.size()<< " " << s.boxR_.size()<< " " << s.classicR_.size()  << endl;
    counter++;
    if (!Search::search(s)){
      valid = false;
      break;
    }
    // cout << counter << endl;
  }
  // cout  << endl << "end search" << endl;

  if (valid){
    cout << "unsatisfible" << endl;
    // cout << valid << "negation valid means unsatisfible" << endl;
  }else{
    cout << "satisfible" << endl;
    // cout << valid << "negation refutable means satisfible" << endl;
  }

}

