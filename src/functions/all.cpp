#include "all.hpp"

#include "buffs.hpp"
#include "debuffs.hpp"
#include "instant.hpp"

namespace functions {
    Functions* Functions::instance = nullptr;
    
    void Functions::init() {
        assert(!instance);
        instance = new Functions();
    }

    void Functions::close() {
        assert(instance);
        delete instance;
        instance = nullptr;
    }
    
    functions_map* Functions::operator->() {
        return &all_functions;
    }
    
    functions_map& Functions::operator*() {
        return all_functions;
    }

    Functions::Functions() {
        all_functions = functions_map();
        buffs  ::fillMap(&all_functions);
        debuffs::fillMap(&all_functions);
        instant::fillMap(&all_functions);
        assert(!all_functions.empty());
    }

    Functions* Functions::get() {
#ifdef DEBUG
        if (!instance) {
            throw CustomException("There's no functions manager instance."
                "May be Functions::init() function call missing?");
        }
#endif
        return instance;
    }
} // functions
