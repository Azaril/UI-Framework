#pragma once

#include "ParserCommand.h"
#include "Factory.h"

//
// Pops the top object from the stack as the key, and the next item as the value and adds it to the dictionary 
// that is currently set in the property on the object on the top of the stack.
//
class CAddToDictionaryCommand : public CParserCommand
{
    public:
        DECLARE_FACTORY1( CAddToDictionaryCommand, CProperty* );

        virtual HRESULT Execute( CParserCommandContext& Context );

    protected:
        CAddToDictionaryCommand();
        virtual ~CAddToDictionaryCommand();

        HRESULT Initialize( CProperty* pProperty );

        CProperty* m_Property;
};