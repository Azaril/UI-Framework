#pragma once

#include "RefCounted.h"
#include "Factory.h"
#include "BindingBase.h"
#include "BindingContext.h"

typedef HRESULT (*SetValueFunc)( CPropertyObject* pTarget, CProperty* pTargetProperty, CObjectWithType* pValue );

class CDefaultBindingContext : public CRefCountedObjectBase< CBindingContext >
{
    public:
        DECLARE_FACTORY( CDefaultBindingContext );

        HRESULT SetPropertyBinding( CProperty* pProperty, CBindingBase* pBinding, SetValueFunc SetFunc, events::signals::connection ChangeConnection );

    protected:
        CDefaultBindingContext();
        virtual ~CDefaultBindingContext();

        HRESULT Initialize();

        class CPropertyBinding
        {
            public:
                CPropertyBinding(CProperty* pProperty, CBindingBase* pBinding, SetValueFunc SetFunc, events::signals::connection ChangeConnection) : m_Property(pProperty),
                                                                                                                                                     m_Binding(pBinding),
                                                                                                                                                     m_ChangeConnection(ChangeConnection),
                                                                                                                                                     m_SetFunc(SetFunc)
                {
                    AddRefObject(m_Property);
                    AddRefObject(m_Binding);
                }

                CPropertyBinding(const CPropertyBinding& Other) : m_Property(Other.m_Property),
                                                                  m_Binding(Other.m_Binding),
                                                                  m_ChangeConnection(Other.m_ChangeConnection),
                                                                  m_SetFunc(Other.m_SetFunc)
                {
                    AddRefObject(m_Property);
                    AddRefObject(m_Binding);
                }

                ~CPropertyBinding()
                {
                    ReleaseObject(m_Property);
                    ReleaseObject(m_Binding);
                }

                CPropertyBinding& operator=(const CPropertyBinding& Other)
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

                CProperty* GetProperty()
                {
                    return m_Property;
                }

                CBindingBase* GetBinding()
                {
                    return m_Binding;
                }

                void Clear()
                {
                    ReleaseObject(m_Property);
                    ReleaseObject(m_Binding);

                    m_ChangeConnection.disconnect();
                }

                SetValueFunc GetSetFunction()
                {
                    return m_SetFunc;
                }

            protected:
                CProperty* m_Property;
                CBindingBase* m_Binding;
                SetValueFunc m_SetFunc;
                events::signals::connection m_ChangeConnection;
        };

        std::vector< CPropertyBinding > m_Bindings;
};

class CBindingManager : public CRefCountedObject
{
    public:
        DECLARE_FACTORY( CBindingManager );

        HRESULT SetBinding( CPropertyObject* pTarget, CProperty* pTargetProperty, CBindingBase* pBinding, SetValueFunc SetFunc );

    protected:
        CBindingManager();
        virtual ~CBindingManager();

        HRESULT Initialize();

        HRESULT GetBindingContext( CPropertyObject* pTarget, CDefaultBindingContext** ppContext );

        static void OnBindingInvalidated( CBindingBase* pBinding, SetValueFunc SetFunc );
        static HRESULT SetValueFromBinding( CBindingBase* pBinding, SetValueFunc SetFunc );
};

