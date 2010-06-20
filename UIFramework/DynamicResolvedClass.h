#pragma once

#include "ClassResolver.h"
#include "Factory.h"
#include "Providers.h"

typedef HRESULT (*CreateTypeFunc)( CProviders* pProviders, CPropertyObject** ppObject );
typedef HRESULT (*GetPropertyInformationFunc)( CPropertyInformation** ppPropertyInformation );
typedef HRESULT (*GetEventInformationFunc)( CEventInformation** ppEventInformation );
typedef HRESULT (*GetCommandInformationFunc)( CCommandInformation** ppCommand );

struct ClassInformation
{
    ClassInformation() : Name(NULL),
                         ClassType((TypeIndex::Value)-1),
                         CreateFunc(NULL),
                         GetPropertiesFunc(NULL),
                         GetEventsFunc(NULL),
                         GetCommandsFunc(NULL)
    {
    }

    ClassInformation(const WCHAR* pName, TypeIndex::Value Type, CreateTypeFunc Create, GetPropertyInformationFunc Properties, GetEventInformationFunc Events, GetCommandInformationFunc Commands) : Name(pName),
                                                                                                                                                                                                    ClassType(Type),
                                                                                                                                                                                                    CreateFunc(Create),
                                                                                                                                                                                                    GetPropertiesFunc(Properties),
                                                                                                                                                                                                    GetEventsFunc(Events),
                                                                                                                                                                                                    GetCommandsFunc(Commands)
    {
    }                        

    const WCHAR* Name;
    TypeIndex::Value ClassType;
    CreateTypeFunc CreateFunc;
    GetPropertyInformationFunc GetPropertiesFunc;
    GetEventInformationFunc GetEventsFunc;
    GetCommandInformationFunc GetCommandsFunc;
};

class CDynamicResolvedClass : public CRefCountedObjectBase< CResolvedClass >
{
    public:
        DECLARE_FACTORY1( CDynamicResolvedClass, const ClassInformation& );

        const WCHAR* GetName();
        virtual TypeIndex::Value GetType();

        virtual HRESULT CreateInstance( CProviders* pProviders, CPropertyObject** ppObject );

        virtual HRESULT GetPropertyInformation( CPropertyInformation** ppInformation );
        virtual HRESULT GetEventInformation( CEventInformation** ppInformation );
        virtual HRESULT GetCommandInformation( CCommandInformation** ppInformation );

    protected:
        CDynamicResolvedClass();
        virtual ~CDynamicResolvedClass();

        HRESULT Initialize( const ClassInformation& Info );

        ClassInformation m_Information;
        CPropertyInformation* m_CachedProperties;
        CEventInformation* m_CachedEvents;
        CCommandInformation* m_CachedCommands;
};