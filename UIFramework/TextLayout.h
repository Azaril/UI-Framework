#pragma once

#include "TextLayoutMetrics.h"

class CTextLayout : public CRefCountedObject
{
    public:
        virtual HRESULT SetMaxSize( const SizeF& Size ) = 0;
        virtual HRESULT GetMetrics( CTextLayoutMetrics** ppMetrics ) = 0;
};
