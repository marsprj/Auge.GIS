#include "RemoveFileHandler.h"
#include "RemoveFileRequest.h"
#include "AugeUser.h"

#include "AugeWebCore.h"

#ifndef WIN32
#include <sys/types.h>
#include <dirent.h>
#endif

namespace auge
{
	//extern bool make_user_path(char* user_path, size_t size, const char* root_path, const char* user_name);
	extern bool make_user_file_path(char* user_path, size_t size, const char* root_path, const char* user_name);

	RemoveFileHandler::RemoveFileHandler()
	{

	}

	RemoveFileHandler::~RemoveFileHandler()
	{

	}

	const char*	RemoveFileHandler::GetName()
	{
		return "RemoveFile";
	}

	const char*	RemoveFileHandler::GetDescription()
	{
		return GetName();
	}

	WebRequest*	RemoveFileHandler::ParseRequest(rude::CGI& cgi)
	{
		RemoveFileRequest* pRequest = new RemoveFileRequest();

		if(!pRequest->Create(cgi))
		{
			GLogger* pLogger = augeGetLoggerInstance();
			pLogger->Error("[Request] is NULL", __FILE__, __LINE__);
			pRequest->Release();
			pRequest = NULL;
		}
		return pRequest;
	}

	WebRequest*	RemoveFileHandler::ParseRequest(rude::CGI& cgi,const char* mapName)
	{
		return ParseRequest(cgi);
	}

	WebRequest*	RemoveFileHandler::ParseRequest(XDocument* pxDoc,const char* mapName)
	{
		return NULL;
	}

	WebResponse* RemoveFileHandler::Execute(WebRequest* pWebRequest, User* pUser)
	{
		RemoveFileRequest* pRequest = static_cast<RemoveFileRequest*>(pWebRequest);
		WebResponse* pWebResponse = NULL;
		return pWebResponse;
	}

	WebResponse* RemoveFileHandler::Execute(WebRequest* pWebRequest, WebContext* pWebContext, User* pUser)
	{
		GLogger* pLogger = augeGetLoggerInstance();
		RemoveFileRequest* pRequest = static_cast<RemoveFileRequest*>(pWebRequest);

		const char* root_path = pWebContext->GetUserRoot();
		const char* rqut_path = pRequest->GetPath();
		if(rqut_path==NULL)
		{
			const char* msg = "Parameter [Path] is NULL";
			WebExceptionResponse* pExpResponse = augeCreateWebExceptionResponse();
			pExpResponse->SetMessage(msg);
			pLogger->Error(msg,__FILE__,__LINE__);

			return pExpResponse;
		}

		char user_path[AUGE_PATH_MAX];
		memset(user_path,0,AUGE_PATH_MAX);
		make_user_file_path(user_path, AUGE_PATH_MAX, root_path, pUser->GetName());

		char local_path[AUGE_PATH_MAX];
		memset(local_path,0,AUGE_PATH_MAX);
		auge_make_path(local_path,NULL,user_path,rqut_path+1,NULL);

		if(g_access(local_path,4))
		{
			char msg[AUGE_MSG_MAX];
			memset(msg,0,AUGE_MSG_MAX);
			g_sprintf(msg,"Path [%s] does not existed.", rqut_path);
			WebExceptionResponse* pExpResponse = augeCreateWebExceptionResponse();
			pExpResponse->SetMessage(msg);
			pLogger->Error(msg,__FILE__,__LINE__);

			return pExpResponse;
		}

		int ret = auge_remove_file(local_path);
		if(ret)
		{
			char msg[AUGE_MSG_MAX];
			memset(msg,0,AUGE_MSG_MAX);
			g_sprintf(msg,"Failed to remove file [%s].", rqut_path);
			WebExceptionResponse* pExpResponse = augeCreateWebExceptionResponse();
			pExpResponse->SetMessage(msg);
			pLogger->Error(msg,__FILE__,__LINE__);

			return pExpResponse;
		}

		WebSuccessResponse* pSusResponse = augeCreateWebSuccessResponse();
		pSusResponse->SetRequest(pRequest->GetRequest());
		return pSusResponse;
	}
}