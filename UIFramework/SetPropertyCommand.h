#pragma once

#include "ParserCommand.h"
#include "Factory.h"

//
// Pops the top object from the stack and sets it to the property on the object
// that is now on the top of the stack.
//
class CSetPropertyCommand : public CParserCommand
{
    public:
        DECLARE_FACTORY1( CSetPropertyCommand, CProperty* );

        __override virtual __checkReturn HRESULT Execute( 
			CParserCommandContext& Context 
			);

    protected:
        CSetPropertyCommand(
			);

        virtual ~CSetPropertyCommand(
			);

        HRESULT Initialize( 
			__in CProperty* pProperty 
			);

        CProperty* m_Property;
};