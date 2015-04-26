#include "GetFontHandler.h"
#include "GetFontRequest.h"
#include "GetFontResponse.h"


namespace auge
{
	GetFontHandler::GetFontHandler()
	{

	}

	GetFontHandler::~GetFontHandler()
	{

	}

	const char*	GetFontHandler::GetName()
	{
		return "GetFont";
	}

	WebRequest*	GetFontHandler::ParseRequest(rude::CGI& cgi)
	{
		GetFontRequest* pRequest = new GetFontRequest();

		if(!pRequest->Create(cgi))
		{
			GLogger* pLogger = augeGetLoggerInstance();
			pLogger->Error("[Request] is NULL", __FILE__, __LINE__);
			pRequest->Release();
			pRequest = NULL;
		}
		return pRequest;
	}

	WebRequest* GetFontHandler::ParseRequest(rude::CGI& cgi, const char* mapName)
	{
		return ParseRequest(cgi);
	}

	WebRequest*	GetFontHandler::ParseRequest(XDocument* pxDoc, const char* mapName)
	{
		return NULL;
	}

	WebResponse* GetFontHandler::Execute(WebRequest* pWebRequest)
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

		GetFontResponse* pResponse = new GetFontResponse(static_cast<GetFontRequest*>(pWebRequest));
		pResponse->SetPath(path);

		return pResponse;
	}

	WebResponse* GetFontHandler::Execute(WebRequest* pWebRequest, WebContext* pWebContext)
	{
		return Execute(pWebRequest);
	}
}