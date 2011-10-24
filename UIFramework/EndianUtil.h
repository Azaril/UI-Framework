#pragma once

#include <boost/detail/endian.hpp>

#ifdef BOOST_LITTLE_ENDIAN
#define PLATFORM_LITTLE_ENDIAN
#elif BOOST_BIG_ENDIAN
#define PLATFORM_BIG_ENDIAN
#endif