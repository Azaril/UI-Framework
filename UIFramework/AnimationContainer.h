#pragma once

#include "RefCounted.h"
#include "Factory.h"
#include "PropertyObject.h"

struct IAnimation
{
    virtual INT32 AddRef() = 0;
    virtual INT32 Release() = 0;
};

struct IAnimatable
{
    virtual HRESULT ApplyAnimationValue( CProperty* pProperty, CObjectWithType* pValue ) = 0;
};

struct ITimeSink
{
    virtual INT32 AddRef() = 0;
    virtual INT32 Release() = 0;

    virtual HRESULT OnTimeUpdate( UINT32 Time ) = 0;
};

struct ITimeSource
{
    virtual INT32 AddRef() = 0;
    virtual INT32 Release() = 0;

    virtual HRESULT GetTime( UINT32* pTime ) = 0;
    virtual HRESULT AddSink( ITimeSink* pSink ) = 0;
    virtual HRESULT RemoveSink( ITimeSink* pSink ) = 0;
};

class CAnimationContainer : public CRefCountedObjectBase< ITimeSink >
{
    public:
        DECLARE_FACTORY2( CAnimationContainer, IAnimatable*, ITimeSource* );

        HRESULT BeginAnimation( IAnimation* pAnimation );

        virtual HRESULT OnTimeUpdate( UINT32 Time );

    protected:
        struct AnimationInfo
        {
            IAnimation* Animation;
            UINT32 StartTime;

            AnimationInfo(IAnimation* pAnimation, UINT32 Time) : Animation(pAnimation),
                                                                 StartTime(Time)
            {
                AddRefObject(Animation);
            }

            AnimationInfo(const AnimationInfo& Info) : Animation(Info.Animation),
                                                       StartTime(Info.StartTime)
            {
                AddRefObject(Animation);
            }

            ~AnimationInfo()
            {
                ReleaseObject(Animation);
            }
        };

        CAnimationContainer();
        virtual ~CAnimationContainer();

        HRESULT Initialize( IAnimatable* pTargetObject, ITimeSource* pTimeSource );

        ITimeSource* m_TimeSource;
        IAnimatable* m_TargetObject;
        std::vector< AnimationInfo > m_Animations;
};
