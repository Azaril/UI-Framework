#pragma once

#include "Types.h"

#define DECLARE_VISITABLE( ) \
template< typename T > \
HRESULT Visit( T& Visitor ) \
{ \
    return Visitor.Visit( *this ); \
}
