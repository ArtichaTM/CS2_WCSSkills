#include <string>
#include <unordered_set>
#include "traits.hpp"

using namespace traits;
using namespace nlohmann;
using std::unordered_set;

namespace traits {
    tr_set make(nlohmann::json const& info) {
        tr_set values;
        for (auto& value : info) {
            values.insert(value. template get<unsigned int>());
        }
        return values;
    }

    tr_set make(std::unordered_set<inner_type> const& info) {
        tr_set values;
        for (inner_type const& value : info) {
            values.insert(value);
        }
        return values;

    }
}


std::size_t std::hash<traits::tr_set>::operator()(const traits::tr_set &s) const noexcept {
    std::size_t hashcode = 0;
    for (traits::Trait trait: s) {
        hashcode ^= trait;
    }
    return hashcode;
}
