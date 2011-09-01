#include "MatrixStack.h"
#include "ErrorChecking.h"

CMatrixStack::CMatrixStack(
	)
{
    m_Identity = Matrix3X2F::Identity();
    
    m_MatrixStack.push_back(m_Identity);
}

CMatrixStack::~CMatrixStack(
	)
{
}

const Matrix3X2F& 
CMatrixStack::GetTop(
	)
{
    if(!m_MatrixStack.empty())
    {
        return m_MatrixStack.back();
    }

    return m_Identity;
}

__checkReturn HRESULT 
CMatrixStack::Push(
	)
{
    HRESULT hr = S_OK;

    m_MatrixStack.push_back(GetTop());

    return hr;
}

__checkReturn HRESULT 
CMatrixStack::Pop(
	)
{
    HRESULT hr = S_OK;

    IFCEXPECT(m_MatrixStack.size() > 1);

    m_MatrixStack.pop_back();

Cleanup:
    return hr;
}

__checkReturn HRESULT 
CMatrixStack::LoadIdentity(
	)
{
    HRESULT hr = S_OK;

    IFC(LoadMatrix(m_Identity));

Cleanup:
    return hr;
}

__checkReturn HRESULT 
CMatrixStack::LoadMatrix(
	const Matrix3X2F& Matrix
	)
{
    HRESULT hr =  S_OK;

    IFCEXPECT(!m_MatrixStack.empty());

    {
        Matrix3X2F& CurrentMatrix = m_MatrixStack.back();

        CurrentMatrix = Matrix;
    }

Cleanup:
    return hr;
}

__checkReturn HRESULT 
CMatrixStack::MultMatrix(
	const Matrix3X2F& Matrix
	)
{
    HRESULT hr =  S_OK;

    IFCEXPECT(!m_MatrixStack.empty());

    {
        Matrix3X2F& CurrentMatrix = m_MatrixStack.back();

        CurrentMatrix = CurrentMatrix * Matrix;
    }

Cleanup:
    return hr;
}

__checkReturn HRESULT 
CMatrixStack::MultMatrixLocal(
	const Matrix3X2F& Matrix
	)
{
    HRESULT hr =  S_OK;

    IFCEXPECT(!m_MatrixStack.empty());

    {
        Matrix3X2F& CurrentMatrix = m_MatrixStack.back();

        CurrentMatrix = Matrix * CurrentMatrix;
    }

Cleanup:
    return hr;
}