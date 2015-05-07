#include "GetDataSourceRequest.h"
#include "GetDataSourceResponse.h"
#include "AugeService.h"
#include "AugeData.h"

namespace auge
{
	GetDataSourceResponse::GetDataSourceResponse(GetDataSourceRequest* pRequest)
	{
		m_pRequest = pRequest;
		m_pRequest->AddRef();
	}

	GetDataSourceResponse::~GetDataSourceResponse()
	{
	}

	RESULTCODE GetDataSourceResponse::Write(WebWriter* pWriter)
	{
		if(pWriter==NULL)
		{
			return AG_FAILURE;
		}

		GLogger* pLogger = augeGetLoggerInstance();

		Workspace* pWorkspace = NULL;
		ConnectionManager* pConnManager = augeGetConnectionManagerInstance();

		XDocument	*pxDoc = new XDocument();
		
		const char* name = m_pRequest->GetName();
		if(name==NULL)
		{
			XElement *pxRoot = pxDoc->CreateRootNode("IMS_DataSources", NULL, NULL);
			EnumWorkspace* pEnum = pConnManager->GetWorkspaces();
			pEnum->Reset();
			while((pWorkspace=pEnum->Next()))
			{
				AddDataSourceNode(pxRoot, pWorkspace);
			}
			pEnum->Release();
			//g_uint count = pConnManager->GetCount();
			//for(g_uint i=0; i<count; i++)
			//{
			//	pWorkspace = pConnManager->GetWorkspace(i);
			//	AddDataSourceNode(pxRoot, pWorkspace);
			//}
		}
		else
		{
			pWorkspace = pConnManager->GetWorkspace(name);
			if(pWorkspace!=NULL)
			{
				AddDataSourceNode(pxDoc, pWorkspace);
			}
		}

		int len = 0;
		g_uchar* buffer = NULL;
		pxDoc->WriteToString(&buffer, len, "GBK",0);

		pWriter->WriteHead(m_pRequest->GetMimeType());
		pWriter->Write(buffer, len);
		pWriter->WriteTail();

		pLogger->Info((g_char*)buffer);

		pxDoc->Close();
		AUGE_SAFE_RELEASE(pxDoc);
		return AG_SUCCESS;
	}

	void GetDataSourceResponse::AddDataSourceNode(XElement* pxRoot, Workspace* pWorkspace)
	{
		XElement	*pxNode = NULL;
		XElement	*pxDataSource  = NULL;

		pxDataSource = pxRoot->AddChild("DataSource", NULL);

		pxNode = pxDataSource->AddChild("Name");
		pxNode->AddChildText(pWorkspace->GetName());
		pxNode = pxDataSource->AddChild("Engine");
		pxNode->AddChildText(pWorkspace->GetEngine()->GetID());
		pxNode = pxDataSource->AddChild("ConnectionString");
		pxNode->AddChildText(pWorkspace->GetConnectionString());
	}

	void GetDataSourceResponse::AddDataSourceNode(XDocument* pxDoc, Workspace* pWorkspace)
	{
		XElement	*pxNode = NULL;
		XElement	*pxDataSource  = NULL;
		pxDataSource = pxDoc->CreateRootNode("DataSource", NULL, NULL);
		pxNode = pxDataSource->AddChild("Name");
		pxNode->AddChildText(pWorkspace->GetName());
		pxNode = pxDataSource->AddChild("Engine");
		pxNode->AddChildText(pWorkspace->GetEngine()->GetID());
		pxNode = pxDataSource->AddChild("ConnectionString");
		pxNode->AddChildText(pWorkspace->GetConnectionString());
	}
}