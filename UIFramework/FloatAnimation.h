#pragma once

#include "FloatAnimationBase.h"
#include "BasicTypes.h"
#include "Providers.h"
#include "StaticPropertyInformation.h"

class UIFRAMEWORK_API CFloatAnimation : public CFloatAnimationBase
{
    public:
        DECLARE_FACTORY1( CFloatAnimation, CProviders* );
        DECLARE_FACTORY3( CFloatAnimation, FLOAT, FLOAT, const CTimeSpan& );

        DECLARE_TYPE_WITH_BASE( TypeIndex::FloatAnimation, CFloatAnimationBase );

        static __checkReturn HRESULT CreatePropertyInformation( 
			__deref_out CPropertyInformation** ppInformation 
			);

        __override virtual __checkReturn HRESULT GetCurrentValue(
            __in CAnimationClock* pClock,
            __in CObjectWithType* pDefaultInitialValue,
            __in CObjectWithType* pDefaultFinalValue, 
            __deref_out CObjectWithType** ppValue
            );

        //
        // Properties
        //
        static CStaticProperty FromProperty;
        static CStaticProperty ToProperty;
        static CStaticProperty DurationProperty;

    protected:
        CFloatAnimation(
            );

        virtual ~CFloatAnimation(
            );

        __checkReturn HRESULT Initialize(
            __in CProviders* pProviders
            );

        __checkReturn HRESULT Initialize( 
            FLOAT From, 
            FLOAT To, 
            const CTimeSpan& Duration 
            );

        __override virtual __checkReturn HRESULT SetValueInternal(
            __in CProperty* pProperty,
            __in CObjectWithType* pValue 
            );

        __override virtual __checkReturn HRESULT GetValueInternal(
            __in CProperty* pProperty, 
            __deref_out_opt CObjectWithType** ppValue 
            );

        CFloatValue* m_From;
        CFloatValue* m_To;
};

template< >
struct ObjectTypeTraits< CFloatAnimation >
{
    static const TypeIndex::Value Type = TypeIndex::FloatAnimation;
};