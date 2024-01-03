#ifndef WCSSKILLS_TRAITS_HPP
#define WCSSKILLS_TRAITS_HPP

#include <unordered_set>
#include "../defines.h"
#include "../includes/json.hpp"

namespace traits {
    typedef unsigned int Trait;
    namespace {
        typedef Trait inner_type;
    }

    typedef std::unordered_set<Trait> tr_set;
    typedef std::unordered_set<tr_set> tr_set_set;
    typedef std::vector<Trait> tr_vector;

    tr_set make(nlohmann::json const&);
    tr_set make(std::unordered_set<Trait> const&);

} // namespace traits

template<> struct std::hash<traits::tr_set> {
    std::size_t operator()(traits::tr_set const& s) const noexcept;
};


#endif //WCSSKILLS_TRAITS_HPP
