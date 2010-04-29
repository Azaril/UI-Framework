#pragma once

#include "RefCounted.h"
#include "Factory.h"
#include "UIElement.h"
#include "ClassResolver.h"
#include "TypeConverter.h"

class CParser : public CRefCountedObject
{
    public:
        DECLARE_FACTORY2( CParser, CClassResolver*, CTypeConverter* );

        HRESULT LoadFromFile( const WCHAR* pPath, CUIElement** pRootElement );

    protected:
        CParser();
        virtual ~CParser();

        HRESULT Initialize( CClassResolver* pResolver, CTypeConverter* pTypeConverter );

        CClassResolver* m_ClassResolver;
        CTypeConverter* m_TypeConverter;
};
