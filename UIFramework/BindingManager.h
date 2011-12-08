#pragma once

#include "RefCounted.h"
#include "Factory.h"
#include "BindingBase.h"
#include "BindingContext.h"

typedef __checkReturn HRESULT (*SetValueFunc)( 
    __in CPropertyObject* pTarget,
    __in CProperty* pTargetProperty, 
    __in CObjectWithType* pValue 
    );

class CDefaultBindingContext : public CBindingContext
{
    public:
        DECLARE_FACTORY( CDefaultBindingContext );

        __checkReturn HRESULT SetPropertyBinding(
            __in CProperty* pProperty, 
            __in CBindingBase* pBinding, 
            __in SetValueFunc SetFunc, 
            events::signals::connection ChangeConnection 
            );

    protected:
        CDefaultBindingContext(
            );

        virtual ~CDefaultBindingContext(
            );

        __checkReturn HRESULT Initialize(
            );

        class CPropertyBinding
        {
            public:
                CPropertyBinding(
                    __in CProperty* pProperty, 
                    __in CBindingBase* pBinding, 
                    __in SetValueFunc SetFunc, 
                    events::signals::connection ChangeConnection
                    ) 
                    : m_Property(pProperty)
                    , m_Binding(pBinding)
                    , m_ChangeConnection(ChangeConnection)
                    , m_SetFunc(SetFunc)
                {
                    AddRefObject(m_Property);
                    AddRefObject(m_Binding);
                }

                CPropertyBinding(
                    const CPropertyBinding& Other
                    ) 
                    : m_Property(Other.m_Property)
                    , m_Binding(Other.m_Binding)
                    , m_ChangeConnection(Other.m_ChangeConnection)
                    , m_SetFunc(Other.m_SetFunc)
                {
                    AddRefObject(m_Property);
                    AddRefObject(m_Binding);
                }

                ~CPropertyBinding(
                    )
                {
                    ReleaseObject(m_Property);
                    ReleaseObject(m_Binding);
                }

                CPropertyBinding& operator=(
                    const CPropertyBinding& Other
                    )
                {
                    ReleaseObject(m_Property);
                    ReleaseObject(m_Binding);

                    m_Property = Other.m_Property;
                    m_Binding = Other.m_Binding;
                    m_ChangeConnection = Other.m_ChangeConnection;
                    m_SetFunc = Other.m_SetFunc;

                    AddRefObject(m_Property);
                    AddRefObject(m_Binding);

                    return *this;
                }

                __out CProperty* GetProperty(
                    )
                {
                    return m_Property;
                }

                __out CBindingBase* GetBinding(
                    )
                {
                    return m_Binding;
                }

                __out SetValueFunc GetSetFunction(
                    )
                {
                    return m_SetFunc;
                }

                void Clear(
                    )
                {
                    ReleaseObject(m_Property);
                    ReleaseObject(m_Binding);

                    m_ChangeConnection.disconnect();
                }

            protected:
                CProperty* m_Property;
                CBindingBase* m_Binding;
                SetValueFunc m_SetFunc;
                events::signals::connection m_ChangeConnection;
        };

        vector< CPropertyBinding > m_Bindings;
};

class CBindingManager : public CRefCountedObject
{
    public:
        DECLARE_FACTORY( CBindingManager );

        __checkReturn HRESULT SetBinding(
            __in CPropertyObject* pTarget, 
            __in CProperty* pTargetProperty, 
            __in CBindingBase* pBinding, 
            __in SetValueFunc SetFunc 
            );

    protected:
        CBindingManager(
            );

        virtual ~CBindingManager(
            );

        __checkReturn HRESULT Initialize(
            );

        __checkReturn HRESULT GetBindingContext( 
            __in CPropertyObject* pTarget,
            __deref_out CDefaultBindingContext** ppContext 
            );

        static void OnBindingInvalidated( 
            __in CBindingBase* pBinding, 
            __in SetValueFunc SetFunc 
            );

        static __checkReturn HRESULT SetValueFromBinding(
            __in CBindingBase* pBinding,
            __in SetValueFunc SetFunc 
            );
};

