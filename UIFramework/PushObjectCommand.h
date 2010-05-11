#pragma once

#include "ParserCommand.h"
#include "Factory.h"

//
// Pushes a value on to the object stack.
//
class CPushObjectCommand : public CParserCommand
{
    public:
        DECLARE_FACTORY1( CPushObjectCommand, CObjectWithType* );

        virtual HRESULT Execute( CParserCommandContext& Context );

    protected:
        CPushObjectCommand();
        virtual ~CPushObjectCommand();

        HRESULT Initialize( CObjectWithType* pValue );

        CObjectWithType* m_Value;
};