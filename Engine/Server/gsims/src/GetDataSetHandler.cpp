#include "GetDataSetHandler.h"
#include "GetDataSetRequest.h"
#include "GetDataSetResponse.h"


namespace auge
{
	GetDataSetHandler::GetDataSetHandler()
	{

	}

	GetDataSetHandler::~GetDataSetHandler()
	{

	}

	const char*	GetDataSetHandler::GetName()
	{
		return "GetDataSet";
	}

	WebRequest*	GetDataSetHandler::ParseRequest(rude::CGI& cgi, WebContext* pWebContext/*=NULL*/, Map* pMap/*=NULL*/)
	{
		GetDataSetRequest* pRequest = new GetDataSetRequest();

		if(!pRequest->Create(cgi))
		{
			GLogger* pLogger = augeGetLoggerInstance();
			pLogger->Error("[Request] is NULL", __FILE__, __LINE__);
			pRequest->Release();
			pRequest = NULL;
		}
		return pRequest;
	}

	WebRequest*	GetDataSetHandler::ParseRequest(XDocument* pxDoc, WebContext* pWebContext/*=NULL*/, Map* pMap/*=NULL*/)
	{
		return NULL;
	}

	WebResponse* GetDataSetHandler::Execute(WebRequest* pWebRequest)
	{
		char path[AUGE_PATH_MAX] = {0};
		char cdir[AUGE_PATH_MAX] = {0};
		auge_get_cwd(cdir, AUGE_PATH_MAX);

#ifdef WIN32
		//char pdir[AUGE_PATH_MAX] = {0};
		//auge_get_parent_dir(cdir, pdir, AUGE_PATH_MAX);
		//auge_make_path(path, NULL, pdir, "conf\\DataEngine", "xml");
		auge_make_path(path, NULL, cdir, "conf\\font", "xml");
#else
		char pdir[AUGE_PATH_MAX] = {0};
		auge_get_parent_dir(cdir, pdir, AUGE_PATH_MAX);
		auge_make_path(path, NULL, pdir, "conf/font", "xml");
#endif

		GetDataSetResponse* pResponse = new GetDataSetResponse(static_cast<GetDataSetRequest*>(pWebRequest));
		pResponse->SetPath(path);

		return pResponse;
	}

	WebResponse* GetDataSetHandler::Execute(WebRequest* pWebRequest, WebContext* pWebContext, Map* pMap)
	{
		return NULL;
	}
}