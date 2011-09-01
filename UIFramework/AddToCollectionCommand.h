#pragma once

#include "ParserCommand.h"
#include "Factory.h"

//
// Pops the top object from the stack and adds it to the collection that is currently
// set in the property on the object on the top of the stack.
//
class CAddToCollectionCommand : public CParserCommand
{
    public:
        DECLARE_FACTORY1( CAddToCollectionCommand, CProperty* );

        __override virtual __checkReturn HRESULT Execute(
			CParserCommandContext& Context 
			);

    protected:
        CAddToCollectionCommand(
			);

        virtual ~CAddToCollectionCommand(
			);

        __checkReturn HRESULT Initialize(
			__in CProperty* pProperty 
			);

        CProperty* m_Property;
};