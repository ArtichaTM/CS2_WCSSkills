#ifndef WCSSKILLS_DEFINES

#include <limits>

#ifndef TRAIT_INDEX_TYPE
#define TRAIT_INDEX_TYPE unsigned char
#endif
#ifndef TRAIT_INDEX_MAX
#define TRAIT_INDEX_MAX std::numeric_limits<TRAIT_INDEX_TYPE>::max()
#endif

#endif