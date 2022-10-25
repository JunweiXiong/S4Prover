#include "Sequent.h"


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

void Sequent::toCluster(){
  if (!Sequent::isNormal() || !boxLbox_.empty() || !boxLdia_.empty() || !diaL_.empty()){
    throw invalid_argument("Cannot convert to cluster form " + Sequent::toString());
  }
  diaL_.insert(blackdia_.begin(),blackdia_.end());
  blackdia_.clear();
  blackbox_.clear();

  for (shared_ptr<Formula> formula : boxdiaL_){
    if (dynamic_cast<Diamond *>((dynamic_cast<Box *>(formula.get())->getSubformula().get()))){
      shared_ptr<Formula> boxDiaFormula = dynamic_cast<Box *>(formula.get())->getSubformula();
      diaL_.insert(boxDiaFormula);
    }else{
      throw invalid_argument("boxdiaL_ error" + Sequent::toString());
    }
  }
  boxdiaL_.clear();

}


// void Sequent::clearTheta(){
//   blackbox_.clear();
//   blackdia_.clear();
// }

// void Sequent::clearGammaSpecial(){
//   boxL_.clear();
//   boxdiaL_.clear();
//   boxLbox_.clear();
//   boxdiaL_.clear();
// }
// void Sequent::clearGammaClassic(){
//   classicL_.clear();
// }
// void Sequent::clearDeltaSpecial(){
//   boxR_.clear();
// }
// void Sequent::clearDeltaClassic(){
//   classicR_.clear();
// }

void Sequent::addTheta(Theta t){
  blackbox_.insert(t.blackbox.begin(),t.blackbox.end());
  blackdia_.insert(t.blackdia.begin(),t.blackdia.end());
}
void Sequent::addGamma(Gamma g){
  boxL_.insert(g.boxL.begin(),g.boxL.end());
  boxdiaL_.insert(g.boxL.begin(),g.boxL.end());
  boxLbox_.insert(g.boxLbox.begin(),g.boxLbox.end());
  boxLdia_.insert(g.boxLdia.begin(),g.boxLdia.end());
}

void Sequent::addGamma_cl(formula_set formulas){
  classicL_.insert(formulas.begin(),formulas.end());
}
void Sequent::addDelta(formula_set formulas){
  boxR_.insert(formulas.begin(),formulas.end());
}
void Sequent::addDelta_cl(formula_set formulas){
  classicR_.insert(formulas.begin(),formulas.end());
}

Theta Sequent::getTheta(){
  Theta t;
  t.blackbox = blackbox_;
  t.blackdia = blackdia_;
  return t;
}
Gamma Sequent::getGamma(){
  Gamma g;
  g.boxL = boxL_;
  g.boxdiaL = boxdiaL_;
  g.boxLbox = boxLbox_;
  g.boxLdia = boxdiaL_;
  return g;
}

formula_set Sequent::getGamma_box(){
  formula_set f_set = boxL_;
  for (shared_ptr<Formula> f : boxLbox_){
    f_set.insert(FormulaExtraction::getBoxC(f));
  }
  return f_set;
}

void Sequent::clear(){
  left_.clear();
  right_.clear();
  blackbox_.clear();
  blackdia_.clear(); 
  boxL_.clear();
  boxdiaL_.clear();  
  boxLbox_.clear();
  boxLdia_.clear();  
  diaL_.clear(); 
  classicL_.clear();
  boxR_.clear();
  classicR_.clear();
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
            const formula_set *subformulas = andFormula->getSubformulasReference();
            left_.insert(subformulas->begin(), subformulas->end()); 
        }else if (type == FOr){
            Or *orFormula = dynamic_cast<Or *>(firstLeft.get());
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
            formula_set boxOrSet = boxOrFormula->getSubformulas();
            shared_ptr<Formula> boxOrFirst = *boxOrSet.begin();
            boxOrSet.erase(boxOrFirst);
            shared_ptr<Formula> boxOrSecond = Or::create(boxOrSet,true);
            shared_ptr<Formula> Z;
            shared_ptr<Formula> boxOrNonClassic;
            if (boxOrFirst->isClassical()){
              Z = boxOrFirst;
              boxOrNonClassic = boxOrSecond;
            }else{
              Z = boxOrSecond;
              boxOrNonClassic = boxOrFirst;
            }

            FormulaType boxOrSubType = boxOrNonClassic->getType();

            if (boxOrSubType==FAnd){
              And *boxOrAndFormula = dynamic_cast<And *>(boxOrNonClassic.get());
              formula_set boxOrAndSet = boxOrAndFormula->getSubformulas();
              shared_ptr<Formula> A = *boxOrAndSet.begin();
              boxOrAndSet.erase(A);
              shared_ptr<Formula> B = And::create(boxOrAndSet,true);
              shared_ptr<Formula> p = AtomGenerator::generate();
              left_.insert(Box::create(Or::create({Z,Not::create(p)},true)));
              left_.insert(Box::create(Or::create({p,A},true)));
              left_.insert(Box::create(Or::create({p,B},true)));
            }else if (boxOrSubType==FOr){
              Or *boxOrOrFormula = dynamic_cast<Or *>(boxOrNonClassic.get());
              formula_set boxOrOrSet = boxOrOrFormula->getSubformulas();
              shared_ptr<Formula> A = *boxOrOrSet.begin();
              boxOrOrSet.erase(A);
              shared_ptr<Formula> B = Or::create(boxOrOrSet,true);
              shared_ptr<Formula> p = AtomGenerator::generate();
              left_.insert(Box::create(Or::create({Not::create(p),A},true)));
              left_.insert(Or::create({Or::create({p,Z},true),B},true));
            }else if (boxOrSubType==FNot){
              Not *boxOrNotFormula = dynamic_cast<Not *>(boxOrNonClassic.get());
              shared_ptr<Formula> boxOrNotSubFormula = boxOrNotFormula->getSubformula();
              FormulaType boxOrNotSubType = boxOrNotSubFormula->getType();
              if (boxOrNotSubType==FBox){
                Box *boxOrNotBoxFormula = dynamic_cast<Box *>(boxOrNotSubFormula.get());
                shared_ptr<Formula> A = boxOrNotBoxFormula->getSubformula();
                shared_ptr<Formula> p = AtomGenerator::generate();
                left_.insert(Box::create(Or::create({p,Not::create(A)},true)));
                left_.insert(Box::create(Or::create({Z,Diamond::create(Not::create(p))},true)));
              }else{
                left_.insert(Box::create(Or::create({Z,boxOrNotFormula->s4reduction()},true)));
              }
            }else if (boxOrSubType==FBox){
              Box *boxOrBoxFormula = dynamic_cast<Box *>(boxOrNonClassic.get());
              shared_ptr<Formula> A = boxOrBoxFormula->getSubformula();
              shared_ptr<Formula> p = AtomGenerator::generate();
              if (A->isClassical()){
                boxLbox_.insert(Box::create(Or::create({Z,boxOrNonClassic},true)));
              }else{
                left_.insert(Box::create(Or::create({Not::create(p),A},true)));
                left_.insert(Box::create(Or::create({Z,Box::create(p)},true)));
              }
            }else if (boxOrSubType==FDiamond){
              Diamond *boxOrDiaFormula = dynamic_cast<Diamond *>(boxOrNonClassic.get());
              shared_ptr<Formula> A = boxOrDiaFormula->getSubformula();
              shared_ptr<Formula> p = AtomGenerator::generate();
              if (A->isClassical()){
                boxLdia_.insert(Box::create(Or::create({Z,boxOrNonClassic},true)));
              }else{
                left_.insert(Box::create(Or::create({Not::create(p),A},true)));
                left_.insert(Box::create(Or::create({Z,Diamond::create(p)},true)));
              }
            }else{
              cout << "unexpected boxOrSubType error: " << firstLeft->toString() << endl;
            }
            
          }else if (boxSubType==FNot){
            Not *boxNotFormula = dynamic_cast<Not *>(boxSubFormula.get());
            FormulaType boxNotSubtype = boxNotFormula->getSubformula()->getType();
            if (boxNotSubtype==FBox){
              Box *boxNotBoxFormula = dynamic_cast<Box *>(boxNotFormula->getSubformula().get());
              shared_ptr<Formula> p = AtomGenerator::generate();
              shared_ptr<Formula> A = boxNotBoxFormula->getSubformula();
              left_.insert(Box::create(Or::create({Not::create(p),Not::create(A)},true)));
              left_.insert(Box::create(Diamond::create(p)));
            }else{ // box not and | box not or | box not dia
              left_.insert(Box::create(boxNotFormula->s4reduction()));
            }
          }else if (boxSubType==FDiamond){
            Diamond *boxDiaFormula = dynamic_cast<Diamond *>(boxSubFormula.get());
            shared_ptr<Formula> A = boxDiaFormula->getSubformula();
            if (A->isClassical()){
              boxdiaL_.insert(firstLeft);
            }else{
              shared_ptr<Formula> p = AtomGenerator::generate();
              left_.insert(Box::create(Or::create({Not::create(p),A},true)));
              left_.insert(Box::create(Diamond::create(p)));
            }
          }else{
            cout << "unexpected boxSubType error: " << firstLeft->toString() << endl;
          }
        }else if (type == FDiamond){
          Diamond *diaFormula = dynamic_cast<Diamond *>(firstLeft.get());
          shared_ptr<Formula> A = diaFormula->getSubformula();
          if (A->isClassical()){
            diaL_.insert(firstLeft);
          }else{
            left_.insert(Box::create(Not::create(A)));
          }
        }else{
          cout << "unexpected left type : " << firstLeft->toString() << endl;
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
            }else{ // box subformula not classical
              FormulaType boxSubtype = boxSubFormula->getType();
              if (boxSubtype == FOr){
                Or *boxOrFormula = dynamic_cast<Or *>(boxSubFormula.get());
                formula_set boxOrSet = boxOrFormula->getSubformulas();
                shared_ptr<Formula> p = AtomGenerator::generate();
                shared_ptr<Formula> A = *boxOrSet.begin();
                boxOrSet.erase(A);
                shared_ptr<Formula> B = Or::create(boxOrSet);
                
                left_.insert(Box::create(Or::create({p,Not::create(A)})));
                left_.insert(Box::create(Or::create({p,Not::create(B)})));
                right_.insert(Box::create(p));

              }else if (boxSubtype == FNot){
                Not *boxNotFormula = dynamic_cast<Not *>(boxSubFormula.get());
                shared_ptr<Formula> A = boxNotFormula->getSubformula();
                shared_ptr<Formula> p = AtomGenerator::generate();
                left_.insert(Box::create(Or::create({p,Not::create(A)},true)));
                right_.insert(Box::create(p));

              }else if (boxSubtype == FDiamond){
                Diamond *boxDiaFormula = dynamic_cast<Diamond *>(boxSubFormula.get());
                shared_ptr<Formula> p = AtomGenerator::generate();
                shared_ptr<Formula> A = boxDiaFormula->getSubformula();
                left_.insert(Box::create(Or::create({p,Box::create(Not::create(A))},true)));
                right_.insert(Box::create(p));

              }else{ // box and | box box
                right_.insert(boxFormula->s4reduction());
              }
            }
          }else if(type == FDiamond){
            Diamond *diaFormula = dynamic_cast<Diamond *>(firstRight.get());
            shared_ptr<Formula> A = diaFormula->getSubformula();
            left_.insert(Box::create(Not::create(A)));
          }else{
            cout << "unexpected right type : " << firstRight->toString() << endl;
          }
        }      
      }
    }
    sequents.push_back(*this);
    return sequents;

 }
