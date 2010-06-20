#pragma once

#include "PropertyObject.h"
#include "Factory.h"
#include "Command.h"

class CDelegatingCommandInformation : public CCommandInformation
{
    public:
        DECLARE_FACTORY2( CDelegatingCommandInformation, CCommandInformation*, CCommandInformation* );

        virtual HRESULT GetCommand( const WCHAR* pPropertyName, CCommand** ppCommand );

    protected:
        CDelegatingCommandInformation();
        virtual ~CDelegatingCommandInformation();

        HRESULT Initialize( CCommandInformation* pRoot, CCommandInformation* pBase );

        CCommandInformation* m_Root;
        CCommandInformation* m_Base;
};
