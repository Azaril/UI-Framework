#pragma once

#include "Control.h"

class CContentControl : public CControl
{
    public:
        DECLARE_TYPE_WITH_BASE( TypeIndex::ContentControl, CControl );

        static HRESULT CreatePropertyInformation( CPropertyInformation** ppInformation );

        //
        // Properties
        //
        static CStaticProperty ContentProperty;

    protected:
        CContentControl();
        virtual ~CContentControl();

        virtual HRESULT GetLayeredValue( CProperty* pProperty, CLayeredValue** ppLayeredValue );

        HRESULT GetEffectiveContent( CObjectWithType** ppContent );

        //
        // Property Change Handlers
        //
        DECLARE_INSTANCE_CHANGE_CALLBACK( OnContentChanged );

        HRESULT OnContentChanged( CObjectWithType* pOldValue, CObjectWithType* pNewValue );

        CTypedLayeredValue< CContentControl, CObjectWithType > m_Content;
};

template< >
struct ObjectTypeTraits< CContentControl >
{
    static const TypeIndex::Value Type = TypeIndex::ContentControl;
};