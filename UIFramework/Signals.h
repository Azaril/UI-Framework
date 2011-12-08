#pragma once

#define BOOST_ALL_NO_LIB

#pragma warning(push)
#pragma warning(disable: 4512)

#include <boost/function.hpp>
#include <boost/bind/bind.hpp>
#include <boost/signals.hpp>

#pragma warning(pop)

namespace events = boost;