#include "DescribeServiceHandler.h"
#include "DescribeServiceRequest.h"
#include "DescribeServiceResponse.h"


namespace auge
{
	DescribeServiceHandler::DescribeServiceHandler()
	{

	}

	DescribeServiceHandler::~DescribeServiceHandler()
	{

	}

	const char*	DescribeServiceHandler::GetName()
	{
		return "DescribeService";
	}

	const char*	DescribeServiceHandler::GetDescription()
	{
		return GetName();
	}

	WebRequest*	DescribeServiceHandler::ParseRequest(rude::CGI& cgi)
	{
		DescribeServiceRequest* pRequest = new DescribeServiceRequest();

		if(!pRequest->Create(cgi))
		{
			GLogger* pLogger = augeGetLoggerInstance();
			pLogger->Error("[Request] is NULL", __FILE__, __LINE__);
			pRequest->Release();
			pRequest = NULL;
		}
		return pRequest;
	}

	WebRequest*	DescribeServiceHandler::ParseRequest(rude::CGI& cgi,const char* mapName)
	{
		return ParseRequest(cgi);
	}

	WebRequest*	DescribeServiceHandler::ParseRequest(XDocument* pxDoc, const char* mapName)
	{
		return NULL;
	}

	WebResponse* DescribeServiceHandler::Execute(WebRequest* pWebRequest, User* pUser)
	{
		DescribeServiceResponse* pResponse = NULL;
		pResponse = new DescribeServiceResponse(static_cast<DescribeServiceRequest*>(pWebRequest));
		pResponse->SetUser(pUser);
		return pResponse;
	}

	WebResponse* DescribeServiceHandler::Execute(WebRequest* pWebRequest, WebContext* pWebContext, User* pUser)
	{
		DescribeServiceResponse* pResponse = NULL;
		pResponse = new DescribeServiceResponse(static_cast<DescribeServiceRequest*>(pWebRequest));
		pResponse->SetUser(pUser);
		return pResponse;
	}
}