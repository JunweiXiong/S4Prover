#include "Search.h"

bool search(Sequent sequent){
    if (!sequent.isNormal()){
        if (sequent.isRegular()){
            return Search::searchRegular(sequent);
        }else{
            return Search::searchSpecial(sequent);
        }  
    }else{
        throw std::invalid_argument( "sequent not in normal form");
    } 
}


bool Search::searchSpecial(Sequent sequent){
    Theta theta = sequent.getTheta();
    Gamma gamma = sequent.getGamma();
    formula_set gamma_cl = sequent.classicL_;
    formula_set delta = sequent.boxR_;
    formula_set delta_cl = sequent.classicR_;
    formula_set gamma_box = sequent.getGamma_box();

    if (gamma.boxLbox.empty() && gamma.boxLdia.empty()){
        Sequent clusterSequent = sequent.copy();
        clusterSequent.toCluster();
        return Search::searchCluster(sequent);
    }

    Sequent newSequent;

    for (shared_ptr<Formula> bcbd : sequent.boxLbox_){
        Gamma gamma_bcbd = gamma;
        gamma_bcbd.boxLbox.erase(bcbd);
        shared_ptr<Formula> boxC = FormulaExtraction::getBoxC(bcbd);
        shared_ptr<Formula> boxD = FormulaExtraction::getBoxD(bcbd);
        

        newSequent.clear();
        newSequent.addTheta(theta);
        newSequent.addGamma(gamma_bcbd);
        newSequent.boxL_.insert(boxC);
        newSequent.addGamma_cl(gamma_cl);
        newSequent.addDelta(delta);
        newSequent.addDelta_cl(delta_cl);
        if (!Search::search(newSequent)){return false;};

        newSequent.clear();
        newSequent.addTheta(theta);
        newSequent.addGamma(gamma_bcbd);
        newSequent.boxL_.insert(boxD);
        newSequent.addGamma_cl(gamma_cl);
        newSequent.addDelta(delta);
        newSequent.addDelta_cl(delta_cl);
        if (!Search::search(newSequent)){return false;};

        newSequent.clear();
        newSequent.boxL_.insert(theta.blackbox.begin(),newSequent.blackbox_.end());
        newSequent.addGamma(gamma_bcbd);
        newSequent.boxL_.insert(boxD);
        if (Search::search(newSequent)){return true;};
    }

    newSequent.clear(); 
    newSequent.diaL_.insert(theta.blackdia.begin(),theta.blackdia.end());
    newSequent.boxL_.insert(gamma_box.begin(),gamma_box.end());
    newSequent.addGamma_cl(gamma_cl);
    newSequent.addDelta_cl(delta_cl);
    if (Search::search(newSequent)){return true;};

    for (shared_ptr<Formula> bg : delta){
        formula_set delta_bg = delta;
        delta_bg.erase(bg);
        shared_ptr<Formula> g = dynamic_cast<Box *>(bg.get())->getSubformula();

        newSequent.clear(); 
        newSequent.addDelta(delta);
        newSequent.blackdia_.insert(Diamond::create(Not::create(g)));
        newSequent.addGamma(gamma);
        newSequent.addGamma_cl(gamma_cl);
        newSequent.addDelta(delta_bg);
        newSequent.addDelta_cl(delta_cl);
        if (!Search::search(newSequent)){return false;};

        newSequent.clear(); 
        newSequent.boxL_.insert(theta.blackbox.begin(),theta.blackbox.end());
        newSequent.addGamma(gamma);
        newSequent.classicR_.insert(g);
        if (Search::search(newSequent)){return true;};

    }

    for (shared_ptr<Formula> bedf : gamma.boxdiaL){
        Gamma gamma_bedf = gamma;
        gamma_bedf.boxdiaL.erase(bedf);
        shared_ptr<Formula> be = FormulaExtraction::getBoxE(bedf);
        shared_ptr<Formula> df = FormulaExtraction::getDiaF(bedf);
        shared_ptr<Formula> f = dynamic_cast<Diamond *>(df.get())->getSubformula();

        newSequent.clear(); 
        newSequent.addTheta(theta);
        newSequent.addGamma(gamma_bedf);
        newSequent.boxL_.insert(be);
        newSequent.addGamma_cl(gamma_cl);
        newSequent.addDelta(delta);
        newSequent.addDelta_cl(delta_cl);
        if (!Search::search(newSequent)){return false;};

        newSequent.clear(); 
        newSequent.addTheta(theta);
        newSequent.addGamma(gamma_bedf);
        newSequent.blackbox_.insert(be);
        newSequent.blackdia_.insert(df);
        newSequent.addGamma_cl(gamma_cl);
        newSequent.addDelta(delta);
        newSequent.addDelta_cl(delta_cl);
        if (!Search::search(newSequent)){return false;};

        newSequent.clear(); 
        newSequent.boxL_.insert(theta.blackbox.begin(),theta.blackbox.end());
        newSequent.addGamma(gamma_bedf);
        newSequent.blackbox_.insert(be);
        newSequent.classicL_.insert(f);
        if (Search::search(newSequent)){return true;};

    }


    return false;
}

bool Search::searchRegular(Sequent sequent){
    Gamma gamma = sequent.getGamma();
    formula_set gamma_cl = sequent.classicL_;
    formula_set delta = sequent.boxR_;
    formula_set delta_cl = sequent.classicR_;
    formula_set gamma_box = sequent.getGamma_box();

    if (gamma.boxLbox.empty() && gamma.boxLdia.empty()){
        Sequent clusterSequent = sequent.copy();
        clusterSequent.toCluster();
        return Search::searchCluster(sequent);
    }

    Sequent newSequent;

    for (shared_ptr<Formula> bcbd : sequent.boxLbox_){
        Gamma gamma_bcbd = gamma;
        gamma_bcbd.boxLbox.erase(bcbd);
        shared_ptr<Formula> boxC = FormulaExtraction::getBoxC(bcbd);
        shared_ptr<Formula> boxD = FormulaExtraction::getBoxD(bcbd);
        

        newSequent.clear();
        newSequent.addGamma(gamma_bcbd);
        newSequent.boxL_.insert(boxC);
        newSequent.addGamma_cl(gamma_cl);
        newSequent.addDelta(delta);
        newSequent.addDelta_cl(delta_cl);
        if (!Search::search(newSequent)){return false;};

        newSequent.clear();
        newSequent.addGamma(gamma_bcbd);
        newSequent.boxL_.insert(boxD);
        newSequent.addGamma_cl(gamma_cl);
        newSequent.addDelta(delta);
        newSequent.addDelta_cl(delta_cl);
        if (!Search::search(newSequent)){return false;};

        newSequent.clear();  
        newSequent.addGamma(gamma_bcbd);
        newSequent.boxL_.insert(boxD);
        if (Search::search(newSequent)){return true;};
    }

    newSequent.clear(); 
    newSequent.boxL_.insert(gamma_box.begin(),gamma_box.end());
    newSequent.addGamma_cl(gamma_cl);
    newSequent.addDelta_cl(delta_cl);
    if (Search::searchCluster(newSequent)){return true;};

    for (shared_ptr<Formula> bg : delta){

        shared_ptr<Formula> g = dynamic_cast<Box *>(bg.get())->getSubformula();

        newSequent.clear(); 
        newSequent.addGamma(gamma);
        newSequent.classicR_.insert(g);
        if (Search::search(newSequent)){return true;};

    }

    for (shared_ptr<Formula> bedf : gamma.boxdiaL){
        Gamma gamma_bedf = gamma;
        gamma_bedf.boxdiaL.erase(bedf);
        shared_ptr<Formula> be = FormulaExtraction::getBoxE(bedf);
        shared_ptr<Formula> df = FormulaExtraction::getDiaF(bedf);
        shared_ptr<Formula> f = dynamic_cast<Diamond *>(df.get())->getSubformula();

        newSequent.clear(); 
        newSequent.addGamma(gamma_bedf);
        newSequent.boxL_.insert(be);
        newSequent.addGamma_cl(gamma_cl);
        newSequent.addDelta(delta);
        newSequent.addDelta_cl(delta_cl);
        if (!Search::search(newSequent)){return false;};

        newSequent.clear(); 
        newSequent.addGamma(gamma_bedf);
        newSequent.blackbox_.insert(be);
        newSequent.classicL_.insert(f);
        if (Search::search(newSequent)){return true;};

    }


    return false;
}




bool Search::searchCluster(Sequent sequent){
    if (!sequent.blackbox_.empty() || !sequent.blackdia_.empty() || 
        !sequent.boxLbox_.empty() || !sequent.boxLdia_.empty() || !sequent.boxdiaL_.empty() ){
        throw invalid_argument("It's not cluster sequent " + sequent.toString());
    }
    formula_set ZSet;

    formula_set notASet;
    for (shared_ptr<Formula> boxA : sequent.boxL_){
        shared_ptr<Formula> A = dynamic_cast<Box *>(boxA.get())->getSubformula();
        notASet.insert(Not::create(A));
    }

    formula_set Z0;
    Z0.insert(notASet.begin(),notASet.end());
    for (shared_ptr<Formula> G : sequent.classicL_){
        Z0.insert(Not::create(G));
    }
    Z0.insert(sequent.classicR_.begin(),sequent.classicR_.end());
    ZSet.insert(Or::create(Z0));

    for (shared_ptr<Formula> diaB : sequent.diaL_){
        formula_set Zi;
        Zi.insert(notASet.begin(),notASet.end());
        shared_ptr<Formula> B = dynamic_cast<Diamond *>(diaB.get())->getSubformula();
        Zi.insert(Not::create(B));
        ZSet.insert(Or::create(Zi));
    }

    for (shared_ptr<Formula> Z : ZSet){
        MinisatProver prover(Not::create(Z));
        bool satisfible = prover.solve();
        bool valid = !satisfible;
        if (valid){
            return true;
        }      
    }
    return false;
}