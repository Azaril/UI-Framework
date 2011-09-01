#pragma once

#include "ParserCommand.h"
#include "Factory.h"

//
// If the object on the stack is a markup extension, it evaluates the markup extension
// and replaces the object on the top of the stack with the result.
//
class CEvaluateMarkupExtensionCommand : public CParserCommand
{
    public:
        DECLARE_FACTORY( CEvaluateMarkupExtensionCommand );

        __override virtual HRESULT __checkReturn Execute( 
			CParserCommandContext& Context 
			);

    protected:
        CEvaluateMarkupExtensionCommand(
			);

        virtual ~CEvaluateMarkupExtensionCommand(
			);

        __checkReturn HRESULT Initialize(
			);
};
