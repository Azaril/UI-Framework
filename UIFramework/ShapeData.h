#pragma once

#include "RefCounted.h"
#include "GeometrySink.h"
#include "FigureBuilder.h"

class UIFRAMEWORK_API CShapeData : public CRefCountedObjectBase< >
{
    public:
        virtual __checkReturn HRESULT SetFillMode(
            FillMode::Value fillMode
            ) = 0;

        virtual __checkReturn HRESULT CreateFigureBuilder(
            __deref_out CFigureBuilder** ppFigureBuilder
            ) = 0;
};

