#ifndef MINISAT_PROVER_H
#define MINISAT_PROVER_H


#include "../../Formula/Atom/Atom.h"
#include "../../Formula/FEnum/FEnum.h"
#include "../../Formula/Not/Not.h"
#include "../../Formula/Or/Or.h"
#include "../../Defines/Defines.h"
#include <exception>
#include <memory>
// #include <minisat/core/Solver.h>
#include <minisat/simp/SimpSolver.h>
#include <string>
#include <unordered_map>
#include <unordered_set>

using namespace std;


class MinisatProver {
private:
  shared_ptr<Minisat::SimpSolver> solver =
      shared_ptr<Minisat::SimpSolver>(new Minisat::SimpSolver());

  unordered_map<string, Minisat::Var> variableMap;
  unordered_map<Minisat::Var, string> nameMap;

  void addFalse();
  void prepareNames(name_set names);
  void addClauses(formula_set clauses);

  
  name_set getNames(formula_set clauses);
  Minisat::Var createOrGetVariable(string name);
  Minisat::Lit makeLiteral(shared_ptr<Formula> formula);


public:
  MinisatProver(shared_ptr<Formula> formula);
  ~MinisatProver();

  void prepareSAT(shared_ptr<Formula> formula);
  
  bool solve(); // true = satisfible, false = unsatisfible


};

#endif