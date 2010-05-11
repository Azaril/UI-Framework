#pragma once

#include "ParserCommandContext.h"

class CParserCommand : public CRefCountedObject
{
    public:
        virtual HRESULT Execute( CParserCommandContext& Context ) = 0;
};