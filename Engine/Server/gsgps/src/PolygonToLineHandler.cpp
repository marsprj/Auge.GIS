#include "PolygonToLineHandler.h"
#include "PolygonToLineRequest.h"
#include "GProcessEngine.h"
#include "AugeXML.h"
#include "AugeWebCore.h"
#include "AugeProcessor.h"
#include "AugeUser.h"

namespace auge
{
	PolygonToLineHandler::PolygonToLineHandler()
	{

	}

	PolygonToLineHandler::~PolygonToLineHandler()
	{

	}

	const char*	PolygonToLineHandler::GetName()
	{
		return "PolygonToLine";
	}

	const char*	PolygonToLineHandler::GetDescription()
	{
		return "多边形转线";
	}

	WebRequest*	PolygonToLineHandler::ParseRequest(rude::CGI& cgi)
	{
		PolygonToLineRequest* pRequest = new PolygonToLineRequest();
		if(!pRequest->Create(cgi))
		{
			GLogger* pLogger = augeGetLoggerInstance();
			pLogger->Error("[Request] is NULL", __FILE__, __LINE__);
			pRequest->Release();
			pRequest = NULL;
		}
		pRequest->Info();
		return pRequest;
	}

	WebRequest*	PolygonToLineHandler::ParseRequest(rude::CGI& cgi, const char* mapName)
	{
		return ParseRequest(cgi);
	}

	WebRequest*	PolygonToLineHandler::ParseRequest(XDocument* pxDoc, const char* mapName)
	{
		PolygonToLineRequest* pRequest = new PolygonToLineRequest();
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
		pRequest->Info();
		return pRequest;
	}

	WebResponse* PolygonToLineHandler::Execute(WebRequest* pWebRequest, User* pUser)
	{
		WebResponse* pWebResponse = NULL;
		PolygonToLineRequest* pRequest = static_cast<PolygonToLineRequest*>(pWebRequest);

		const char* version = pRequest->GetVersion();
		if(strcmp(version,"1.0.0")==0)
		{

		}
		else if(strcmp(version,"1.3.0")==0)
		{

		}
		else
		{
			char msg[AUGE_MSG_MAX];
			WebExceptionResponse* pExpResponse = augeCreateWebExceptionResponse();
			g_sprintf(msg, "WPS does not support %s",version);
			pExpResponse->SetMessage(msg);
			pWebResponse = pExpResponse;
		}

		return pWebResponse;
	}

	WebResponse* PolygonToLineHandler::Execute(WebRequest* pWebRequest, WebContext* pWebContext, User* pUser)
	{
		PolygonToLineRequest* pRequest = static_cast<PolygonToLineRequest*>(pWebRequest);

		const char* input_source_name = pRequest->GetInputSourceName();
		const char* input_type_name	  = pRequest->GetInputTypeName();
		const char* output_source_name= pRequest->GetOutputSourceName();
		const char* output_type_name  = pRequest->GetOutputTypeName();
		
		auge::PolygonToLineProcessor* pProcessor = NULL;
		auge::GProcessorFactory* pFactory = auge::augeGetGeoProcessorFactoryInstance();
		pProcessor = pFactory->CreatePolygonToLineProcessor();

		pProcessor->SetUser(pUser->GetID());
		pProcessor->SetInputDataSource(input_source_name);
		pProcessor->SetInputFeatureClass(input_type_name);

		pProcessor->SetOutputDataSource(output_source_name);
		pProcessor->SetOutputFeatureClass(output_type_name);

		RESULTCODE rc = pProcessor->Execute();
		pProcessor->Release();

		if(rc != AG_SUCCESS)
		{
			GError* pError = augeGetErrorInstance();
			augeGetLoggerInstance()->Error(pError->GetLastError(),__FILE__, __LINE__);
			WebExceptionResponse* pExpResponse = augeCreateWebExceptionResponse();
			pExpResponse->SetMessage(pError->GetLastError());
			return pExpResponse;
		}

		WebSuccessResponse* pSusResponse = augeCreateWebSuccessResponse();
		pSusResponse->SetRequest(pRequest->GetRequest());

		return pSusResponse;
	}
}
