#pragma once

#if defined(FRAMEWORK_DWRITE)

#include "TextFormat.h"
#include "Factory.h"
#include <dwrite.h>

class CDirectWriteTextFormat : public CTextFormat
{
    public:
        DECLARE_FACTORY1( CDirectWriteTextFormat, IDWriteTextFormat* );

        __out IDWriteTextFormat* GetDirectWriteTextFormat(
			);

    protected:
        CDirectWriteTextFormat(
			);

        virtual ~CDirectWriteTextFormat(
			);

        __checkReturn HRESULT Initialize(
			__in IDWriteTextFormat* pLayout 
			);

        IDWriteTextFormat* m_Format;
};

#endif