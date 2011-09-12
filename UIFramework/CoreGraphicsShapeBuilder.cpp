#include "CoreGraphicsShapeBuilder.h"
#include "CoreShapeData.h"

CCoreGraphicsShapeBuilder::CCoreGraphicsShapeBuilder(
    )
    : m_pPathGeometry(NULL)
{
}


CCoreGraphicsShapeBuilder::~CCoreGraphicsShapeBuilder(
    )
{
    ReleaseObject(m_pPathGeometry);
}

__checkReturn HRESULT 
CCoreGraphicsShapeBuilder::Initialize(
    )
{
    HRESULT hr = S_OK;
    CCoreShapeData* pShapeData = NULL;

    IFC(CCoreShapeData::Create(&pShapeData));

    IFC(CCorePathGraphicsGeometry::Create(pShapeData, &m_pPathGeometry));

    IFC(CShapeBuilder< CGraphicsGeometryBuilder >::Initialize(pShapeData));

Cleanup:
    ReleaseObject(pShapeData);

    return hr;
}

__checkReturn HRESULT
CCoreGraphicsShapeBuilder::GetGraphicsGeometry(
    __deref_out CGraphicsGeometry** ppGeometry
    )
{
    HRESULT hr = S_OK;

    *ppGeometry = m_pPathGeometry;
    AddRefObject(m_pPathGeometry);

    return hr;
}