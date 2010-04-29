#pragma once

#include "PropertyObject.h"
#include "Factory.h"

class CDelegatingPropertyInformation : public CPropertyInformation
{
    public:
        DECLARE_FACTORY2( CDelegatingPropertyInformation, CPropertyInformation*, CPropertyInformation* );

        virtual HRESULT GetProperty( const WCHAR* pPropertyName, CProperty** ppProperty );
        virtual HRESULT GetContentProperty( CProperty** ppProperty );

    protected:
        CDelegatingPropertyInformation();
        virtual ~CDelegatingPropertyInformation();

        HRESULT Initialize( CPropertyInformation* pRoot, CPropertyInformation* pBase );

        CPropertyInformation* m_Root;
        CPropertyInformation* m_Base;
};