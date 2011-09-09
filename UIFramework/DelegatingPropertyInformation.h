#pragma once

#include "PropertyObject.h"
#include "Factory.h"

class CDelegatingPropertyInformation : public CPropertyInformation
{
    public:
        DECLARE_FACTORY2( CDelegatingPropertyInformation, CPropertyInformation*, CPropertyInformation* );

        __override virtual __checkReturn HRESULT GetProperty( 
            __in_z const WCHAR* pPropertyName, 
            __deref_out_opt CProperty** ppProperty 
            );

        __override virtual __checkReturn HRESULT GetContentProperty( 
            __deref_out_opt CProperty** ppProperty 
            );

    protected:
        CDelegatingPropertyInformation(
            );

        virtual ~CDelegatingPropertyInformation(
            );

        __checkReturn HRESULT Initialize(
            __in CPropertyInformation* pRoot,
            __in CPropertyInformation* pBase 
            );

        CPropertyInformation* m_Root;
        CPropertyInformation* m_Base;
};