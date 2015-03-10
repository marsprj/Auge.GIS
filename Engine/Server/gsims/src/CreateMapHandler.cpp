#include "CreateMapHandler.h"
#include "CreateMapRequest.h"
#include "AugeCarto.h"
#include "AugeService.h"

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

	WebRequest*	CreateMapHandler::ParseRequest(XDocument* pxDoc)
	{
		return NULL;
	}

	WebResponse* CreateMapHandler::Execute(WebRequest* pWebRequest)
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
		pMap = pCartoManager->GetMap(name);
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
		pMap = pCartoManager->CreateMap(name, srid, xmin, ymin, xmax, ymax);
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
}