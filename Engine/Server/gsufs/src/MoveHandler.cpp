#include "MoveHandler.h"
#include "MoveRequest.h"
#include "AugeUser.h"

#include "AugeWebCore.h"


namespace auge
{
	//extern bool make_user_path(char* user_path, size_t size, const char* root_path, const char* user_name);
	extern bool make_user_file_path(char* user_path, size_t size, const char* root_path, const char* user_name);

	MoveHandler::MoveHandler()
	{

	}

	MoveHandler::~MoveHandler()
	{

	}

	const char*	MoveHandler::GetName()
	{
		return "Move";
	}

	const char*	MoveHandler::GetDescription()
	{
		return GetName();
	}

	WebRequest*	MoveHandler::ParseRequest(rude::CGI& cgi)
	{
		MoveRequest* pRequest = new MoveRequest();

		if(!pRequest->Create(cgi))
		{
			GLogger* pLogger = augeGetLoggerInstance();
			pLogger->Error("[Request] is NULL", __FILE__, __LINE__);
			pRequest->Release();
			pRequest = NULL;
		}
		return pRequest;
	}

	WebRequest*	MoveHandler::ParseRequest(rude::CGI& cgi,const char* mapName)
	{
		return ParseRequest(cgi);
	}

	WebRequest*	MoveHandler::ParseRequest(XDocument* pxDoc,const char* mapName)
	{
		return NULL;
	}

	WebResponse* MoveHandler::Execute(WebRequest* pWebRequest, User* pUser)
	{
		MoveRequest* pRequest = static_cast<MoveRequest*>(pWebRequest);
		WebResponse* pWebResponse = NULL;
		return pWebResponse;
	}

	WebResponse* MoveHandler::Execute(WebRequest* pWebRequest, WebContext* pWebContext, User* pUser)
	{
		GLogger* pLogger = augeGetLoggerInstance();
		MoveRequest* pRequest = static_cast<MoveRequest*>(pWebRequest);

		const char* root_path = pWebContext->GetUserRoot();
		const char* rqut_src_path = pRequest->GetSrc();
		const char* rqut_des_path = pRequest->GetDes();

		if(rqut_src_path==NULL)
		{
			const char* msg = "Parameter [src] is NULL";
			WebExceptionResponse* pExpResponse = augeCreateWebExceptionResponse();
			pExpResponse->SetMessage(msg);
			pLogger->Error(msg,__FILE__,__LINE__);

			return pExpResponse;
		}

		if(rqut_des_path==NULL)
		{
			const char* msg = "Parameter [des] is NULL";
			WebExceptionResponse* pExpResponse = augeCreateWebExceptionResponse();
			pExpResponse->SetMessage(msg);
			pLogger->Error(msg,__FILE__,__LINE__);

			return pExpResponse;
		}

		char user_path[AUGE_PATH_MAX];
		memset(user_path,0,AUGE_PATH_MAX);
		make_user_file_path(user_path, AUGE_PATH_MAX, root_path, pUser->GetName());
		
		char local_src_path[AUGE_PATH_MAX];
		memset(local_src_path,0,AUGE_PATH_MAX);
		auge_make_path(local_src_path,NULL,user_path,rqut_src_path+1,NULL);

		char local_des_path[AUGE_PATH_MAX];
		memset(local_des_path,0,AUGE_PATH_MAX);
		auge_make_path(local_des_path,NULL,user_path,rqut_des_path+1,NULL);

		if(g_access(local_src_path,4))
		{
			char msg[AUGE_MSG_MAX];
			memset(msg,0,AUGE_MSG_MAX);
			g_sprintf(msg,"src [%s] does not existed.", rqut_src_path);
			WebExceptionResponse* pExpResponse = augeCreateWebExceptionResponse();
			pExpResponse->SetMessage(msg);
			pLogger->Error(msg,__FILE__,__LINE__);

			return pExpResponse;
		}

		if(!g_access(local_des_path,4))
		{
			char msg[AUGE_MSG_MAX];
			memset(msg,0,AUGE_MSG_MAX);
			g_sprintf(msg,"des [%s] does not existed.", rqut_des_path);
			WebExceptionResponse* pExpResponse = augeCreateWebExceptionResponse();
			pExpResponse->SetMessage(msg);
			pLogger->Error(msg,__FILE__,__LINE__);

			return pExpResponse;
		}

		int ret = auge_rename(local_src_path, local_des_path);
		if(ret)
		{
			char msg[AUGE_MSG_MAX];
			memset(msg,0,AUGE_MSG_MAX);
			g_sprintf(msg,"Failed to remove file [%s].", rqut_src_path);
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