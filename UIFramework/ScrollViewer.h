#pragma once

#include "ContentControl.h"

class CScrollViewer : public CContentControl
{
    public:
        DECLARE_FACTORY1( CScrollViewer, CProviders* );

        DECLARE_TYPE_WITH_BASE( TypeIndex::ScrollViewer, CContentControl );

        virtual HRESULT HitTest( Point2F LocalPoint, CHitTestResult** ppHitTestResult );

    protected:
        CScrollViewer();
        virtual ~CScrollViewer();

        HRESULT Initialize( CProviders* pProviders );
};

template< >
struct ObjectTypeTraits< CScrollViewer >
{
    static const TypeIndex::Value Type = TypeIndex::ScrollViewer;
};