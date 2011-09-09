#pragma once

#include "PropertyObject.h"
#include "Factory.h"
#include "Command.h"

class CDelegatingCommandInformation : public CCommandInformation
{
    public:
        DECLARE_FACTORY2( CDelegatingCommandInformation, CCommandInformation*, CCommandInformation* );

        __override virtual __checkReturn HRESULT GetCommand(
            __in_z const WCHAR* pPropertyName, 
            __deref_out_opt CCommand** ppCommand 
            );

    protected:
        CDelegatingCommandInformation(
            );

        virtual ~CDelegatingCommandInformation(
            );

        __checkReturn HRESULT Initialize( 
            __in CCommandInformation* pRoot,
            __in CCommandInformation* pBase 
            );

        CCommandInformation* m_Root;
        CCommandInformation* m_Base;
};
