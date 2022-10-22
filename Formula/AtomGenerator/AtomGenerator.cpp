#include "AtomGenerator.h"


shared_ptr<Formula>  AtomGenerator::generate(){
        const string prefix = "$x";
        static int i = 0;
        return Atom::create(prefix+to_string(i++));
};