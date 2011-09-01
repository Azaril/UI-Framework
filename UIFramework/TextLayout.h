#pragma once

#include "PropertyObject.h"
#include "TextLayoutMetrics.h"

class CTextLayout : public CRefCountedObjectBase< CObjectWithType >
{
    public:
        DECLARE_TYPE_WITH_BASE( TypeIndex::TextLayout, CObjectWithType );

        virtual __checkReturn HRESULT SetMaxSize( 
			const SizeF& Size 
			) = 0;

        virtual __checkReturn HRESULT GetMetrics( 
			__deref_out CTextLayoutMetrics** ppMetrics
			) = 0;
};
