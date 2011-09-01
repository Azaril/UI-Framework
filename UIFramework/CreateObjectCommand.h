#pragma once

#include "ParserCommand.h"
#include "ClassResolver.h"
#include "Factory.h"

//
// Creates a new object and pushes it on to the object stack.
//
class CCreateObjectCommand : public CParserCommand
{
    public:
        DECLARE_FACTORY1( CCreateObjectCommand, CResolvedClass* );

        __override virtual __checkReturn HRESULT Execute(
			CParserCommandContext& Context 
			);

    protected:
        CCreateObjectCommand(
			);

        virtual ~CCreateObjectCommand(
			);

        __checkReturn HRESULT Initialize( 
			__in CResolvedClass* pResolvedClass 
			);

        CResolvedClass* m_ResolvedClass;
};