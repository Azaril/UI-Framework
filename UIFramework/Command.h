#pragma once

#include "RefCounted.h"
#include "Factory.h"
#include "PropertyObject.h"

class CUIElement;

class CCommand : public CObjectWithType
{
    public:
        DECLARE_TYPE_WITH_BASE( TypeIndex::Command, CObjectWithType );

        virtual const WCHAR* GetName() = 0;
        virtual HRESULT Execute( CUIElement* pSender ) = 0;
};

template< >
struct ObjectTypeTraits< CCommand >
{
    static const TypeIndex::Value Type = TypeIndex::Command;
};