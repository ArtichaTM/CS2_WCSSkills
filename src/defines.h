#ifndef WCSSKILLS_DEFINES

#include <limits>

#ifndef TRAIT_INDEX_TYPE
#define TRAIT_INDEX_TYPE unsigned char
#endif
#ifndef TRAIT_INDEX_MAX
#define TRAIT_INDEX_MAX std::numeric_limits<TRAIT_INDEX_TYPE>::max()
#endif

#ifndef MAX_TRAITS_IN_TRSET
#define MAX_TRAITS_IN_TRSET 10
#endif

#endif