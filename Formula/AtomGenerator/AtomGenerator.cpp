#include "AtomGenerator.h"


shared_ptr<Formula> static generate(){
        const string prefix = "$x";
        static int i = 0;

        return Atom::create(prefix+to_string(i++));
};