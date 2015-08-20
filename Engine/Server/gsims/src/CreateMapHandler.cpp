#include "CreateMapHandler.h"
#include "CreateMapRequest.h"
#include "AugeCarto.h"
#include "AugeService.h"
#include "AugeUser.h"

namespace auge
{
	CreateMapHandler::CreateMapHandler()
	{

	}

	CreateMapHandler::~CreateMapHandler()
	{

	}

	const char*	CreateMapHandler::GetName()
	{
		return "CreateMap";
	}

	const char*	CreateMapHandler::GetDescription()
	{
		return GetName();
	}

	WebRequest*	CreateMapHandler::ParseRequest(rude::CGI& cgi)
	{
		CreateMapRequest* pRequest = new CreateMapRequest();

		if(!pRequest->Create(cgi))
		{
			GLogger* pLogger = augeGetLoggerInstance();
			pLogger->Error("[Request] is NULL", __FILE__, __LINE__);
			pRequest->Release();
			pRequest = NULL;
		}
		return pRequest;
	}

	WebRequest* CreateMapHandler::ParseRequest(rude::CGI& cgi, const char* mapName)
	{
		return ParseRequest(cgi);
	}

	WebRequest*	CreateMapHandler::ParseRequest(XDocument* pxDoc, const char* mapName)
	{
		return NULL;
	}

	WebResponse* CreateMapHandler::Execute(WebRequest* pWebRequest, User* pUser)
	{	
		CreateMapRequest* pRequest = static_cast<CreateMapRequest*>(pWebRequest);

		const char* name = pRequest->GetName();
		if(name==NULL)
		{
			WebExceptionResponse* pExpResponse = augeCreateWebExceptionResponse();
			pExpResponse->SetMessage("Parameter [name] is NULL");
			return pExpResponse;
		}

		Map* pMap = NULL;
		CartoManager* pCartoManager = NULL;
		pCartoManager = augeGetCartoManagerInstance();
		pMap = pCartoManager->LoadMap(pUser->GetID(), name);
		if(pMap)
		{
			char msg[AUGE_MSG_MAX];
			g_sprintf(msg, "Map %s already exists.", name);
			WebExceptionResponse* pExpResponse = augeCreateWebExceptionResponse();
			pExpResponse->SetMessage(msg);
			return pExpResponse;
		}

		const char* extent = pRequest->GetExtent(); 
		if(extent==NULL)
		{
			WebExceptionResponse* pExpResponse = augeCreateWebExceptionResponse();
			pExpResponse->SetMessage("Parameter extent is null.");
			return pExpResponse;
		}

		double xmin=0.0f, ymin=0.0f, xmax=0.0f, ymax=0.0f;
		sscanf(extent,"%lf,%lf,%lf,%lf",&xmin, &ymin, &xmax, &ymax);
		int srid = atoi(pRequest->GetSRID());
		pMap = pCartoManager->CreateMap(pUser->GetID(), name, srid, xmin, ymin, xmax, ymax);
		if(!pMap)
		{
			GError* pError = augeGetErrorInstance();
			WebExceptionResponse* pExpResponse = augeCreateWebExceptionResponse();
			pExpResponse->SetMessage(pError->GetLastError());
			return pExpResponse;
		}				
		WebSuccessResponse* pSusResponse = augeCreateWebSuccessResponse();
		pSusResponse->SetRequest("CreateMap");
		return pSusResponse;
	}

	WebResponse* CreateMapHandler::Execute(WebRequest* pWebRequest, WebContext* pWebContext, User* pUser)
	{
		return Execute(pWebRequest, pUser);
	}
}