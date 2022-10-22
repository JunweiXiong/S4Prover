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
        shared_ptr<Formula> firstLeft = *left_.begin();
        left_.erase(firstLeft);

        if (firstLeft->isClassical()){
          classicL_.insert(firstLeft);
          continue;
        }
        FormulaType type = firstLeft->getType();
        if (type == FAnd){
          And *andFormula = dynamic_cast<And *>(firstLeft.get());
          if (andFormula) {
            // left_.insert(andFormula->getSubformulas().begin(),andFormula->getSubformulas().end());
            const formula_set *subformulas = andFormula->getSubformulasReference();
            left_.insert(subformulas->begin(), subformulas->end()); 
          }else{
            cout << "FAnd cast error" << endl;
          }
        }else if (type == FOr){
          Or *orFormula = dynamic_cast<Or *>(firstLeft.get());
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
        }else if (type == FNot){
           Not *notFormula = dynamic_cast<Not *>(firstLeft.get());
          if (notFormula) {
            right_.insert(notFormula->getSubformula());
          }else{
            cout << "FNot cast error" << endl;
          }
        }else if (type == FBox){
          Box *boxFormula = dynamic_cast<Box *>(firstLeft.get());
          if (boxFormula) {
            shared_ptr<Formula> boxSubFormula = boxFormula->getSubformula();
            if (boxSubFormula->isClassical()){
              boxL_.insert(firstLeft);
              continue;
            }
            FormulaType boxSubType = boxSubFormula->getType();
            if (boxSubType == FOr){
              Or *boxOrFormula = dynamic_cast<Or *>(boxSubFormula.get());
              if (boxOrFormula){
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
                  if (!classicalLeft && !classicalLeft){
                    shared_ptr<Formula> p = AtomGenerator::generate();
                    formula_set set1({Not::create(p),boxOrSubLeft});
                    formula_set set2({p,boxOrSubRight});
                    left_.insert(Box::create(Or::create(set1)));
                    left_.insert(Box::create(Or::create(set2)));
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
                      if (boxOrAndFormula){
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
                        boxL_.insert(Box::create(Or::create(formula_set({boxOrSubClassic,Not::create(p)}))));
                        left_.insert(Box::create(Or::create(formula_set({boxOrAndLeft,p}))));
                        left_.insert(Box::create(Or::create(formula_set({boxOrAndRight,p}))));
                      }else{
                        cout << "boxOrSubType and cast error" << endl;
                      }
                    }else if (boxOrSubType==FOr){
                      Or *boxOrOrFormula = dynamic_cast<Or *>(boxOrSubNonClassic.get());
                      if (boxOrOrFormula){
                        formula_set boxOrOrSubFormulas = boxOrOrFormula->getSubformulas();
                        shared_ptr<Formula> boxOrOrLeft = *boxOrOrSubFormulas.begin();
                        boxOrOrSubFormulas.erase(boxOrOrLeft);
                        shared_ptr<Formula> boxOrOrRight;
                        if (boxOrOrSubFormulas.size()>1){
                          cout << "unexpected or size " << endl;
                          boxOrOrRight = Or::create(boxOrOrSubFormulas);
                        }else{
                            boxOrOrRight = *boxOrOrSubFormulas.begin();
                        }
                        shared_ptr<Formula> p = AtomGenerator::generate();
                        left_.insert(Box::create(Or::create(formula_set({Not::create(p),boxOrOrLeft}))));
                        left_.insert(Box::create(Or::create({boxOrOrRight,Or::create({p,boxOrSubClassic})})));
                      }else{
                        cout << "boxOrSubType or cast error" << endl;
                      }
                    }else if (boxOrSubType==FNot){
                      Not *boxOrNotFormula = dynamic_cast<Not *>(boxOrSubNonClassic.get());
                      if (boxOrNotFormula){
                        shared_ptr<Formula> boxOrNotSubFormula = boxOrNotFormula->getSubformula();
                        if (boxOrNotSubFormula->getType()==FBox){
                          Box *boxOrNotBoxFormula = dynamic_cast<Box *>(boxOrNotSubFormula.get());
                          if (boxOrNotBoxFormula){
                            shared_ptr<Formula> p = AtomGenerator::generate();
                            shared_ptr<Formula> f = boxOrNotBoxFormula->getSubformula();
                            left_.insert(Box::create(Or::create(formula_set({p,Not::create(f)}))));
                            boxLdia_.insert(Box::create(Or::create(formula_set({boxOrSubClassic,Diamond::create(Not::create(p))}))));
                          }else{
                            cout << "boxOrNotBoxFormula cast error" << endl;
                          }

                        }else{
                          cout << "unexpected boxOrNotSubFormula : "<< firstLeft->toString();
                        }
                      }else{
                        cout << "boxOrSubClassic cast error" << endl;
                      }
                    }else if (boxOrSubType==FBox){
                      Box *boxOrBoxFormula = dynamic_cast<Box *>(boxOrSubNonClassic.get());
                      shared_ptr<Formula> f = boxOrBoxFormula->getSubformula();
                      shared_ptr<Formula> p = AtomGenerator::generate();
                      boxLbox_.insert(Box::create(Or::create(formula_set({boxOrSubClassic,Box::create(p)}))));
                      left_.insert(Box::create(Or::create(formula_set({Not::create(p),boxOrSubNonClassic}))));
                    }else if (boxOrSubType==FDiamond){
                      Diamond *boxOrDiaFormula = dynamic_cast<Diamond *>(boxOrSubNonClassic.get());
                      shared_ptr<Formula> f = boxOrDiaFormula->getSubformula();
                      shared_ptr<Formula> p = AtomGenerator::generate();
                      boxLdia_.insert(Box::create(Or::create(formula_set({boxOrSubClassic,Diamond::create(p)}))));
                      left_.insert(Box::create(Or::create(formula_set({Not::create(p),boxOrSubNonClassic}))));
                    }else{
                      cout << "unexpected boxOrSubType " << firstLeft->toString() << endl;
                    }

                  }
                  
                }    
              }else{
                cout << "boxOrFormula cast error" << endl;
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
                formula1 = Box::create(Or::create(orSet));
                formula2 = Box::create(Diamond::create(p));
                left_.insert(formula1);
                boxdiaL_.insert(formula2);
              }else{
                cout << "unexpected type after box not: " << firstLeft->toString() << endl;
              }

            }else if (boxSubType==FDiamond){
              Diamond *boxDiaFormula = dynamic_cast<Diamond *>(boxSubFormula.get());
              if (boxDiaFormula){
                shared_ptr<Formula> a = boxDiaFormula->getSubformula();
                if (a->isClassical()){
                  boxdiaL_.insert(firstLeft);
                }else{
                  shared_ptr<Formula> p = AtomGenerator::generate();
                  formula_set orSet({Not::create(p),a});
                  left_.insert(Box::create(Or::create(orSet)));
                  boxdiaL_.insert(Box::create(Diamond::create(p)));
                }
              }else{
                cout << "subdiaformula cast error" << endl;
              }
            }else{
              cout << "unexpected subbox type: " << firstLeft->toString() << endl;
            }

          }else{
            cout << "FBox cast error" << endl;
          }
          
        }else if (type == FDiamond){
           Diamond *diaFormula = dynamic_cast<Diamond *>(firstLeft.get());
          if (diaFormula){
            shared_ptr<Formula> diaSubFormula = diaFormula->getSubformula();
            shared_ptr<Formula> newFormula = Box::create(Not::create(diaSubFormula));
            right_.insert(newFormula);
          }else{
             cout << "FDiamond cast error" << endl;
          }
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
            if (andFormula) {
              for (shared_ptr<Formula> f:andFormula->getSubformulas()){
                Sequent newSequent = Sequent::copy();
                newSequent.right_.insert(f);
                vector<Sequent> subSequents = newSequent.normalReduction();
                sequents.insert(sequents.end(),subSequents.begin(),subSequents.end());
              }
            }else{
              cout << "FAnd cast error" << endl;
            }
          }else if(type == FOr){
            Or *orFormula = dynamic_cast<Or *>(firstRight.get());
            if (orFormula) {
              formula_set orSubformulas = orFormula->getSubformulas();
              for (shared_ptr<Formula> f:orSubformulas){
                right_.insert(f);
              }
            }else{
              cout << "FOr cast error" << endl;
            }
          }else if(type == FNot){
            Not *notFormula = dynamic_cast<Not *>(firstRight.get());
            if (notFormula) {
              left_.insert(notFormula->getSubformula());
            }else{
              cout << "FNot cast error" << endl;
            }
          }else if(type == FBox){
            Box *boxFormula = dynamic_cast<Box *>(firstRight.get());
            if (boxFormula) {
              shared_ptr<Formula> boxSubFormula = boxFormula->getSubformula();
              if (boxSubFormula->isClassical()){
                boxR_.insert(firstRight);
              }else{
                FormulaType boxSubtype = boxSubFormula->getType();
                if (boxSubtype == FOr){
                  Or *boxOrFormula = dynamic_cast<Or *>(boxSubFormula.get());
                  if (boxOrFormula){
                    shared_ptr<Formula> p = AtomGenerator::generate();
                    if (boxOrFormula->getSubformulas().size()==2){
                      for (shared_ptr<Formula> f : boxOrFormula->getSubformulas()){
                      formula_set orSet({p,Not::create(f)});
                      left_.insert(Box::create(Or::create(orSet)));
                      boxR_.insert(Box::create(p));
                      }
                    }else{
                      cout << "unexpected size: " << boxOrFormula->getSubformulas().size() << endl;
                    }
                  }
                }else if (boxSubtype == FNot){
                  Not *boxNotFormula = dynamic_cast<Not *>(boxSubFormula.get());
                  if (boxNotFormula){
                    shared_ptr<Formula> f = boxNotFormula->getSubformula();
                    shared_ptr<Formula> p = AtomGenerator::generate();
                    formula_set orSet({p,Not::create(f)});
                    left_.insert(Box::create(Or::create(orSet)));
                    boxR_.insert(Box::create(p));
                  }else{
                    cout << "suborformula cast error" << endl;
                  }
                }else if (boxSubtype == FDiamond){
                  Diamond *boxDiaFormula = dynamic_cast<Diamond *>(boxSubFormula.get());
                  if (boxDiaFormula){
                    shared_ptr<Formula> p = AtomGenerator::generate();
                    shared_ptr<Formula> f = boxDiaFormula->getSubformula();
                    formula_set orSet({p,Box::create(Not::create(f))});
                    left_.insert(Box::create(Or::create(orSet)));
                    boxR_.insert(Box::create(f));
                  }else{
                    cout << "boxDiaFormula cast error" << endl;  
                  }
                }else{
                  cout << "unexpected subtype after right box " << firstRight->toString() << endl;
                }
              }
            }else{
              cout << "FBox cast error" << endl;
            }
          }else if(type == FDiamond){
            Diamond *diaFormula = dynamic_cast<Diamond *>(firstRight.get());
            if (diaFormula){
              left_.insert(Box::create(Not::create(diaFormula->getSubformula())));
            }else{
              cout << "diamond formula cast error" << endl;
            }
          }else{
            cout << "unexpected type:" << firstRight->toString() << endl;
          }
        }      
      }
    }
    sequents.push_back(*this);
    return sequents;

 }
