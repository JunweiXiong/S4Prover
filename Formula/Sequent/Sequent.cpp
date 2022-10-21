#include "Sequent.h"

// Sequent::Sequent(formula_set left,  formula_set right, formula_set blackbox,
//          formula_set blackdia, formula_set boxL, formula_set boxdiaL,
//          formula_set boxLbox, formula_set boxLdia, formula_set classicL,
//          formula_set boxR, formula_set classicR) {
//           left_ = left; 
// }
Sequent::Sequent(const formula_set &left, const formula_set &right, const formula_set &blackbox,
          const formula_set &blackdia, const formula_set &boxL,const formula_set &boxdiaL,
          const formula_set &boxLbox, const formula_set &boxLdia, const formula_set &diaL, const formula_set &classicL,
          const formula_set &boxR, const formula_set &classicR){
            left_ = left;
            right_ = right;
            blackbox_ = blackbox;
            blackdia_ = blackdia;
            boxL_ = boxL;
            boxdiaL_ = boxdiaL;
            boxLbox_ = boxLbox;
            boxLdia_ = boxLdia;
            diaL_ = diaL;
            classicL_ = classicL;
            boxR_ = boxR;
            classicR_ = classicR;
          }; 

Sequent::~Sequent() {
#if DEBUG_DESTRUCT
  cout << "DESTRUCTING OR" << endl;
#endif
}

bool Sequent::isNormal(){
  if (left_.empty() && right_.empty()){
    return true;
  }else{
    return false;
  }
}

bool Sequent::isRegular(){
  if (Sequent::isNormal() && blackbox_.empty() && blackdia_.empty()){
    return true;
  }else{
    return false;
  }
}

bool Sequent::isCluster(){
  if (Sequent::isRegular() && Sequent::boxdiaL_.empty() && Sequent::boxLbox_.empty() &&
      Sequent::boxLdia_.empty() && Sequent::boxR_.empty()){
    return true;
  }else{
    return false;
  }
}

Sequent Sequent::copy(){
  Sequent newSequent(  left_,   right_,   blackbox_,
            blackdia_,   boxL_,  boxdiaL_,
            boxLbox_,   boxLdia_,   diaL_,   classicL_,
            boxR_,   classicR_);
  return newSequent;
}


 vector<Sequent> Sequent::normalReduction(){
    vector<Sequent> sequents;
    while (!Sequent::isNormal()){
      if(!left_.empty()){
        shared_ptr<Formula> first = *left_.begin();
        left_.erase(first);
        FormulaType type = first->getType();
        switch (type)
        {
        case FAnd:
          And *andFormula = dynamic_cast<And *>(first.get());
          if (andFormula) {
            // left_.insert(andFormula->getSubformulas().begin(),andFormula->getSubformulas().end());
            const formula_set *subformulas = andFormula->getSubformulasReference();
            left_.insert(subformulas->begin(), subformulas->end()); 
          }else{
            cout << "FAnd cast error" << endl;
          }
          
          break;
        case FOr:
          Or *orFormula = dynamic_cast<Or *>(first.get());
          if (orFormula) {
            // left_.insert(andFormula->getSubformulas().begin(),andFormula->getSubformulas().end());
            formula_set subformulas = orFormula->getSubformulas();
            for (shared_ptr<Formula> f:subformulas){
                Sequent newSequent = Sequent::copy();
                newSequent.left_.insert(f);
                vector<Sequent> subSequents = newSequent.normalReduction();
                sequents.insert(sequents.end(),subSequents.begin(),subSequents.end());
            }
          }else{
            cout << "FOr cast error" << endl;
          }
          break;
        case FNot:
          Not *notFormula = dynamic_cast<Not *>(first.get());
          if (notFormula) {
            right_.insert(notFormula->getSubformula());
          }else{
            cout << "FNot cast error" << endl;
          }
          break;
        case FBox:
          Box *boxFormula = dynamic_cast<Box *>(first.get());
          if (boxFormula) {
            shared_ptr<Formula> subFormula = boxFormula->getSubformula();
            if (subFormula->getType()==FOr){

              break;

            }else if (subFormula->getType()==FNot){
              Not *subNotFormula = dynamic_cast<Not *>(subFormula.get());
              if (subNotFormula->getSubformula()->getType()==FBox){
                Box *subSubBoxFormula = dynamic_cast<Box *>(subNotFormula->getSubformula().get());
                shared_ptr<Formula> formula1;
                shared_ptr<Formula> formula2;
                shared_ptr<Formula> p = atomGenerator::generate();
                shared_ptr<Formula> a = subSubBoxFormula->getSubformula();
                formula_set orSet({Not::create(p),Not::create(a)});
                formula1 = Box::create(boxFormula->getModality(),1,Or::create(orSet));
                formula2 = Box::create(boxFormula->getModality(),1,Diamond::create(boxFormula->getModality(),1,p));
                left_.insert(formula1);
                left_.insert(formula2);
                break;
              }

            }else if (subFormula->getType()==FDiamond){
              break;
            }

          }else{
            cout << "FBox cast error" << endl;
          }
          
        case FDiamond:
          Diamond *diaFormula = dynamic_cast<Diamond *>(first.get());
          if (diaFormula){
            shared_ptr<Formula> subFormula = diaFormula->getSubformula();
            shared_ptr<Formula> newFormula = Box::create(diaFormula->getModality(),1,Not::create(subFormula));
            right_.insert(newFormula);
            break;
          }else{
             cout << "FDiamond cast error" << endl;
          }
          
        
        default:
          break;
        } 
      }else{
        
      }
    }
    sequents.push_back(*this);
    return sequents;

 }
