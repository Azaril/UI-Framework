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

        virtual HRESULT Execute( CParserCommandContext& Context );

    protected:
        CCreateObjectCommand();
        virtual ~CCreateObjectCommand();

        HRESULT Initialize( CResolvedClass* pResolvedClass );

        CResolvedClass* m_ResolvedClass;
};