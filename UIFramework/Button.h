#pragma once

#include "ButtonBase.h"

class CButton : public CButtonBase
{
    public:
        DECLARE_FACTORY1( CButton, CProviders* );

        DECLARE_TYPE_WITH_BASE( TypeIndex::Button, CButtonBase );

        virtual HRESULT HitTest( Point2F LocalPoint, CHitTestResult** ppHitTestResult );

    protected:
        CButton();
        virtual ~CButton();

        HRESULT Initialize( CProviders* pProviders );
};

template< >
struct ObjectTypeTraits< CButton >
{
    static const TypeIndex::Value Type = TypeIndex::Button;
};