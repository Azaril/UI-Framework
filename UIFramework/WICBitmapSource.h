#pragma once

#include "BitmapSource.h"
#include "Factory.h"

#include <Wincodec.h>

class CWICBitmapSource : public CBitmapSource
{
    public:
        DECLARE_FACTORY1( CWICBitmapSource, IWICBitmapSource* );

        IWICBitmapSource* GetWICBitmapSource();

        virtual HRESULT GetSize( SizeU* pSize );

    protected:
        CWICBitmapSource();
        virtual ~CWICBitmapSource();

        HRESULT Initialize( IWICBitmapSource* pSource );

        IWICBitmapSource* m_Source;
};
