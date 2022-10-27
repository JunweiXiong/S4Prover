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
  if (!Sequent::isNormal() || !boxLbox_.empty() || !boxLdia_.empty()){
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



void Sequent::addTheta(Theta t){
  blackbox_.insert(t.blackbox.begin(),t.blackbox.end());
  blackdia_.insert(t.blackdia.begin(),t.blackdia.end());
}
void Sequent::addGamma(Gamma g){
  boxL_.insert(g.boxL.begin(),g.boxL.end());
  boxdiaL_.insert(g.boxdiaL.begin(),g.boxdiaL.end());
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
  g.boxLdia = boxLdia_;
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
  sequent += " \n ";

  sequent += " right: ";
  for (const shared_ptr<Formula> &f: right_){
    sequent += f->toString();
  }
  sequent += " \n ";

  sequent += " blackbox: ";
  for (const shared_ptr<Formula> &f: blackbox_){
    sequent += f->toString();
  }
  sequent += " \n ";

  sequent += " blackdia: ";
  for (const shared_ptr<Formula> &f: blackdia_){
    sequent += f->toString();
  }
  sequent += " \n ";

  sequent += " boxL: ";
  for (const shared_ptr<Formula> &f: boxL_){
    sequent += f->toString();
  }
  sequent += " \n ";

  sequent += " boxdiaL: ";
  for (const shared_ptr<Formula> &f: boxdiaL_){
    sequent += f->toString();
  }
  sequent += " \n ";

  sequent += " boxLbox: ";
  for (const shared_ptr<Formula> &f: boxLbox_){
    sequent += f->toString();
  }
  sequent += " \n ";

  sequent += " boxLdia: ";
  for (const shared_ptr<Formula> &f: boxLdia_){
    sequent += f->toString();
  }
  sequent += " \n ";

  sequent += " diaL: ";
  for (const shared_ptr<Formula> &f: diaL_){
    sequent += f->toString();
  }
  sequent += " \n ";

  sequent += " classicL: ";
  for (const shared_ptr<Formula> &f: classicL_){
    sequent += f->toString();
  }
  sequent += " \n ";

  sequent += " boxR: ";
  for (const shared_ptr<Formula> &f: boxR_){
    sequent += f->toString();
  }
  sequent += " \n ";

  sequent += " classicR_: ";
  for (const shared_ptr<Formula> &f: classicR_){
    sequent += f->toString();
  }
  sequent += " \n ";

  return sequent;

}


