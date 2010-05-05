#pragma once

#include "Dictionary.h"

class CResourceDictionary : public CDictionary< CObjectWithType >
{
    public:
        DECLARE_FACTORY( CResourceDictionary );

        DECLARE_TYPE_WITH_BASE( TypeIndex::ResourceDictionary, CDictionary< CObjectWithType > );
};