#pragma once

#include "RefCounted.h"
#include "TimeSource.h"
#include "Collections.h"
#include "Factory.h"
#include "TimeValue.h"

namespace AsyncTimeControllerOp
{
    enum Value
    {
        Add,
        Remove
    };
}

class UIFRAMEWORK_API CTimeController : public CRefCountedObjectBase< CTimeSource >
{
    public:
        DECLARE_FACTORY1( CTimeController, const CTime& );

        HRESULT UpdateTime( const CTime& Time );
        HRESULT Disconnect();

        virtual HRESULT AddSink( CTimeSink* pSink );
        virtual HRESULT RemoveSink( CTimeSink* pSink );

    protected:
        CTimeController();
        virtual ~CTimeController();

        HRESULT Initialize( const CTime& Time );

        HRESULT CallSinks( const CTimeSpan& Delta );
        HRESULT AddSinkInternal( CTimeSink* pSink );
        HRESULT RemoveSinkInternal( CTimeSink* pSink );

        CTime m_LastTime;
        vector< CTimeSink* > m_Sinks;
        BOOL m_CallingSinks;

        class CAsyncOp
        {
            public:
                CAsyncOp( AsyncTimeControllerOp::Value Op, CTimeSink* pSink ) : m_Operation(Op),
                                                                                m_Sink(pSink)
                {
                    AddRefObject(m_Sink);
                }

                CAsyncOp( const CAsyncOp& Other ) : m_Operation(Other.m_Operation),
                                                    m_Sink(Other.m_Sink)
                {
                    AddRefObject(m_Sink);
                }

                ~CAsyncOp()
                {
                    ReleaseObject(m_Sink);
                }

                CAsyncOp& operator=( CAsyncOp& Other )
                {
                    m_Operation = Other.m_Operation;
                    m_Sink = Other.m_Sink;

                    AddRefObject(m_Sink);

                    return *this;
                }

                AsyncTimeControllerOp::Value GetOperation()
                {
                    return m_Operation;
                }

                CTimeSink* GetSink()
                {
                    return m_Sink;
                }

            protected:
                AsyncTimeControllerOp::Value m_Operation;
                CTimeSink* m_Sink;
        };

        vector< CAsyncOp > m_AsyncOperations;
};

