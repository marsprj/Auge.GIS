#include "DescribeCsvHandler.h"
#include "DescribeCsvRequest.h"
#include "DescribeCsvResponse.h"
#include "AugeCarto.h"
#include "AugeService.h"
#include "AugeUser.h"

namespace auge
{
	//extern bool make_user_path(char* user_path, size_t size, const char* root_path, const char* user_name);
	extern bool make_user_file_path(char* user_path, size_t size, const char* root_path, const char* user_name);

	DescribeCsvHandler::DescribeCsvHandler()
	{

	}

	DescribeCsvHandler::~DescribeCsvHandler()
	{

	}

	const char*	DescribeCsvHandler::GetName()
	{
		return "DescribeCsv";
	}

	const char*	DescribeCsvHandler::GetDescription()
	{
		return GetName();
	}

	WebRequest*	DescribeCsvHandler::ParseRequest(rude::CGI& cgi)
	{
		DescribeCsvRequest* pRequest = new DescribeCsvRequest();

		if(!pRequest->Create(cgi))
		{
			GLogger* pLogger = augeGetLoggerInstance();
			pLogger->Error("[Request] is NULL", __FILE__, __LINE__);
			pRequest->Release();
			pRequest = NULL;
		}
		return pRequest;
	}

	WebRequest* DescribeCsvHandler::ParseRequest(rude::CGI& cgi, const char* mapName)
	{
		return ParseRequest(cgi);
	}

	WebRequest*	DescribeCsvHandler::ParseRequest(XDocument* pxDoc, const char* mapName)
	{
		return NULL;
	}

	WebResponse* DescribeCsvHandler::Execute(WebRequest* pWebRequest, User* pUser)
	{
		return NULL;

		//DescribeCsvRequest* pRequest = static_cast<DescribeCsvRequest*>(pWebRequest);
		//DescribeCsvResponse* pWebResponse = new DescribeCsvResponse(pRequest);
		//CartoManager* pCartoManager = augeGetCartoManagerInstance();

		//const char* name = pRequest->GetName();
		//if(name==NULL)
		//{
		//	EnumMap* pMaps = pCartoManager->DescribeCsvs();
		//	pWebResponse->SetMaps(pMaps);
		//}
		//else
		//{
		//	Map* pMap = pCartoManager->LoadMap(name);
		//	pWebResponse->SetMap(pMap);
		//}
		//return pWebResponse;
	}

	WebResponse* DescribeCsvHandler::Execute(WebRequest* pWebRequest, WebContext* pWebContext, User* pUser)
	{
		GError	*pError = augeGetErrorInstance();
		GLogger	*pLogger = augeGetLoggerInstance();

		DescribeCsvRequest* pRequest = static_cast<DescribeCsvRequest*>(pWebRequest);
		
		const char* root_path = pWebContext->GetUserRoot();
		const char* csv_path = pRequest->GetPath();

		if(csv_path==NULL)
		{
			const char* msg = "Parameter [path] is NULL";
			WebExceptionResponse* pExpResponse = augeCreateWebExceptionResponse();
			pExpResponse->SetMessage(msg);
			pLogger->Error(msg,__FILE__,__LINE__);

			return pExpResponse;
		}

		char user_path[AUGE_PATH_MAX];
		memset(user_path,0,AUGE_PATH_MAX);
		make_user_file_path(user_path, AUGE_PATH_MAX, root_path, pUser->GetName());

		// build csv local path
		char csv_local_path[AUGE_PATH_MAX];
		memset(csv_local_path, 0, AUGE_PATH_MAX);
		auge_make_path(csv_local_path,  NULL, user_path, csv_path+1, NULL);
		if(g_access(csv_local_path,4))
		{
			char msg[AUGE_MSG_MAX];
			g_snprintf(msg, AUGE_MSG_MAX, "File [%s] does not exist", csv_path);
			GLogger* pLogger = augeGetLoggerInstance();
			pLogger->Error(msg, __FILE__, __LINE__);
			WebExceptionResponse* pExpResponse = augeCreateWebExceptionResponse();
			pExpResponse->SetMessage(msg);
			return pExpResponse;
		}

		char csvDrv[AUGE_DRV_MAX];
		char csvDir[AUGE_PATH_MAX];
		char csvName[AUGE_NAME_MAX];
		char csvPath[AUGE_PATH_MAX];		
		memset(csvDrv, 0, AUGE_DRV_MAX);
		memset(csvDir, 0, AUGE_PATH_MAX);
		memset(csvName, 0, AUGE_NAME_MAX);
		memset(csvPath, 0, AUGE_PATH_MAX);		
		auge_split_path(csv_local_path, csvDrv, csvDir, csvName, NULL);
		char csv_local_dir[AUGE_PATH_MAX];
		memset(csv_local_dir,0,AUGE_PATH_MAX);
		auge_make_path(csv_local_dir, csvDrv, csvDir, NULL, NULL);

		char constr[AUGE_PATH_MAX];
		memset(constr,0,AUGE_PATH_MAX);
		g_snprintf(constr, AUGE_PATH_MAX, "DATABASE=%s",csv_local_dir);

		DataEngine* pDataEngine = NULL;
		DataEngineManager* pDataEngineManager = augeGetDataEngineManagerInstance();
		pDataEngine = pDataEngineManager->GetEngine("CsvFile");
		if(pDataEngine==NULL)
		{
			const char* msg = pError->GetLastError();
			pLogger->Error(msg,__FILE__,__LINE__);
			WebExceptionResponse* pExpResponse = augeCreateWebExceptionResponse();
			pExpResponse->SetMessage(msg);
			return pExpResponse;
		}

		RESULTCODE rc = AG_FAILURE;
		Workspace* pcsvWorkspace = pDataEngine->CreateWorkspace();
		pcsvWorkspace->SetConnectionString(constr);
		rc = pcsvWorkspace->Open();
		if(rc!=AG_SUCCESS)
		{
			pcsvWorkspace->Release();

			const char* msg = pError->GetLastError();
			pLogger->Error(msg,__FILE__,__LINE__);
			WebExceptionResponse* pExpResponse = augeCreateWebExceptionResponse();
			pExpResponse->SetMessage(msg);
			return pExpResponse;
		}

		AttributeDataSet* pcsvDataset = NULL;
		pcsvDataset = static_cast<AttributeDataSet*>(pcsvWorkspace->OpenDataSet(csvName));

		DescribeCsvResponse* pResponse = new DescribeCsvResponse(pRequest);
		pResponse->SetDataset(pcsvDataset);
		return pResponse;
	}
}