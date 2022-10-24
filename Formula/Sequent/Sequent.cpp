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
  Sequent newSequent(  left_, right_, blackbox_,
            blackdia_, boxL_, boxdiaL_,
            boxLbox_, boxLdia_,   diaL_,   classicL_,
            boxR_,   classicR_);
  return newSequent;
}

 string Sequent::toString() const {

  string sequent = "";
  sequent += " left: ";
  for (const shared_ptr<Formula> &f: left_){
    sequent += f->toString();
  }

  sequent += " right: ";
  for (const shared_ptr<Formula> &f: right_){
    sequent += f->toString();
  }

  sequent += " blackbox: ";
  for (const shared_ptr<Formula> &f: blackbox_){
    sequent += f->toString();
  }

  sequent += " blackdia: ";
  for (const shared_ptr<Formula> &f: blackdia_){
    sequent += f->toString();
  }

  sequent += " boxL: ";
  for (const shared_ptr<Formula> &f: boxL_){
    sequent += f->toString();
  }

  sequent += " boxdiaL: ";
  for (const shared_ptr<Formula> &f: boxdiaL_){
    sequent += f->toString();
  }

  sequent += " boxLbox: ";
  for (const shared_ptr<Formula> &f: boxLbox_){
    sequent += f->toString();
  }

  sequent += " boxLdia: ";
  for (const shared_ptr<Formula> &f: boxLdia_){
    sequent += f->toString();
  }

  sequent += " diaL: ";
  for (const shared_ptr<Formula> &f: diaL_){
    sequent += f->toString();
  }

  sequent += " classicL: ";
  for (const shared_ptr<Formula> &f: classicL_){
    sequent += f->toString();
  }

  sequent += " boxR: ";
  for (const shared_ptr<Formula> &f: boxR_){
    sequent += f->toString();
  }

  sequent += " classicR_: ";
  for (const shared_ptr<Formula> &f: classicR_){
    sequent += f->toString();
  }

  return sequent;

}


 vector<Sequent> Sequent::normalReduction(){
    vector<Sequent> sequents;
    while (!Sequent::isNormal()){
      if(!left_.empty()){
        shared_ptr<Formula> firstLeft = *left_.begin();
        left_.erase(firstLeft);

        if (firstLeft->isClassical()){
          classicL_.insert(firstLeft);
          continue;
        }
        FormulaType type = firstLeft->getType();
        if (type == FAnd){
          And *andFormula = dynamic_cast<And *>(firstLeft.get());
          // left_.insert(andFormula->getSubformulas().begin(),andFormula->getSubformulas().end());
          const formula_set *subformulas = andFormula->getSubformulasReference();
          left_.insert(subformulas->begin(), subformulas->end()); 
        }else if (type == FOr){
          Or *orFormula = dynamic_cast<Or *>(firstLeft.get());
          // left_.insert(andFormula->getSubformulas().begin(),andFormula->getSubformulas().end());
          formula_set subformulas = orFormula->getSubformulas();
          for (shared_ptr<Formula> f:subformulas){
              Sequent newSequent = Sequent::copy();
              newSequent.left_.insert(f);
              vector<Sequent> subSequents = newSequent.normalReduction();
              sequents.insert(sequents.end(),subSequents.begin(),subSequents.end());
          }
        }else if (type == FNot){
          Not *notFormula = dynamic_cast<Not *>(firstLeft.get());
          right_.insert(notFormula->getSubformula());
        }else if (type == FBox){
          Box *boxFormula = dynamic_cast<Box *>(firstLeft.get());
          shared_ptr<Formula> boxSubFormula = boxFormula->getSubformula();
          if (boxSubFormula->isClassical()){
            boxL_.insert(firstLeft);
            continue;
          }
          FormulaType boxSubType = boxSubFormula->getType();
          if (boxSubType == FOr){
            Or *boxOrFormula = dynamic_cast<Or *>(boxSubFormula.get());
            formula_set boxOrSubFormulas = boxOrFormula->getSubformulas();
            if (boxOrSubFormulas.size()>2){
              cout << "or size: " << boxOrSubFormulas.size() << " > 2" << endl;;
            }else{
              shared_ptr<Formula> boxOrSubLeft;
              shared_ptr<Formula> boxOrSubRight;
              int counter = 0;
              for (shared_ptr<Formula> f:boxOrSubFormulas){
                if (counter==0){
                  boxOrSubLeft = f;
                  counter++;
                }else{
                  boxOrSubRight = f; 
                }
              }
              bool classicalLeft = boxOrSubLeft->isClassical();
              bool classicalRight = boxOrSubRight->isClassical();
              if (!classicalLeft && !classicalRight){
                shared_ptr<Formula> p = AtomGenerator::generate();
                formula_set set1({Not::create(p),boxOrSubLeft});
                formula_set set2({p,boxOrSubRight});
                left_.insert(Box::create(Or::create(set1,true)));
                left_.insert(Box::create(Or::create(set2,true)));
              }else{
                shared_ptr<Formula> boxOrSubClassic;
                shared_ptr<Formula> boxOrSubNonClassic;
                if (classicalLeft){
                  boxOrSubClassic = boxOrSubLeft;
                  boxOrSubNonClassic = boxOrSubRight;
                }else{
                  boxOrSubClassic = boxOrSubRight;
                  boxOrSubNonClassic = boxOrSubLeft;
                }
                FormulaType boxOrSubType = boxOrSubNonClassic->getType();
                if (boxOrSubType==FAnd){
                  And *boxOrAndFormula = dynamic_cast<And *>(boxOrSubNonClassic.get());
                  formula_set boxOrAndSubFormulas = boxOrAndFormula->getSubformulas();
                  shared_ptr<Formula> boxOrAndLeft = *boxOrAndSubFormulas.begin();
                  boxOrAndSubFormulas.erase(boxOrAndLeft);
                  shared_ptr<Formula> boxOrAndRight;
                  if (boxOrAndSubFormulas.size()>1){
                      boxOrAndRight = And::create(boxOrAndSubFormulas);
                  }else{
                      boxOrAndRight = *boxOrAndSubFormulas.begin();
                  }
                  shared_ptr<Formula> p = AtomGenerator::generate();
                  boxL_.insert(Box::create(Or::create(formula_set({boxOrSubClassic,Not::create(p)}),true)));
                  left_.insert(Box::create(Or::create(formula_set({boxOrAndLeft,p}),true)));
                  left_.insert(Box::create(Or::create(formula_set({boxOrAndRight,p}),true)));
                }else if (boxOrSubType==FOr){
                  Or *boxOrOrFormula = dynamic_cast<Or *>(boxOrSubNonClassic.get());
                  formula_set boxOrOrSubFormulas = boxOrOrFormula->getSubformulas();
                  shared_ptr<Formula> boxOrOrLeft = *boxOrOrSubFormulas.begin();
                  boxOrOrSubFormulas.erase(boxOrOrLeft);
                  shared_ptr<Formula> boxOrOrRight;
                  if (boxOrOrSubFormulas.size()>1){
                    cout << "unexpected or size " << endl;
                    boxOrOrRight = Or::create(boxOrOrSubFormulas,true);
                  }else{
                      boxOrOrRight = *boxOrOrSubFormulas.begin();
                  }
                  shared_ptr<Formula> p = AtomGenerator::generate();
                  left_.insert(Box::create(Or::create(formula_set({Not::create(p),boxOrOrLeft}),true)));
                  left_.insert(Box::create(Or::create({boxOrOrRight,Or::create({p,boxOrSubClassic},true)},true)));

                }else if (boxOrSubType==FNot){
                  Not *boxOrNotFormula = dynamic_cast<Not *>(boxOrSubNonClassic.get());
                  shared_ptr<Formula> boxOrNotSubFormula = boxOrNotFormula->getSubformula();
                  if (boxOrNotSubFormula->getType()==FBox){
                    Box *boxOrNotBoxFormula = dynamic_cast<Box *>(boxOrNotSubFormula.get());
                    shared_ptr<Formula> p = AtomGenerator::generate();
                    shared_ptr<Formula> f = boxOrNotBoxFormula->getSubformula();
                    left_.insert(Box::create(Or::create(formula_set({p,Not::create(f)}),true)));
                    boxLdia_.insert(Box::create(Or::create(formula_set({boxOrSubClassic,Diamond::create(Not::create(p))}),true)));
                  }else{
                    cout << "unexpected boxOrNotSubFormula : "<< firstLeft->toString() << endl;
                  }
                }else if (boxOrSubType==FBox){
                  Box *boxOrBoxFormula = dynamic_cast<Box *>(boxOrSubNonClassic.get());
                  shared_ptr<Formula> f = boxOrBoxFormula->getSubformula();
                  shared_ptr<Formula> p = AtomGenerator::generate();
                  boxLbox_.insert(Box::create(Or::create(formula_set({boxOrSubClassic,Box::create(p)}),true)));
                  left_.insert(Box::create(Or::create(formula_set({Not::create(p),boxOrSubNonClassic}),true)));
                }else if (boxOrSubType==FDiamond){
                  Diamond *boxOrDiaFormula = dynamic_cast<Diamond *>(boxOrSubNonClassic.get());
                  shared_ptr<Formula> f = boxOrDiaFormula->getSubformula();
                  shared_ptr<Formula> p = AtomGenerator::generate();
                  boxLdia_.insert(Box::create(Or::create(formula_set({boxOrSubClassic,Diamond::create(p)}),true)));
                  left_.insert(Box::create(Or::create(formula_set({Not::create(p),boxOrSubNonClassic}),true)));
                }else{
                  cout << "unexpected boxOrSubType " << firstLeft->toString() << endl;
                }

              }
              
            }    
            
          }else if (boxSubType==FNot){
            Not *boxNotFormula = dynamic_cast<Not *>(boxSubFormula.get());
            if (boxNotFormula->getSubformula()->getType()==FBox){
              Box *boxNotBoxFormula = dynamic_cast<Box *>(boxNotFormula->getSubformula().get());
              shared_ptr<Formula> formula1;
              shared_ptr<Formula> formula2;
              shared_ptr<Formula> p = AtomGenerator::generate();
              shared_ptr<Formula> a = boxNotBoxFormula->getSubformula();
              formula_set orSet({Not::create(p),Not::create(a)});
              formula1 = Box::create(Or::create(orSet,true));
              formula2 = Box::create(Diamond::create(p));
              left_.insert(formula1);
              boxdiaL_.insert(formula2);
            }else{
              cout << "unexpected type after box not: " << firstLeft->toString() << endl;
            }

          }else if (boxSubType==FDiamond){
            Diamond *boxDiaFormula = dynamic_cast<Diamond *>(boxSubFormula.get());
            shared_ptr<Formula> a = boxDiaFormula->getSubformula();
            if (a->isClassical()){
              boxdiaL_.insert(firstLeft);
            }else{
              shared_ptr<Formula> p = AtomGenerator::generate();
              formula_set orSet({Not::create(p),a});
              left_.insert(Box::create(Or::create(orSet,true)));
              boxdiaL_.insert(Box::create(Diamond::create(p)));
            }
          }else{
            cout << "unexpected subbox type: " << firstLeft->toString() << endl;
          }

          
        }else if (type == FDiamond){
          Diamond *diaFormula = dynamic_cast<Diamond *>(firstLeft.get());
          shared_ptr<Formula> diaSubFormula = diaFormula->getSubformula();
          shared_ptr<Formula> newFormula = Box::create(Not::create(diaSubFormula));
          right_.insert(newFormula);
        }else{
          cout << "no case found" << endl;
        }  
      }else{ // right side of sequent
        shared_ptr<Formula> firstRight = *right_.begin();
        right_.erase(firstRight);
        if(firstRight->isClassical()){
          classicR_.insert(firstRight);
        }else{
          FormulaType type = firstRight->getType();
          if (type == FAnd){
            And *andFormula = dynamic_cast<And *>(firstRight.get());
            for (shared_ptr<Formula> f:andFormula->getSubformulas()){
              Sequent newSequent = Sequent::copy();
              newSequent.right_.insert(f);
              vector<Sequent> subSequents = newSequent.normalReduction();
              sequents.insert(sequents.end(),subSequents.begin(),subSequents.end());
            }
            
          }else if(type == FOr){
            Or *orFormula = dynamic_cast<Or *>(firstRight.get());
            formula_set orSubformulas = orFormula->getSubformulas();
            for (shared_ptr<Formula> f:orSubformulas){
              right_.insert(f);
            }
          }else if(type == FNot){
            Not *notFormula = dynamic_cast<Not *>(firstRight.get());
            left_.insert(notFormula->getSubformula());
          }else if(type == FBox){
            Box *boxFormula = dynamic_cast<Box *>(firstRight.get());
            shared_ptr<Formula> boxSubFormula = boxFormula->getSubformula();
            if (boxSubFormula->isClassical()){
              boxR_.insert(firstRight);
            }else{
              FormulaType boxSubtype = boxSubFormula->getType();
              if (boxSubtype == FOr){
                Or *boxOrFormula = dynamic_cast<Or *>(boxSubFormula.get());
                shared_ptr<Formula> p = AtomGenerator::generate();
                if (boxOrFormula->getSubformulas().size()==2){
                  for (shared_ptr<Formula> f : boxOrFormula->getSubformulas()){
                  formula_set orSet({p,Not::create(f)});
                  left_.insert(Box::create(Or::create(orSet,true)));
                  boxR_.insert(Box::create(p));
                  }
                }else{
                  cout << "unexpected size: " << boxOrFormula->getSubformulas().size() << endl;
                }
              }else if (boxSubtype == FNot){
                Not *boxNotFormula = dynamic_cast<Not *>(boxSubFormula.get());
                shared_ptr<Formula> f = boxNotFormula->getSubformula();
                shared_ptr<Formula> p = AtomGenerator::generate();
                formula_set orSet({p,Not::create(f)});
                left_.insert(Box::create(Or::create(orSet,true)));
                boxR_.insert(Box::create(p));

              }else if (boxSubtype == FDiamond){
                Diamond *boxDiaFormula = dynamic_cast<Diamond *>(boxSubFormula.get());
                shared_ptr<Formula> p = AtomGenerator::generate();
                shared_ptr<Formula> f = boxDiaFormula->getSubformula();
                formula_set orSet({p,Box::create(Not::create(f))});
                left_.insert(Box::create(Or::create(orSet,true)));
                boxR_.insert(Box::create(f));

              }else{
                cout << "unexpected subtype after right box " << firstRight->toString() << endl;
              }
            }

          }else if(type == FDiamond){
            Diamond *diaFormula = dynamic_cast<Diamond *>(firstRight.get());
            left_.insert(Box::create(Not::create(diaFormula->getSubformula())));
          }else{
            cout << "unexpected type:" << firstRight->toString() << endl;
          }
        }      
      }
    }
    sequents.push_back(*this);
    return sequents;

 }
