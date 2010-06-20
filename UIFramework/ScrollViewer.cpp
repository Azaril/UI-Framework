#include "ScrollViewer.h"
#include "RoutedCommand.h"
#include "RoutedCommandEventArgs.h"
#include "ScrollBar.h"

CScrollViewer::CScrollViewer() : m_ScrollPresenter(NULL)
{
}

CScrollViewer::~CScrollViewer()
{
    m_RoutedCommandExecutedConnection.disconnect();

    ReleaseObject(m_ScrollPresenter);
}

HRESULT CScrollViewer::Initialize(CProviders* pProviders)
{
    HRESULT hr = S_OK;

    IFCPTR(pProviders);

    IFC(CContentControl::Initialize(pProviders));

    IFC(AddHandler(&CRoutedCommand::ExecutedEvent, boost::bind(&CScrollViewer::OnCommandExecuted, this, _1, _2), &m_RoutedCommandExecutedConnection));

Cleanup:
    return hr;
}

HRESULT CScrollViewer::OnAttach(CUIAttachContext& Context)
{
    HRESULT hr = S_OK;

    IFC(CControl::OnAttach(Context));

    IFC(GetTemplateChild(L"PART_ScrollContentPresenter", &m_ScrollPresenter));

Cleanup:
    return hr;
}

HRESULT CScrollViewer::OnDetach(CUIDetachContext& Context)
{
    HRESULT hr = S_OK;

    ReleaseObject(m_ScrollPresenter);

    IFC(CControl::OnDetach(Context));

Cleanup:
    return hr;
}

HRESULT CScrollViewer::HitTest(Point2F LocalPoint, CHitTestResult** ppHitTestResult)
{
    HRESULT hr = S_OK;

    IFCPTR(ppHitTestResult);

    *ppHitTestResult = NULL;

Cleanup:
    return hr;
}

void CScrollViewer::OnCommandExecuted(CObjectWithType* pSender, CRoutedEventArgs* pRoutedEventArgs)
{
    HRESULT hr = S_OK;
    CRoutedCommandEventArgs* pRoutedCommandEventArgs = NULL;
    CCommand* pCommand = NULL;

    IFCPTR(pSender);
    IFCPTR(pRoutedEventArgs);

    IFC(CastType(pRoutedEventArgs, &pRoutedCommandEventArgs));

    pCommand = pRoutedCommandEventArgs->GetCommand();
    IFCPTR(pCommand);

    if(pCommand == &CScrollBar::LineUpCommand)
    {
        IFC(LineUp());

        pRoutedCommandEventArgs->SetHandled();
    }
    else if(pCommand == &CScrollBar::LineDownCommand)
    {
        IFC(LineDown());

        pRoutedCommandEventArgs->SetHandled();
    }

Cleanup:
    ;
}

HRESULT CScrollViewer::LineUp()
{
    HRESULT hr = S_OK;

    if(m_ScrollPresenter)
    {
        IFC(m_ScrollPresenter->LineUp());
    }

Cleanup:
    return hr;
}

HRESULT CScrollViewer::LineDown()
{
    HRESULT hr = S_OK;

    if(m_ScrollPresenter)
    {
        IFC(m_ScrollPresenter->LineDown());
    }

Cleanup:
    return hr;
}