#pragma once

#include "InputEventArgs.h"
#include "BasicTypes.h"

class CTextEventArgs : public CInputEventArgs
{
    public:
        DECLARE_FACTORY3( CTextEventArgs, CRoutedEvent*, const WCHAR*, UINT32 );

        DECLARE_TYPE_WITH_BASE( TypeIndex::TextEventArgs, CInputEventArgs );

        __out const WCHAR* GetText(
            );

        UINT32 GetTextLength(
            );

    protected:
        CTextEventArgs(
            );

        virtual ~CTextEventArgs(
            );

        __checkReturn HRESULT Initialize( 
            __in CRoutedEvent* pRoutedEvent, 
            __in_ecount(TextLength) const WCHAR* pText, 
            UINT32 TextLength 
            );

        CStringValue* m_Text;
};

template< >
struct ObjectTypeTraits< CTextEventArgs >
{
    static const TypeIndex::Value Type = TypeIndex::TextEventArgs;
};