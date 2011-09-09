#pragma once

#include "PropertyObject.h"

struct IStyleCallback
{
    virtual __checkReturn HRESULT SetStyleValue( 
        __in CProperty* pProperty, 
        __in CObjectWithType* pValue
        ) = 0;
};