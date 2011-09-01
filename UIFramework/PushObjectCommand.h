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

        __override virtual HRESULT Execute( 
			CParserCommandContext& Context 
			);

    protected:
        CPushObjectCommand(
			);

        virtual ~CPushObjectCommand(
			);

        __checkReturn HRESULT Initialize(
			__in CObjectWithType* pValue 
			);

        CObjectWithType* m_Value;
};