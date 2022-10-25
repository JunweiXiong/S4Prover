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

  Sequent sequent;
  sequent.right_.insert(formula);
  cout << "start normal reduction" << endl;
  vector<Sequent> normal_sequents = sequent.normalReduction();
  cout << "finish normal reduction" << endl;

  for (Sequent s : normal_sequents){
    cout << Search::search(s) << endl;
  }


}

