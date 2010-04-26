#pragma once

#include "TextFormat.h"
#include "Factory.h"
#include <dwrite.h>

class CDirectWriteTextFormat : public CTextFormat
{
    public:
        DECLARE_FACTORY1( CDirectWriteTextFormat, IDWriteTextFormat* );

        IDWriteTextFormat* GetDirectWriteTextFormat();

    protected:
        CDirectWriteTextFormat();
        virtual ~CDirectWriteTextFormat();

        HRESULT Initialize( IDWriteTextFormat* pLayout );

        IDWriteTextFormat* m_Format;
};
