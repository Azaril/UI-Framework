#include "EvaluateMarkupExtensionCommand.h"
#include "MarkupExtension.h"

CEvaluateMarkupExtensionCommand::CEvaluateMarkupExtensionCommand(
	)
{
}

CEvaluateMarkupExtensionCommand::~CEvaluateMarkupExtensionCommand(
	)
{
}

__checkReturn HRESULT 
CEvaluateMarkupExtensionCommand::Initialize(
	)
{
    HRESULT hr = S_OK;

    return hr;
}

__override __checkReturn HRESULT 
CEvaluateMarkupExtensionCommand::Execute(
	CParserCommandContext& Context
	)
{
    HRESULT hr = S_OK;
    CObjectWithType* pCurrentObject = NULL;
    CObjectWithType* pEvaluatedObject = NULL;

    IFC(Context.GetObject(&pCurrentObject));

    if(pCurrentObject->IsTypeOf(TypeIndex::MarkupExtension))
    {
        CMarkupExtension* pExtension = (CMarkupExtension*)pCurrentObject;

        IFC(Context.PopObject());

        IFC(pExtension->ExecuteMarkup(&pEvaluatedObject));

        IFC(Context.PushObject(pEvaluatedObject));
    }

Cleanup:
    ReleaseObject(pCurrentObject);
    ReleaseObject(pEvaluatedObject);

    return hr;
}