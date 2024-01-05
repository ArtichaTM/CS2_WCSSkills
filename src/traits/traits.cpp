#include <string>
#include <unordered_set>
#include "traits.hpp"

using std::size_t;
using std::hash;
using traits::Trait;
using traits::tr_set;
using nlohmann::json;
using std::unordered_set;

namespace traits {
    tr_set make(json const& info) {
        tr_set values;
        for (auto& value : info) {
            values.insert(value. template get<unsigned int>());
        }
        return values;
    }

    tr_set make(unordered_set<inner_type> const& info) {
        tr_set values;
        for (inner_type const& value : info) {
            values.insert(value);
        }
        return values;

    }
}


size_t hash<tr_set>::operator()(const tr_set &s) const noexcept {
    size_t hashcode = 0;
    for (Trait trait: s) {
        hashcode ^= trait;
    }
    return hashcode;
}
