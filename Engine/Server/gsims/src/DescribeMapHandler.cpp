#include "DescribeMapHandler.h"
#include "DescribeMapRequest.h"
#include "DescribeMapResponse.h"
#include "AugeCarto.h"

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

	WebResponse* DescribeMapHandler::Execute(WebRequest* pWebRequest)
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

	WebResponse* DescribeMapHandler::Execute(WebRequest* pWebRequest, WebContext* pWebContext)
	{
		DescribeMapRequest* pRequest = static_cast<DescribeMapRequest*>(pWebRequest);
		DescribeMapResponse* pWebResponse = new DescribeMapResponse(pRequest);
		CartoManager* pCartoManager = augeGetCartoManagerInstance();

		const char* name = pRequest->GetName();
		if(name==NULL)
		{
			EnumMap* pMaps = pCartoManager->GetMaps();
			pWebResponse->SetMaps(pMaps);
		}
		else
		{
			Map* pMap = pCartoManager->LoadMap(name);
			pWebResponse->SetMap(pMap);
		}

		pWebResponse->SetWebContext(pWebContext);
		return pWebResponse;
	}
}