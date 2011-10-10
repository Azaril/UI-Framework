#include "CoreShapeData.h"
#include "CoreFigureBuilder.h"

CCoreShapeData::CCoreShapeData(
    )
{
}

CCoreShapeData::~CCoreShapeData(
    )
{
    for (vector< CCoreFigureData* >::iterator it = m_Figures.begin(); it != m_Figures.end(); ++it)
    {
        (*it)->Release();
    }
}

__checkReturn HRESULT 
CCoreShapeData::Initialize(
    )
{
    HRESULT hr = S_OK;

    return hr;
}

__override __checkReturn HRESULT
CCoreShapeData::SetFillMode(
    FillMode::Value fillMode
    )
{
    HRESULT hr = S_OK;

    IFCEXPECT(m_Figures.empty());

    m_FillMode = fillMode;

Cleanup:
    return hr;
}

__override __checkReturn HRESULT
CCoreShapeData::CreateFigureBuilder(
    __deref_out CFigureBuilder** ppFigureBuilder
    )
{
    HRESULT hr = S_OK;
    CCoreFigureBuilder* pCoreFigureBuilder = NULL;

    IFC(CCoreFigureBuilder::Create(this, &pCoreFigureBuilder));

    *ppFigureBuilder = pCoreFigureBuilder;
    pCoreFigureBuilder = NULL;

Cleanup:
    ReleaseObject(pCoreFigureBuilder);

    return hr;
}

__checkReturn HRESULT
CCoreShapeData::AddFigureData(
    __in CCoreFigureData* pFigureData
    )
{
    HRESULT hr = S_OK;

    m_Figures.push_back(pFigureData);
    AddRefObject(pFigureData);

    return hr;
}