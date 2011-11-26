#pragma once

#ifdef WIN32

#include <boost/detail/endian.hpp>

#ifdef BOOST_LITTLE_ENDIAN
#define PLATFORM_LITTLE_ENDIAN
#elif BOOST_BIG_ENDIAN
#define PLATFORM_BIG_ENDIAN
#endif

#else

#ifdef __LITTLE_ENDIAN__
#define PLATFORM_LITTLE_ENDIAN
#elif __BIG_ENDIAN__
#define PLATFORM_BIG_ENDIAN
#else
#error Unable to calculate platform endianness.
#endif

#endif