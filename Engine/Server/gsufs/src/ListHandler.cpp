#include "ListHandler.h"
#include "ListRequest.h"
#include "ListResponse.h"

#include "AugeWebCore.h"

namespace auge
{
	ListHandler::ListHandler()
	{

	}

	ListHandler::~ListHandler()
	{

	}

	const char*	ListHandler::GetName()
	{
		return "List";
	}

	WebRequest*	ListHandler::ParseRequest(rude::CGI& cgi)
	{
		ListRequest* pRequest = new ListRequest();

		if(!pRequest->Create(cgi))
		{
			GLogger* pLogger = augeGetLoggerInstance();
			pLogger->Error("[Request] is NULL", __FILE__, __LINE__);
			pRequest->Release();
			pRequest = NULL;
		}
		return pRequest;
	}

	WebRequest*	ListHandler::ParseRequest(rude::CGI& cgi,const char* mapName)
	{
		return ParseRequest(cgi);
	}

	WebRequest*	ListHandler::ParseRequest(XDocument* pxDoc,const char* mapName)
	{
		return NULL;
	}

	WebResponse* ListHandler::Execute(WebRequest* pWebRequest)
	{
		ListRequest* pRequest = static_cast<ListRequest*>(pWebRequest);
		WebResponse* pWebResponse = NULL;

		/*const char* sourceName = pRequest->GetSourceName();
		const char* dataSetName= pRequest->GetDataSetName();

		if(sourceName==NULL)
		{
			const char* msg = "Parameter [sourceName] is NULL";
			GLogger* pLogger = augeGetLoggerInstance();
			pLogger->Error(msg, __FILE__, __LINE__);
			WebExceptionResponse* pExpResponse = augeCreateWebExceptionResponse();
			pExpResponse->SetMessage(msg);
			return pExpResponse;
		}

		Workspace* pWorkspace = NULL;
		ConnectionManager* pConnectionManager = augeGetConnectionManagerInstance();
		pWorkspace = pConnectionManager->GetWorkspace(sourceName);
		if( pWorkspace==NULL )
		{
			char msg[AUGE_PATH_MAX] = {0};
			g_sprintf(msg, "Cannot load datasource [%s].", sourceName);
			GLogger* pLogger = augeGetLoggerInstance();
			pLogger->Error(msg, __FILE__, __LINE__);
			WebExceptionResponse* pExpResponse = augeCreateWebExceptionResponse();
			pExpResponse->SetMessage(msg);
			return pExpResponse;
		}

		DataSet* pDataSet = pWorkspace->OpenDataSet(dataSetName);
		if(pDataSet==NULL)
		{
			char msg[AUGE_PATH_MAX] = {0};
			g_sprintf(msg, "Cannot load data set [%s].", dataSetName);
			GLogger* pLogger = augeGetLoggerInstance();
			pLogger->Error(msg, __FILE__, __LINE__);
			WebExceptionResponse* pExpResponse = augeCreateWebExceptionResponse();
			pExpResponse->SetMessage(msg);
			return pExpResponse;
		}

		
		switch(pDataSet->GetType())
		{
		case augeDataSetFeature:
			pWebResponse = DrawFeature(static_cast<FeatureClass*>(pDataSet),pRequest);
			break;
		case augeDataSetRaster:
			break;
		}*/

		return pWebResponse;
	}

	WebResponse* ListHandler::Execute(WebRequest* pWebRequest, WebContext* pWebContext)
	{
		GLogger* pLogger = augeGetLoggerInstance();
		ListRequest* pRequest = static_cast<ListRequest*>(pWebRequest);

		const char* root_path = pWebContext->GetUploadPath();
		const char* rqut_path = pRequest->GetPath();
		if(rqut_path==NULL)
		{
			const char* msg = "Parameter [Path] is NULL";
			WebExceptionResponse* pExpResponse = augeCreateWebExceptionResponse();
			pExpResponse->SetMessage(msg);
			pLogger->Error(msg,__FILE__,__LINE__);

			return pExpResponse;
		}

		char local_path[AUGE_PATH_MAX];
		memset(local_path,0,AUGE_PATH_MAX);
		auge_make_path(local_path,NULL,root_path,rqut_path+1,NULL);

		if(g_access(local_path,4))
		{
			char msg[AUGE_MSG_MAX];
			memset(msg,0,AUGE_MSG_MAX);
			g_sprintf(msg,"Path [%s] does not exist.", rqut_path);
			WebExceptionResponse* pExpResponse = augeCreateWebExceptionResponse();
			pExpResponse->SetMessage(msg);
			pLogger->Error(msg,__FILE__,__LINE__);

			return pExpResponse;
		}

		ListResponse* pListResponse = new ListResponse(pRequest);

#ifdef WIN32
		
		HANDLE hFind = NULL;
		WIN32_FIND_DATAA wfd;

		char filter[AUGE_PATH_MAX];
		auge_make_path(filter,NULL,local_path,"*",NULL);

		hFind = ::FindFirstFile(filter, &wfd);
		if(hFind == INVALID_HANDLE_VALUE)
		{
			::FindClose(hFind);
			return NULL;
		}

		while(::FindNextFile(hFind, &wfd)==TRUE)
		{
			if(wfd.cFileName[0]=='.')
			{
				continue;
			}
			if (wfd.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY)
			{
				pListResponse->AddFile(wfd.cFileName,true);
			}
			else
			{
				pListResponse->AddFile(wfd.cFileName,false);
			}
		}
		::FindClose(hFind);
#else
#endif

		return pListResponse;
	}
}