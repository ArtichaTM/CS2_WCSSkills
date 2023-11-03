#ifndef WCSSKILLS_FUNCTIONS_ALL
#define WCSSKILLS_FUNCTIONS_ALL

#include "../events/event.hpp"

namespace functions {
    class Functions {
        functions_map all_functions;
        static Functions* instance;
        Functions();
        ~Functions() = default;
    public:
        static void init();
        static void close();
        functions_map* operator->();
        functions_map& operator*();
        static Functions* get();
    };
} // functions

#endif //WCSSKILLS_FUNCTIONS_ALL
