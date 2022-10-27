#include "NormalReduction.h"



vector<Sequent> NormalReduction::reduction(Sequent regularSequent){
    bool log = false;
    stack<Sequent> sequent_queue;
    sequent_queue.push(regularSequent);
    vector<Sequent> normal_sequents;
    while (!sequent_queue.empty()){
      Sequent sequent = sequent_queue.top();
      sequent_queue.pop();
      bool skip = false;
      while (!sequent.isNormal()){
        // cout << "sequents.size " <<sequent.size() << endl;
        // cout << "left size " << sequent.left_.size() << " right size " << sequent.right_.size() << endl;
        if(!sequent.left_.empty()){
          shared_ptr<Formula> firstLeft = *sequent.left_.begin();
          
          sequent.left_.erase(firstLeft);
          if (firstLeft->isClassical()){
              if(log){ cout << "classicL_" << endl;}
              sequent.classicL_.insert(firstLeft);
              continue;
          }
          FormulaType type = firstLeft->getType();
          if (type == FAnd){
              if(log){ cout << "2" << endl;}
              And *andFormula = dynamic_cast<And *>(firstLeft.get());
              const formula_set *subformulas = andFormula->getSubformulasReference();
              sequent.left_.insert(subformulas->begin(), subformulas->end()); 
          }else if (type == FOr){
              if(log){ cout << "3" << endl;}
              Or *orFormula = dynamic_cast<Or *>(firstLeft.get());
              formula_set subformulas = orFormula->getSubformulas();
              for (shared_ptr<Formula> f:subformulas){
                  Sequent newSequent = sequent.copy();
                  newSequent.left_.insert(f);
                  sequent_queue.push(newSequent);
              }
              skip = true;
              break;
          }else if (type == FNot){
            if(log){ cout << "4" << endl;}
            Not *notFormula = dynamic_cast<Not *>(firstLeft.get());
            shared_ptr<Formula> A = notFormula->getSubformula();
            sequent.right_.insert(A);
          }else if (type == FBox){
            Box *boxFormula = dynamic_cast<Box *>(firstLeft.get());
            shared_ptr<Formula> boxSubFormula = boxFormula->getSubformula();
            if (boxSubFormula->isClassical()){
              if(log){ cout << "boxL_" << endl;}
              sequent.boxL_.insert(firstLeft);
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
              if (!boxOrFirst->isClassical() && !boxOrSecond->isClassical()){
                if(log){ cout << "11" << endl;}
                shared_ptr<Formula> p = AtomGenerator::generate();
                sequent.left_.insert(Box::create(Or::create({Not::create(p),boxOrFirst})));
                sequent.left_.insert(Box::create(Or::create({p,boxOrSecond})));
                continue;
              }else if (boxOrFirst->isClassical()){
                Z = boxOrFirst;
                boxOrNonClassic = boxOrSecond;
              }else{
                Z = boxOrSecond;
                boxOrNonClassic = boxOrFirst;
              }

              FormulaType boxOrSubType = boxOrNonClassic->getType();

              if (boxOrSubType==FAnd){
                if(log){ cout << "5" << endl;}
                And *boxOrAndFormula = dynamic_cast<And *>(boxOrNonClassic.get());
                formula_set boxOrAndSet = boxOrAndFormula->getSubformulas();
                shared_ptr<Formula> A = *boxOrAndSet.begin();
                boxOrAndSet.erase(A);
                shared_ptr<Formula> B = And::create(boxOrAndSet,true);
                shared_ptr<Formula> p = AtomGenerator::generate();
                sequent.left_.insert(Box::create(Or::create({Z,Not::create(p)},true)));
                sequent.left_.insert(Box::create(Or::create({p,A},true)));
                sequent.left_.insert(Box::create(Or::create({p,B},true)));
              }else if (boxOrSubType==FOr){
                if(log){ cout << "6" << endl;}
                Or *boxOrOrFormula = dynamic_cast<Or *>(boxOrNonClassic.get());
                formula_set boxOrOrSet = boxOrOrFormula->getSubformulas();
                shared_ptr<Formula> A = *boxOrOrSet.begin();
                boxOrOrSet.erase(A);
                shared_ptr<Formula> B = Or::create(boxOrOrSet,true);
                shared_ptr<Formula> p = AtomGenerator::generate();
                sequent.left_.insert(Box::create(Or::create({Not::create(p),A},true)));
                sequent.left_.insert(Box::create(Or::create({Or::create({p,Z},true),B},true)));
              }else if (boxOrSubType==FNot){
                Not *boxOrNotFormula = dynamic_cast<Not *>(boxOrNonClassic.get());
                shared_ptr<Formula> boxOrNotSubFormula = boxOrNotFormula->getSubformula();
                FormulaType boxOrNotSubType = boxOrNotSubFormula->getType();
                if (boxOrNotSubType==FBox){
                  if(log){ cout << "7" << endl;}
                  Box *boxOrNotBoxFormula = dynamic_cast<Box *>(boxOrNotSubFormula.get());
                  shared_ptr<Formula> A = boxOrNotBoxFormula->getSubformula();
                  shared_ptr<Formula> p = AtomGenerator::generate();
                  sequent.left_.insert(Box::create(Or::create({p,Not::create(A)},true)));
                  sequent.left_.insert(Box::create(Or::create({Z,Diamond::create(Not::create(p))},true)));
                }else{
                  if(log){ cout << "7.5" << endl;}
                  sequent.left_.insert(Box::create(Or::create({Z,boxOrNotFormula->s4reduction()},true)));
                }
              }else if (boxOrSubType==FBox){
                Box *boxOrBoxFormula = dynamic_cast<Box *>(boxOrNonClassic.get());
                shared_ptr<Formula> A = boxOrBoxFormula->getSubformula();
                shared_ptr<Formula> p = AtomGenerator::generate();
                if (A->isClassical()){
                  if(log){ cout << "boxLbox_" << endl;}
                  sequent.boxLbox_.insert(Box::create(Or::create({Z,Box::create(A)},true)));
                }else{
                  if(log){ cout << "8" << endl;}
                  sequent.left_.insert(Box::create(Or::create({Not::create(p),A},true)));
                  sequent.left_.insert(Box::create(Or::create({Z,Box::create(p)},true)));
                }
              }else if (boxOrSubType==FDiamond){
                Diamond *boxOrDiaFormula = dynamic_cast<Diamond *>(boxOrNonClassic.get());
                shared_ptr<Formula> A = boxOrDiaFormula->getSubformula();
                shared_ptr<Formula> p = AtomGenerator::generate();
                if (A->isClassical()){
                  if(log){ cout << "boxLdia_" << endl;}
                  sequent.boxLdia_.insert(Box::create(Or::create({Z,Diamond::create(A)},true)));
                }else{
                  if(log){ cout << "9" << endl;}
                  sequent.left_.insert(Box::create(Or::create({Not::create(p),A},true)));
                  sequent.left_.insert(Box::create(Or::create({Z,Diamond::create(p)},true)));
                }
              }else{
                throw std::invalid_argument( "unexpected boxOrSubType error: " + firstLeft->toString());
              }
              
            }else if (boxSubType==FNot){
              Not *boxNotFormula = dynamic_cast<Not *>(boxSubFormula.get());
              FormulaType boxNotSubtype = boxNotFormula->getSubformula()->getType();
              if (boxNotSubtype==FBox){
                if(log){ cout << "12" << endl;}
                Box *boxNotBoxFormula = dynamic_cast<Box *>(boxNotFormula->getSubformula().get());
                shared_ptr<Formula> p = AtomGenerator::generate();
                shared_ptr<Formula> A = boxNotBoxFormula->getSubformula();
                sequent.left_.insert(Box::create(Or::create({Not::create(p),Not::create(A)},true)));
                sequent.left_.insert(Box::create(Diamond::create(p)));
              }else{ // box not and | box not or | box not dia
              if(log){ cout << "12.5" << endl;}
                sequent.left_.insert(Box::create(boxNotFormula->s4reduction()));
              }
            }else if (boxSubType==FDiamond){
              Diamond *boxDiaFormula = dynamic_cast<Diamond *>(boxSubFormula.get());
              shared_ptr<Formula> A = boxDiaFormula->getSubformula();
              if (A->isClassical()){
                if(log){ cout << "boxdiaL_" << endl;}
                sequent.boxdiaL_.insert(firstLeft);
              }else{
                if(log){ cout << "13" << endl;}
                shared_ptr<Formula> p = AtomGenerator::generate();
                sequent.left_.insert(Box::create(Or::create({Not::create(p),A},true)));
                sequent.left_.insert(Box::create(Diamond::create(p)));
              }
            }else{
              throw std::invalid_argument("unexpected boxSubType error: " + firstLeft->toString());
            }
          }else if (type == FDiamond){
            Diamond *diaFormula = dynamic_cast<Diamond *>(firstLeft.get());
            shared_ptr<Formula> A = diaFormula->getSubformula();
            if(log){ cout << "14" << endl;}
            sequent.right_.insert(Box::create(Not::create(A)));
          }else{
            throw std::invalid_argument("unexpected left type : " + firstLeft->toString());
          }  
        }else{ // right side of sequent
          shared_ptr<Formula> firstRight = *sequent.right_.begin();
          sequent.right_.erase(firstRight);
          if(firstRight->isClassical()){
            if(log){ cout << "classicR_" << endl;}
            sequent.classicR_.insert(firstRight);
          }else{
            FormulaType type = firstRight->getType();
            if (type == FAnd){
              if(log){ cout << "15" << endl;}
              And *andFormula = dynamic_cast<And *>(firstRight.get());
              for (shared_ptr<Formula> f:andFormula->getSubformulas()){
                Sequent newSequent = sequent.copy();
                newSequent.right_.insert(f);
                sequent_queue.push(newSequent);
              }
              skip = true;
              break;
            }else if(type == FOr){
              if(log){ cout << "16" << endl;}
              Or *orFormula = dynamic_cast<Or *>(firstRight.get());
              formula_set orSubformulas = orFormula->getSubformulas();
              for (shared_ptr<Formula> f:orSubformulas){
                sequent.right_.insert(f);
              }
            }else if(type == FNot){
              if(log){ cout << "17" << endl;}
              Not *notFormula = dynamic_cast<Not *>(firstRight.get());
              sequent.left_.insert(notFormula->getSubformula());
            }else if(type == FBox){
              Box *boxFormula = dynamic_cast<Box *>(firstRight.get());
              shared_ptr<Formula> boxSubFormula = boxFormula->getSubformula();
              if (boxSubFormula->isClassical()){
                if(log){ cout << "boxR_" << endl;}
                sequent.boxR_.insert(firstRight);
              }else{ // box subformula not classical
                FormulaType boxSubtype = boxSubFormula->getType();
                if (boxSubtype == FOr){
                  if(log){ cout << "18" << endl;}
                  Or *boxOrFormula = dynamic_cast<Or *>(boxSubFormula.get());
                  formula_set boxOrSet = boxOrFormula->getSubformulas();
                  shared_ptr<Formula> p = AtomGenerator::generate();
                  shared_ptr<Formula> A = *boxOrSet.begin();
                  boxOrSet.erase(A);
                  shared_ptr<Formula> B = Or::create(boxOrSet);
                  
                  sequent.left_.insert(Box::create(Or::create({p,Not::create(A)})));
                  sequent.left_.insert(Box::create(Or::create({p,Not::create(B)})));
                  sequent.right_.insert(Box::create(p));

                }else if (boxSubtype == FNot){
                  if(log){ cout << "19" << endl;}
                  Not *boxNotFormula = dynamic_cast<Not *>(boxSubFormula.get());
                  shared_ptr<Formula> A = boxNotFormula->getSubformula();
                  shared_ptr<Formula> p = AtomGenerator::generate();
                  sequent.left_.insert(Box::create(Or::create({p,A},true)));
                  sequent.right_.insert(Box::create(p));

                }else if (boxSubtype == FDiamond){
                  if(log){ cout << "20" << endl;}
                  Diamond *boxDiaFormula = dynamic_cast<Diamond *>(boxSubFormula.get());
                  shared_ptr<Formula> p = AtomGenerator::generate();
                  shared_ptr<Formula> A = boxDiaFormula->getSubformula();
                  sequent.left_.insert(Box::create(Or::create({p,Box::create(Not::create(A))},true)));
                  sequent.right_.insert(Box::create(p));

                }else{ // box and | box box
                  if(log){ cout << "20.5" << endl;}
                  sequent.right_.insert(boxFormula->s4reduction());
                }
              }
            }else if(type == FDiamond){
              if(log){ cout << "21" << endl;}
              Diamond *diaFormula = dynamic_cast<Diamond *>(firstRight.get());
              shared_ptr<Formula> A = diaFormula->getSubformula();
              sequent.left_.insert(Box::create(Not::create(A)));
            }else{
              throw std::invalid_argument("unexpected right type : " + firstRight->toString());
            }
          }      
        }
      }
      if (!skip){
        normal_sequents.push_back(sequent);
      }  
    }  


    return normal_sequents;

 }