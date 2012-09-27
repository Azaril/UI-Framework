#pragma once

#if defined(__LITTLE_ENDIAN__)

#define PLATFORM_LITTLE_ENDIAN

#elif defined(__BIG_ENDIAN__)

#define PLATFORM_BIG_ENDIAN

#else

#include <boost/detail/endian.hpp>

#if defined(BOOST_LITTLE_ENDIAN)
#define PLATFORM_LITTLE_ENDIAN
#elif defined(BOOST_BIG_ENDIAN)
#define PLATFORM_BIG_ENDIAN
#else
#error Unable to calculate platform endianness.
#endif

#endif