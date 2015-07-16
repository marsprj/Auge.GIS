#include "DescribeMapHandler.h"
#include "DescribeMapRequest.h"
#include "DescribeMapResponse.h"
#include "AugeCarto.h"
#include "AugeService.h"
#include "AugeUser.h"

namespace auge
{
	DescribeMapHandler::DescribeMapHandler()
	{

	}

	DescribeMapHandler::~DescribeMapHandler()
	{

	}

	const char*	DescribeMapHandler::GetName()
	{
		return "DescribeMap";
	}

	WebRequest*	DescribeMapHandler::ParseRequest(rude::CGI& cgi)
	{
		DescribeMapRequest* pRequest = new DescribeMapRequest();

		if(!pRequest->Create(cgi))
		{
			GLogger* pLogger = augeGetLoggerInstance();
			pLogger->Error("[Request] is NULL", __FILE__, __LINE__);
			pRequest->Release();
			pRequest = NULL;
		}
		return pRequest;
	}

	WebRequest* DescribeMapHandler::ParseRequest(rude::CGI& cgi, const char* mapName)
	{
		return ParseRequest(cgi);
	}

	WebRequest*	DescribeMapHandler::ParseRequest(XDocument* pxDoc, const char* mapName)
	{
		return NULL;
	}

	WebResponse* DescribeMapHandler::Execute(WebRequest* pWebRequest, User* pUser)
	{
		return NULL;

		//DescribeMapRequest* pRequest = static_cast<DescribeMapRequest*>(pWebRequest);
		//DescribeMapResponse* pWebResponse = new DescribeMapResponse(pRequest);
		//CartoManager* pCartoManager = augeGetCartoManagerInstance();

		//const char* name = pRequest->GetName();
		//if(name==NULL)
		//{
		//	EnumMap* pMaps = pCartoManager->DescribeMaps();
		//	pWebResponse->SetMaps(pMaps);
		//}
		//else
		//{
		//	Map* pMap = pCartoManager->LoadMap(name);
		//	pWebResponse->SetMap(pMap);
		//}
		//return pWebResponse;
	}

	WebResponse* DescribeMapHandler::Execute(WebRequest* pWebRequest, WebContext* pWebContext, User* pUser)
	{
		WebResponse* pWebResponse = NULL;
		DescribeMapRequest* pRequest = static_cast<DescribeMapRequest*>(pWebRequest);		
		CartoManager* pCartoManager = augeGetCartoManagerInstance();

		const char* name = pRequest->GetName();
		if(name==NULL)
		{
			EnumMap* pMaps = pCartoManager->GetMaps(pUser->GetID());
			DescribeMapResponse* pResponse = new DescribeMapResponse(pRequest);
			pResponse->SetMaps(pMaps);
			pResponse->SetWebContext(pWebContext);
			pWebResponse = pResponse;
		}
		else
		{
			Map* pMap = pCartoManager->LoadMap(pUser->GetID(), name);
			if(pMap==NULL)
			{
				GError* pError = augeGetErrorInstance();
				WebExceptionResponse* pExpResponse = augeCreateWebExceptionResponse();
				pExpResponse->SetMessage(pError->GetLastError());
				pWebResponse = pExpResponse;
			}
			else
			{
				DescribeMapResponse* pResponse = new DescribeMapResponse(pRequest);
				pResponse->SetMap(pMap);
				pResponse->SetWebContext(pWebContext);
				pWebResponse = pResponse;;
			}
		}
		
		return pWebResponse;
	}
}