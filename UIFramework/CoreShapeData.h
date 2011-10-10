#pragma once

#include "ShapeData.h"
#include "Factory.h"
#include "CoreFigureData.h"

class CCoreShapeData : public CShapeData
{
    public:
        DECLARE_FACTORY( CCoreShapeData );

        __override virtual __checkReturn HRESULT SetFillMode(
            FillMode::Value fillMode
            );

        __override virtual __checkReturn HRESULT CreateFigureBuilder(
            __deref_out CFigureBuilder** ppFigureBuilder
            );

        __checkReturn HRESULT AddFigureData(
            __in CCoreFigureData* pFigure
            );

    public:
        CCoreShapeData(
            );

        virtual ~CCoreShapeData(
            );

        __checkReturn HRESULT Initialize(
            );

        vector< CCoreFigureData* > m_Figures;
        FillMode::Value m_FillMode;
};

