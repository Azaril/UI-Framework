#pragma once

#include "PropertyObject.h"
#include "TextLayoutMetrics.h"

class CTextLayout : public CRefCountedObjectBase< CObjectWithType >
{
    public:
        DECLARE_TYPE_WITH_BASE( TypeIndex::TextLayout, CObjectWithType );

        virtual HRESULT SetMaxSize( const SizeF& Size ) = 0;
        virtual HRESULT GetMetrics( CTextLayoutMetrics** ppMetrics ) = 0;
};
