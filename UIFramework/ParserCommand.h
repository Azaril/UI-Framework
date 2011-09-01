#pragma once

#include "ParserCommandContext.h"

class CParserCommand : public CRefCountedObject
{
    public:
        virtual __checkReturn HRESULT Execute( 
			CParserCommandContext& Context 
			) = 0;
};