#include "MatrixStack.h"
#include "ErrorChecking.h"

CMatrixStack::CMatrixStack()
{
    m_Identity = Matrix3X2::Identity();
    
    m_MatrixStack.push_back(m_Identity);
}

CMatrixStack::~CMatrixStack()
{
}

const Matrix3X2& CMatrixStack::GetTop()
{
    if(!m_MatrixStack.empty())
    {
        return m_MatrixStack.back();
    }

    return m_Identity;
}

HRESULT CMatrixStack::Push()
{
    HRESULT hr = S_OK;

    m_MatrixStack.push_back(GetTop());

    return hr;
}

HRESULT CMatrixStack::Pop()
{
    HRESULT hr = S_OK;

    IFCEXPECT(m_MatrixStack.size() > 1);

    m_MatrixStack.pop_back();

Cleanup:
    return hr;
}

HRESULT CMatrixStack::LoadIdentity()
{
    HRESULT hr = S_OK;

    IFC(LoadMatrix(m_Identity));

Cleanup:
    return hr;
}

HRESULT CMatrixStack::LoadMatrix(const Matrix3X2& Matrix)
{
    HRESULT hr =  S_OK;

    IFCEXPECT(!m_MatrixStack.empty());

    {
        Matrix3X2& CurrentMatrix = m_MatrixStack.back();

        CurrentMatrix = Matrix;
    }

Cleanup:
    return hr;
}

HRESULT CMatrixStack::MultMatrix(const Matrix3X2& Matrix)
{
    HRESULT hr =  S_OK;

    IFCEXPECT(!m_MatrixStack.empty());

    {
        Matrix3X2& CurrentMatrix = m_MatrixStack.back();

        CurrentMatrix = CurrentMatrix * Matrix;
    }

Cleanup:
    return hr;
}

HRESULT CMatrixStack::MultMatrixLocal(const Matrix3X2& Matrix)
{
    HRESULT hr =  S_OK;

    IFCEXPECT(!m_MatrixStack.empty());

    {
        Matrix3X2& CurrentMatrix = m_MatrixStack.back();

        CurrentMatrix = Matrix * CurrentMatrix;
    }

Cleanup:
    return hr;
}