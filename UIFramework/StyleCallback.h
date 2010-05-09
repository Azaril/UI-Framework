#pragma once

#include "PropertyObject.h"

struct IStyleCallback
{
    virtual HRESULT SetStyleValue( CProperty* pProperty, CObjectWithType* pValue ) = 0;
};