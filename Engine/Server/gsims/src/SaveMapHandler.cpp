#include "SaveMapHandler.h"
#include "SaveMapRequest.h"
#include "AugeCarto.h"
#include "AugeService.h"

namespace auge
{
	SaveMapHandler::SaveMapHandler()
	{

	}

	SaveMapHandler::~SaveMapHandler()
	{

	}

	const char*	SaveMapHandler::GetName()
	{
		return "SaveMap";
	}

	const char*	SaveMapHandler::GetDescription()
	{
		return GetName();
	}

	WebRequest*	SaveMapHandler::ParseRequest(rude::CGI& cgi)
	{
		return NULL;
	}

	WebRequest* SaveMapHandler::ParseRequest(rude::CGI& cgi, const char* mapName)
	{
		return ParseRequest(cgi);
	}

	WebRequest*	SaveMapHandler::ParseRequest(XDocument* pxDoc, const char* mapName)
	{
		SaveMapRequest* pRequest = new SaveMapRequest();
		//if(!pRequest->Create(pxDoc, pMap))
		if(!pRequest->Create(pxDoc))
		{
			GLogger* pLogger = augeGetLoggerInstance();
			pLogger->Error("[Request] is NULL", __FILE__, __LINE__);
			pRequest->Release();
			return NULL;
		}
		//if((mapName!=NULL)&&(strlen(mapName)>0))
		//{
		//	pRequest->SetMapName(mapName);
		//}
		//pRequest->Info();
		return pRequest;
	}

	WebResponse* SaveMapHandler::Execute(WebRequest* pWebRequest, User* pUser)
	{	
		GLogger* pLogger = augeGetLoggerInstance();
		SaveMapRequest* pRequest = static_cast<SaveMapRequest*>(pWebRequest);

		const char* name = pRequest->GetName();
		if(name==NULL)
		{
			const char* msg = "Parameter [name] is NULL"; 
			pLogger->Error(msg,__FILE__,__LINE__);
			WebExceptionResponse* pExpResponse = augeCreateWebExceptionResponse();
			pExpResponse->SetMessage(msg);
			return pExpResponse;
		}

		GEnvelope& extent = pRequest->GetExtent();
		GEnvelope& viewer = pRequest->GetViewer();

		char layers[AUGE_PATH_MAX];
		memset(layers, 0, AUGE_PATH_MAX);
		g_uint count = pRequest->GetLayerCount();
		for(g_int i=count-1; i>=0; i--)
		//for(g_int i=0; i<count; i++)
		{
			strcat(layers, pRequest->GetLayerID(i));
			strcat(layers, ":");
			strcat(layers, pRequest->IsLayerVisible(i) ? "1" : "0");
			if(i)
			{
				strcat(layers, ",");
			}
		}
		pLogger->Debug(layers, __FILE__, __LINE__);

		CartoManager* pCartoManager = augeGetCartoManagerInstance();
		RESULTCODE rc = pCartoManager->SaveMap(0, name, atoi(pRequest->GetSRID()), extent, viewer, layers);
		if(rc != AG_SUCCESS)
		{
			GError* pError = augeGetErrorInstance();
			pLogger->Error(pError->GetLastError(),__FILE__,__LINE__);
			WebExceptionResponse* pExpResponse = augeCreateWebExceptionResponse();
			pExpResponse->SetMessage(pError->GetLastError());
			return pExpResponse;
		}

		WebSuccessResponse* pSusResponse = augeCreateWebSuccessResponse();
		pSusResponse->SetRequest(pRequest->GetRequest());
		return pSusResponse;
	}

	WebResponse* SaveMapHandler::Execute(WebRequest* pWebRequest, WebContext* pWebContext, User* pUser)
	{
		return Execute(pWebRequest, pUser);
	}
}