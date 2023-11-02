#ifndef WCSSKILLS_TRAITS_HPP
#define WCSSKILLS_TRAITS_HPP

#include <unordered_set>
#include "../includes/json.hpp"


namespace traits {

    typedef unsigned int inner_type;
    
    class Trait {
    public:
        const inner_type id;
        
        Trait(inner_type);
        Trait(std::initializer_list<inner_type>&);
        bool operator==(inner_type) const;
        bool operator==(Trait) const;
    };
    
    typedef std::unordered_set<traits::Trait> tr_set;
    typedef std::unordered_set<tr_set> tr_set_set;
    
    inline tr_set make(nlohmann::json const&);
    inline tr_set make(std::unordered_set<inner_type> const&);
    inline tr_set make(std::unordered_set<inner_type>&&);

} // namespace traits

template<> struct std::hash<traits::Trait> {
    std::size_t operator()(traits::Trait const& s) const noexcept;
};

template<> struct std::hash<traits::tr_set> {
    std::size_t operator()(traits::tr_set const& s) const noexcept;
};


#endif //WCSSKILLS_TRAITS_HPP
