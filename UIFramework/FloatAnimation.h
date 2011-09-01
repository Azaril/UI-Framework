#pragma once

#include "FloatAnimationBase.h"
#include "BasicTypes.h"

class UIFRAMEWORK_API CFloatAnimation : public CFloatAnimationBase
{
    public:
        DECLARE_FACTORY3( CFloatAnimation, FLOAT, FLOAT, const CTimeSpan& );

        __override virtual __checkReturn HRESULT GetCurrentValue(
            __in CAnimationClock* pClock,
            __in CObjectWithType* pDefaultInitialValue,
            __in CObjectWithType* pDefaultFinalValue, 
            __deref_out CObjectWithType** ppValue
            );

    protected:
        CFloatAnimation(
            );

        virtual ~CFloatAnimation(
            );

        __checkReturn HRESULT Initialize( 
            FLOAT From, 
            FLOAT To, 
            const CTimeSpan& Duration 
            );

        CFloatValue* m_From;
        CFloatValue* m_To;
};