#pragma once

#include "GeometrySink.h"
#include "RefCounted.h"
#include "Factory.h"
#include "ShapeData.h"
#include "GeometryBuilder.h"

template< typename T >
class CShapeBuilder : public T
{
    public:
        DECLARE_FACTORY1( CShapeBuilder, CShapeData* );

        __override virtual __checkReturn HRESULT Close(
            )
        {
            HRESULT hr = S_OK;

            IFCEXPECT(!m_FigureOpen);

            if (m_pFigureBuilder != NULL)
            {
                IFC(m_pFigureBuilder->Close());
            }

        Cleanup:
            ReleaseObject(m_pFigureBuilder);

            return hr;
        }

        __override virtual __checkReturn HRESULT SetFillMode(
            FillMode::Value fillMode
            )
        {
            HRESULT hr = S_OK;

            IFCEXPECT(!m_FigureOpen);

            m_pShapeData->SetFillMode(fillMode);

        Cleanup:
            return hr;
        }

        __override virtual __checkReturn HRESULT BeginFigure(
            const Point2F& startPoint,
            BeginFigureOptions::Value options
            )
        {
            HRESULT hr = S_OK;

            IFCEXPECT(!m_FigureOpen);

            if (m_pFigureBuilder == NULL)
            {
                IFC(m_pShapeData->CreateFigureBuilder(&m_pFigureBuilder));
            }

            IFC(m_pFigureBuilder->BeginFigure(startPoint, options));

            m_FigureOpen = TRUE;

        Cleanup:
            return hr;
        }

        __override virtual __checkReturn HRESULT EndFigure(
            EndFigureOptions::Value options
            )
        {
            HRESULT hr = S_OK;

            IFCEXPECT(m_FigureOpen);

            IFC(m_pFigureBuilder->EndFigure(options));

        Cleanup:
            return hr;
        }

        __override virtual __checkReturn HRESULT AddLines(
            __in_ecount(PointCount) const Point2F* pPoints,
            UINT32 Points
            )
        {
            HRESULT hr = S_OK;

            IFCEXPECT(m_FigureOpen);

            IFC(m_pFigureBuilder->AddLines(pPoints, Points));

        Cleanup:
            return hr;
        }

        __override virtual __checkReturn HRESULT AddBeziers(
            __in_ecount(BezierCount) const BezierSegment* pBeziers,
            UINT32 BezierCount
            )
        {
            HRESULT hr = S_OK;

            IFCEXPECT(m_FigureOpen);

            IFC(m_pFigureBuilder->AddBeziers(pBeziers, BezierCount));

        Cleanup:
            return hr;
        }

    protected:
        CShapeBuilder(
            )
            : m_pShapeData(NULL)
            , m_pFigureBuilder(NULL)
            , m_FigureOpen(FALSE)
        {
        }

        virtual ~CShapeBuilder(
            )
        {
            ReleaseObject(m_pShapeData);
            ReleaseObject(m_pFigureBuilder);
        }

        __checkReturn HRESULT Initialize(
            __in CShapeData* pShapeData
            )
        {
            HRESULT hr = S_OK;

            m_pShapeData = pShapeData;
            AddRefObject(m_pShapeData);

            return hr;
        }

        CShapeData* m_pShapeData;
        CFigureBuilder* m_pFigureBuilder;
        bool m_FigureOpen;
};

