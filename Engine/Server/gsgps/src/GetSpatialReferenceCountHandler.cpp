#include "GetSpatialReferenceCountHandler.h"
#include "GetSpatialReferenceCountRequest.h"
#include "GetSpatialReferenceCountResponse.h"
#include "AugeService.h"
#include "AugeXML.h"

namespace auge
{
	GetSpatialReferenceCountHandler::GetSpatialReferenceCountHandler()
	{
		
	}

	GetSpatialReferenceCountHandler::~GetSpatialReferenceCountHandler()
	{
		
	}

	const char*	GetSpatialReferenceCountHandler::GetName()
	{
		return "GetSpatialReferenceCount";
	}

	WebRequest*	GetSpatialReferenceCountHandler::ParseRequest(rude::CGI& cgi)
	{
		GetSpatialReferenceCountRequest* pRequest = new GetSpatialReferenceCountRequest();
		if(!pRequest->Create(cgi))
		{
			GLogger* pLogger = augeGetLoggerInstance();
			pLogger->Error("[Request] is NULL", __FILE__, __LINE__);
			pRequest->Release();
			pRequest = NULL;
		}
		return pRequest;
	}

	WebRequest*	GetSpatialReferenceCountHandler::ParseRequest(rude::CGI& cgi, const char* mapName)
	{
		GetSpatialReferenceCountRequest* pRequest = new GetSpatialReferenceCountRequest();
		if(!pRequest->Create(cgi))
		{
			GLogger* pLogger = augeGetLoggerInstance();
			pLogger->Error("[Request] is NULL", __FILE__, __LINE__);
			pRequest->Release();
			pRequest = NULL;
		}
		return pRequest;
	}

	WebRequest*	GetSpatialReferenceCountHandler::ParseRequest(XDocument* pxDoc, const char* mapName)
	{
		return NULL;
	}

	WebResponse* GetSpatialReferenceCountHandler::Execute(WebRequest* pWebRequest, User* pUser)
	{
		return NULL;
	}

	WebResponse* GetSpatialReferenceCountHandler::Execute(WebRequest* pWebRequest, WebContext* pWebContext, User* pUser)
	{
		return (new GetSpatialReferenceCountResponse(static_cast<GetSpatialReferenceCountRequest*>(pWebRequest)));
	}
}
