#ifndef FORMULAEXTRACTION_H
#define FORMULAEXTRACTION_H

#include "../Formula/FEnum/FEnum.h"
#include "../Formula/Formula/Formula.h"
#include "../Formula/Sequent/Sequent.h"
#include <functional>
#include <iostream>
#include <string>
#include <stdexcept>

using namespace std;

class FormulaExtraction {
public:

    static shared_ptr<Formula> getBoxC(shared_ptr<Formula> formula){
        if (formula->getType()==FBox){
            Box *boxformula = dynamic_cast<Box *>(formula.get());
            if (boxformula->getSubformula()->getType()==FOr){
                Or *boxOrformula = dynamic_cast<Or *>(boxformula->getSubformula().get());
                formula_set boxOrSet = boxOrformula->getSubformulas();
                if (boxOrSet.size()==2){
                    shared_ptr<Formula> first =  *boxOrSet.begin();
                    if (first->getType()!=FBox){
                        return Box::create(first);
                    }else{
                        boxOrSet.erase(formula);
                        shared_ptr<Formula> second = *boxOrSet.begin();
                        if (second->getType()!=FBox){
                            return Box::create(second);
                        }else{
                            throw std::invalid_argument( "getBoxC: two box: "+formula->toString());
                        }
                    }
                }else{
                    throw std::invalid_argument( "getBoxC: wrong boxOrSet size: "+formula->toString());
                }
            }else{
                throw std::invalid_argument( "getBoxC: wrong input type: "+formula->toString());
            }
        }else{
            throw std::invalid_argument( "getBoxC: wrong input type: "+formula->toString());
        }
    }
    static shared_ptr<Formula> getBoxD(shared_ptr<Formula> formula){
        if (formula->getType()==FBox){
            Box *boxformula = dynamic_cast<Box *>(formula.get());
            if (boxformula->getSubformula()->getType()==FOr){
                Or *boxOrformula = dynamic_cast<Or *>(boxformula->getSubformula().get());
                formula_set boxOrSet = boxOrformula->getSubformulas();
                if (boxOrSet.size()==2){
                    shared_ptr<Formula> first =  *boxOrSet.begin();
                    if (first->getType()==FBox){
                        return first;
                    }else{
                        boxOrSet.erase(formula);
                        shared_ptr<Formula> second = *boxOrSet.begin();
                        if (second->getType()==FBox){
                            return second;
                        }else{
                            throw std::invalid_argument( "getBoxD: boxC not found: "+formula->toString());
                        }
                    }
                }else{
                    throw std::invalid_argument( "getBoxD: wrong boxOrSet size: "+formula->toString());
                }
            }else{
                throw std::invalid_argument( "getBoxD: wrong input type: "+formula->toString());
            }
        }else{
            throw std::invalid_argument( "getBoxD: wrong input type: "+formula->toString());
        }
    }
    static shared_ptr<Formula> getBoxE(shared_ptr<Formula> formula){
        if (formula->getType()==FBox){
            Box *boxformula = dynamic_cast<Box *>(formula.get());
            if (boxformula->getSubformula()->getType()==FOr){
                Or *boxOrformula = dynamic_cast<Or *>(boxformula->getSubformula().get());
                formula_set boxOrSet = boxOrformula->getSubformulas();
                if (boxOrSet.size()==2){
                    shared_ptr<Formula> first =  *boxOrSet.begin();
                    if (first->getType()!=FDiamond){
                        return Box::create(first);
                    }else{
                        boxOrSet.erase(formula);
                        shared_ptr<Formula> second = *boxOrSet.begin();
                        if (second->getType()!=FDiamond){
                            return Box::create(second);
                        }else{
                            throw std::invalid_argument( "getBoxE: two diamond: "+formula->toString());
                        }
                    }
                }else{
                    throw std::invalid_argument( "getBoxE: wrong boxOrSet size: "+formula->toString());
                }
            }else{
                throw std::invalid_argument( "getBoxE: wrong input type: "+formula->toString());
            }
        }else{
            throw std::invalid_argument( "getBoxE: wrong input type: "+formula->toString());
        }
    };
    static shared_ptr<Formula> getDiaF(shared_ptr<Formula> formula){
        if (formula->getType()==FBox){
            Box *boxformula = dynamic_cast<Box *>(formula.get());
            if (boxformula->getSubformula()->getType()==FOr){
                Or *boxOrformula = dynamic_cast<Or *>(boxformula->getSubformula().get());
                formula_set boxOrSet = boxOrformula->getSubformulas();
                if (boxOrSet.size()==2){
                    shared_ptr<Formula> first =  *boxOrSet.begin();
                    if (first->getType()==FDiamond){
                        return first;
                    }else{
                        boxOrSet.erase(formula);
                        shared_ptr<Formula> second = *boxOrSet.begin();
                        if (second->getType()==FDiamond){
                            return second;
                        }else{
                            throw std::invalid_argument( "getDiaF: DiaF not found: "+formula->toString());
                        }
                    }
                }else{
                    throw std::invalid_argument( "getDiaF: wrong boxOrSet size: "+formula->toString());
                }
            }else{
                throw std::invalid_argument( "getDiaF: wrong input type: "+formula->toString());
            }
        }else{
            throw std::invalid_argument( "getDiaF: wrong input type: "+formula->toString());
        }
    };   

};

#endif