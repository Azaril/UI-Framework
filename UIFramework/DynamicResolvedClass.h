#pragma once

#include "ClassResolver.h"
#include "Factory.h"
#include "Providers.h"

typedef __checkReturn HRESULT (*CreateTypeFunc)( 
	__in CProviders* pProviders, 
	__deref_out CPropertyObject** ppObject 
	);

typedef __checkReturn HRESULT (*GetPropertyInformationFunc)( 
	__deref_out CPropertyInformation** ppPropertyInformation 
	);

typedef __checkReturn HRESULT (*GetEventInformationFunc)(
	__deref_out CEventInformation** ppEventInformation 
	);

typedef __checkReturn HRESULT (*GetCommandInformationFunc)( 
	__deref_out CCommandInformation** ppCommand 
	);

struct ClassInformation
{
    ClassInformation(
		) 
		: Name(NULL)
		, ClassType((TypeIndex::Value)-1)
		, CreateFunc(NULL)
		, GetPropertiesFunc(NULL)
		, GetEventsFunc(NULL)
		, GetCommandsFunc(NULL)
    {
    }

    ClassInformation(
		__in_z const WCHAR* pName, 
		TypeIndex::Value Type, 
		__in_opt CreateTypeFunc Create, 
		__in_opt GetPropertyInformationFunc Properties, 
		__in_opt GetEventInformationFunc Events, 
		__in_opt GetCommandInformationFunc Commands
		) 
		: Name(pName)
		, ClassType(Type)
		, CreateFunc(Create)
		, GetPropertiesFunc(Properties)
		, GetEventsFunc(Events)
		, GetCommandsFunc(Commands)
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

        const WCHAR* GetName(
			);

        __override virtual TypeIndex::Value GetType(
			);

        __override virtual __checkReturn HRESULT CreateInstance( 
			__in CProviders* pProviders,
			__deref_out CPropertyObject** ppObject 
			);

        __override virtual __checkReturn HRESULT GetPropertyInformation(
			__deref_out CPropertyInformation** ppInformation 
			);

        __override virtual __checkReturn HRESULT GetEventInformation(
			__deref_out CEventInformation** ppInformation 
			);

        __override virtual __checkReturn HRESULT GetCommandInformation( 
			__deref_out CCommandInformation** ppInformation 
			);

    protected:
        CDynamicResolvedClass(
			);

        virtual ~CDynamicResolvedClass(
			);

        __checkReturn HRESULT Initialize(
			const ClassInformation& Info 
			);

        ClassInformation m_Information;
        CPropertyInformation* m_CachedProperties;
        CEventInformation* m_CachedEvents;
        CCommandInformation* m_CachedCommands;
};