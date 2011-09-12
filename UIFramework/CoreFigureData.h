#pragma once

#include "RefCounted.h"
#include "Factory.h"

namespace FigureDataType
{
    enum Value
    {
        BeginFigure         = 0x00,
        EndFigure           = 0x01,
        Line                = 0x02,
        Bezier              = 0x03,

        ActionTypeMask      = 0x0F,

        OpenFilled          = 0x00,
        OpenHollow          = 0x10,

        EndOpened           = 0x00,
        EndClosed           = 0x20,

        OptionTypeMask      = 0xF0
    };
}

class CCoreFigureData : public CRefCountedObject
{
    public:
        DECLARE_FACTORY4( CCoreFigureData, FigureDataType::Value*, UINT32, Point2F*, UINT32 );

    protected:
        CCoreFigureData(
            );

        virtual ~CCoreFigureData(
            );

        __checkReturn HRESULT Initialize(
            __in_ecount(FigureDataSize) FigureDataType::Value* pFigureData,
            UINT32 FigureDataSize,
            __in_ecount(FigurePointSize) Point2F* pFigurePoints,
            UINT32 FigurePointSize
            );

        FigureDataType::Value* m_pFigureData;
        UINT32 m_FigureDataSize;
        Point2F* m_pFigurePoints;
        UINT32 m_FigurePointSize;
};

