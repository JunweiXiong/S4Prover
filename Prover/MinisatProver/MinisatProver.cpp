#include "MinisatProver.h"

MinisatProver::MinisatProver(shared_ptr<Formula> formula) {
  solver->eliminate(true);
  prepareSAT(formula);
}
MinisatProver::~MinisatProver() {}

void MinisatProver::prepareSAT(shared_ptr<Formula> formula) {

  formula = formula->flatten()->negatedNormalForm()->simplify();

  // initialize clause set
  formula_set clauseSet;
  if (formula->getType() == FAnd){
    And *andFormula = dynamic_cast<And *>(formula.get());
    clauseSet = andFormula->getSubformulas();
  }else{
    clauseSet.insert(formula);
  }


  // remove true and false
  formula_set newClauseSet;
  for (shared_ptr<Formula> clause : clauseSet) {
    shared_ptr<Formula> newClause;
    if (clause->getType() == FTrue) {
      clause = Not::create(Atom::create("$false"));
    } else if (clause->getType() == FFalse) {
      clause = Atom::create("$false");
    }
    newClauseSet.insert(clause);
  }
  
  //  get all distinct names 
  name_set names = MinisatProver::getNames(newClauseSet);  
  
  prepareNames(names);

  addFalse();

  addClauses(newClauseSet);

}

name_set MinisatProver::getNames(formula_set clauses){
  name_set names;
  for (shared_ptr<Formula> clause : clauses) {
    if (clause->getType() == FAtom) {
      names.insert(dynamic_cast<Atom *>(clause.get())->getName());
    }else if (clause->getType() == FNot) {
      if (dynamic_cast<Not *>(clause.get())->getSubformula()->getType()==FAtom){
        names.insert(dynamic_cast<Atom *>(
                      dynamic_cast<Not *>(clause.get())->getSubformula().get())
                      ->getName());
      }else{
        throw invalid_argument("getName(): Expected Not Atom but got " + clause->toString());
      }
    }else if (clause->getType() == FOr){
      for (shared_ptr<Formula> subformula : dynamic_cast<Or *>(clause.get())->getSubformulas()){
        if (subformula->getType() == FAtom) {
          names.insert(dynamic_cast<Atom *>(subformula.get())->getName());
        }else if (subformula->getType() == FNot) {
          names.insert(dynamic_cast<Atom *>(
                          dynamic_cast<Not *>(subformula.get())->getSubformula().get())
                          ->getName());
        }else{
          throw invalid_argument("getName(): Expected Atom or Not but got " + clause->toString());
        }  
      }
    }else{
      throw invalid_argument("Expected Atom,Not,Or but got " + clause->toString());
    }
  }
  return names;
}

void MinisatProver::addFalse() {
  solver->addClause(~Minisat::mkLit(createOrGetVariable("$false")));
}

void MinisatProver::prepareNames(name_set names) {
  for (string name : names) {
    createOrGetVariable(name);
  }
}

void MinisatProver::addClauses(formula_set clauses) {
  for (shared_ptr<Formula> clause : clauses) {
    if (clause->getType() == FOr) {
      Minisat::vec<Minisat::Lit> literals;
      for (shared_ptr<Formula> subformula :
           dynamic_cast<Or *>(clause.get())->getSubformulas()) {
        literals.push(makeLiteral(subformula));
      }
      solver->addClause(literals);
    } else {
      solver->addClause(makeLiteral(clause));
    }
  }
}

Minisat::Var MinisatProver::createOrGetVariable(string name) {
  if (variableMap.find(name) == variableMap.end()) {
    variableMap[name] = solver->newVar();
    nameMap[variableMap[name]] = name;
  }
  return variableMap[name];
}

Minisat::Lit MinisatProver::makeLiteral(shared_ptr<Formula> formula) {
  if (formula->getType() == FAtom) {
    string name = dynamic_cast<Atom *>(formula.get())->getName();
    return Minisat::mkLit(createOrGetVariable(name));
  } else if (formula->getType() == FNot) {
    string name = dynamic_cast<Atom *>(
                      dynamic_cast<Not *>(formula.get())->getSubformula().get())
                      ->getName();
    return ~Minisat::mkLit(createOrGetVariable(name));
  }
  throw invalid_argument("makeLiteral(): Expected Atom or Not but got " + formula->toString());
}



bool MinisatProver::solve() {
  return solver->solve();
}


