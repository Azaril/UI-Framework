#pragma once

#include "FloatAnimationBase.h"
#include "BasicTypes.h"

class UIFRAMEWORK_API CFloatAnimation : public CFloatAnimationBase
{
    public:
        DECLARE_FACTORY3( CFloatAnimation, FLOAT, FLOAT, const CTimeSpan& );

        virtual HRESULT GetCurrentValue(CAnimationClock* pClock, CObjectWithType* pDefaultInitialValue, CObjectWithType* pDefaultFinalValue, CObjectWithType** ppValue);

    protected:
        CFloatAnimation();
        virtual ~CFloatAnimation();

        HRESULT Initialize( FLOAT From, FLOAT To, const CTimeSpan& Duration );

        CFloatValue* m_From;
        CFloatValue* m_To;
};