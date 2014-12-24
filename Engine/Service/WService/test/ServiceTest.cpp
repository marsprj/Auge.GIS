#include "ServiceTest.h"
#include "AugeCore.h"
#include "AugeCarto.h"
#include "AugeData.h"
#include "AugeService.h"
#include "AugeWebCore.h"

CPPUNIT_TEST_SUITE_REGISTRATION(ServiceTest);

auge::Map* CreateMap();

void ServiceTest::setUp() 
{
	printf("setUp\n");
	auge::GLogger* pLogger = auge::augeGetLoggerInstance();
	pLogger->Initialize();

	auge::DataEngineManager* pDataEngineManager = NULL;
	pDataEngineManager = auge::augeGetDataEngineManagerInstance();
	pDataEngineManager->Load();

	auge::WebEngineManager* pWebEngineManager = NULL;
	pWebEngineManager = auge::augeGetWebEngineManagerInstance();
	pWebEngineManager->Load();

	auge::WebContext* pWebContext = NULL;
	pWebContext = auge::augeGetWebContextInstance();
	pWebContext->Initialize();

	auge::GConnection* pConnection = NULL;
	pConnection = pWebContext->GetConnection();
	CPPUNIT_ASSERT(pConnection!=NULL);
	if(!pConnection->IsOpen())
	{
		pWebContext->Unload();
		pDataEngineManager->Unload();
		return;
	}

	auge::ConnectionManager* pConManager = NULL;
	pConManager = auge::augeGetConnectionManagerInstance();
	pConManager->Initialize(pConnection);

	auge::CartoManager *pCartoManager = auge::augeGetCartoManagerInstance();
	pCartoManager->Initialize(pConnection);

	auge::ServiceManager* pServiceManager = NULL;
	pServiceManager = auge::augeGetServiceManagerInstance();
	pServiceManager->Initialize(pConnection);
}

void ServiceTest::tearDown()
{
	auge::DataEngineManager* pDataEngineManager = NULL;
	pDataEngineManager = auge::augeGetDataEngineManagerInstance();
	auge::WebEngineManager* pWebEngineManager = NULL;
	pWebEngineManager = auge::augeGetWebEngineManagerInstance();
	auge::WebContext* pWebContext = NULL;
	pWebContext = auge::augeGetWebContextInstance();
	auge::ConnectionManager* pConManager = NULL;
	pConManager = auge::augeGetConnectionManagerInstance();
	auge::CartoManager *pCartoManager = NULL;
	pCartoManager = auge::augeGetCartoManagerInstance();
	auge::ServiceManager* pServiceManager = NULL;
	pServiceManager = auge::augeGetServiceManagerInstance();

	pConManager->Unload();
	pCartoManager->Cleanup();
	pWebContext->Unload();
	pWebEngineManager->Unload();
	pDataEngineManager->Unload();

	printf("tearDown\n");
}

void ServiceTest::RegisterService()
{
	const char* name = "world";

	auge::ServiceManager* pServiceManager = NULL;
	pServiceManager = auge::augeGetServiceManagerInstance();

	RESULTCODE rc = AG_SUCCESS;
	rc = pServiceManager->Register(name);
	//CPPUNIT_ASSERT(rc==AG_SUCCESS);

	auge::Service *pService = pServiceManager->GetService(name);
	//CPPUNIT_ASSERT(pService!=NULL);

}

void ServiceTest::UnRegisterService()
{
	const char* name = "world";

	auge::ServiceManager* pServiceManager = NULL;
	pServiceManager = auge::augeGetServiceManagerInstance();

	RESULTCODE rc = AG_SUCCESS;
	rc = pServiceManager->Unregister(name);
	CPPUNIT_ASSERT(rc==AG_SUCCESS);
}

void ServiceTest::RegisterMap()
{
	const char* name = "world";
	auge::ServiceManager* pServiceManager = NULL;
	pServiceManager = auge::augeGetServiceManagerInstance();
	auge::Service *pService = pServiceManager->GetService(name);
	CPPUNIT_ASSERT(pService!=NULL);

	auge::CartoManager* pCartoManager = NULL;
	pCartoManager = auge::augeGetCartoManagerInstance();
	auge::Map *pMap = NULL;
	pMap = pCartoManager->LoadMap("world");
	CPPUNIT_ASSERT(pMap!=NULL);

	pServiceManager->RegisterMap(pService->GetID(), pMap->GetID());
	//pService->SetMap(pMap);
}

void ServiceTest::DoGetMap()
{
	int i=0;
	while(true)
	{
		printf("\r%d", i++);
		const char* request = "service=wms&version=1.3.0&request=GetMap&layers=cities,rivers,country&styles=point,line,polygon&bbox=-180,-90,180,90&width=800&height=600&transparent=true&bgcolor=#e01356";

		const char* srs_type = "wms";
		const char* svs_name = "world";
		RESULTCODE rc = AG_SUCCESS;

		auge::GError	*pError = auge::augeGetErrorInstance();
		auge::GLogger	*pLogger = auge::augeGetLoggerInstance();
		pLogger->Initialize();

		auge::DataEngineManager* pDataEngineManager = NULL;
		pDataEngineManager = auge::augeGetDataEngineManagerInstance();
		rc = pDataEngineManager->Load();
		
		auge::WebEngineManager* pWebEngineManager = NULL;
		pWebEngineManager = auge::augeGetWebEngineManagerInstance();
		rc = pWebEngineManager->Load();
		
		auge::WebContext* pWebContext = NULL;
		pWebContext = auge::augeGetWebContextInstance();
		rc = pWebContext->Initialize();
		
		auge::GConnection* pConnection = NULL;
		pConnection = pWebContext->GetConnection();
		if(!pConnection->IsOpen())
		{
			pLogger->Error(pError->GetLastError(), __FILE__, __LINE__);
			return ;
		}
		
		auge::ConnectionManager* pConManager = NULL;
		pConManager = auge::augeGetConnectionManagerInstance();
		rc = pConManager->Initialize(pConnection);
		if(rc!=AG_SUCCESS)
		{
			pWebContext->Unload();
			pWebEngineManager->Unload();
			pDataEngineManager->Unload();
			pLogger->Error(pError->GetLastError(), __FILE__, __LINE__);
			return;
		}

		auge::CartoManager *pCartoManager = auge::augeGetCartoManagerInstance();
		rc = pCartoManager->Initialize(pConnection);
		if(rc!=AG_SUCCESS)
		{
			pLogger->Error(pError->GetLastError(), __FILE__, __LINE__);
			return;
		}

		auge::ServiceManager* pServiceManager = NULL;
		pServiceManager = auge::augeGetServiceManagerInstance();
		pServiceManager->Initialize(pConnection);
		
		auge::WebEngine* pWebEngine = NULL;
		pWebEngine = pWebEngineManager->GetEngine(srs_type);
		if(pWebEngine==NULL)
		{
			const char* msg = pError->GetLastError();
			auge::WebExceptionResponse* pExResponse = auge::augeCreateWebExceptionResponse();
			pExResponse->SetMessage(msg);
			////cleanup
			return;
		}

		auge::WebRequest	*pRequest = NULL;
		auge::WebResponse	*pResponse = NULL;
		auge::WebWriter		*pWriter = auge::augeCreateWebWriter();

		pRequest = pWebEngine->ParseRequest(request);
		if(pRequest==NULL)
		{
			const char* msg = pError->GetLastError();
			auge::WebExceptionResponse* pExResponse = auge::augeCreateWebExceptionResponse();
			pExResponse->SetMessage(msg);
			pExResponse->Write(pWriter);
			AUGE_SAFE_RELEASE(pExResponse);
			AUGE_SAFE_RELEASE(pWriter);
			return;
		}

		auge::Service* pService = NULL;
		pService = pServiceManager->GetService(svs_name);
		if(pService==NULL)
		{
			const char* msg = pError->GetLastError();
			auge::WebExceptionResponse* pExResponse = auge::augeCreateWebExceptionResponse();
			pExResponse->SetMessage(msg);
			AUGE_SAFE_RELEASE(pExResponse);		
			AUGE_SAFE_RELEASE(pRequest);
			AUGE_SAFE_RELEASE(pWriter);
			return;
		}

		pService->Start();
		pResponse = pService->Execute(pRequest);
		if(pResponse!=NULL)
		{
			//pResponse->Write(pWriter);
		}

		AUGE_SAFE_RELEASE(pRequest);
		AUGE_SAFE_RELEASE(pResponse);

		pService->Shutdown();
		AUGE_SAFE_RELEASE(pWriter);
	}
	
}

void ServiceTest::DoGetMapCapabilities()
{
	const char* request = "service=wms&version=1.3.0&request=GetCapabilities";

	const char* srs_type = "wms";
	const char* svs_name = "world";
	RESULTCODE rc = AG_SUCCESS;

	auge::GError	*pError = auge::augeGetErrorInstance();
	auge::GLogger	*pLogger = auge::augeGetLoggerInstance();
	pLogger->Initialize();

	auge::DataEngineManager* pDataEngineManager = NULL;
	pDataEngineManager = auge::augeGetDataEngineManagerInstance();
	rc = pDataEngineManager->Load();

	auge::WebEngineManager* pWebEngineManager = NULL;
	pWebEngineManager = auge::augeGetWebEngineManagerInstance();
	rc = pWebEngineManager->Load();

	auge::WebContext* pWebContext = NULL;
	pWebContext = auge::augeGetWebContextInstance();
	rc = pWebContext->Initialize();

	auge::GConnection* pConnection = NULL;
	pConnection = pWebContext->GetConnection();
	if(!pConnection->IsOpen())
	{
		pLogger->Error(pError->GetLastError(), __FILE__, __LINE__);
		return ;
	}

	auge::ConnectionManager* pConManager = NULL;
	pConManager = auge::augeGetConnectionManagerInstance();
	rc = pConManager->Initialize(pConnection);
	if(rc!=AG_SUCCESS)
	{
		pWebContext->Unload();
		pWebEngineManager->Unload();
		pDataEngineManager->Unload();
		pLogger->Error(pError->GetLastError(), __FILE__, __LINE__);
		return;
	}

	auge::CartoManager *pCartoManager = auge::augeGetCartoManagerInstance();
	rc = pCartoManager->Initialize(pConnection);
	if(rc!=AG_SUCCESS)
	{
		pLogger->Error(pError->GetLastError(), __FILE__, __LINE__);
		return;
	}

	auge::ServiceManager* pServiceManager = NULL;
	pServiceManager = auge::augeGetServiceManagerInstance();
	pServiceManager->Initialize(pConnection);

	auge::WebEngine* pWebEngine = NULL;
	pWebEngine = pWebEngineManager->GetEngine(srs_type);
	if(pWebEngine==NULL)
	{
		const char* msg = pError->GetLastError();
		auge::WebExceptionResponse* pExResponse = auge::augeCreateWebExceptionResponse();
		pExResponse->SetMessage(msg);
		////cleanup
		return;
	}

	auge::WebRequest	*pRequest = NULL;
	auge::WebResponse	*pResponse = NULL;
	auge::WebWriter		*pWriter = auge::augeCreateWebWriter();

	pRequest = pWebEngine->ParseRequest(request);
	if(pRequest==NULL)
	{
		const char* msg = pError->GetLastError();
		auge::WebExceptionResponse* pExResponse = auge::augeCreateWebExceptionResponse();
		pExResponse->SetMessage(msg);
		pExResponse->Write(pWriter);
		AUGE_SAFE_RELEASE(pExResponse);
		AUGE_SAFE_RELEASE(pWriter);
		return;
	}

	auge::Service* pService = NULL;
	pService = pServiceManager->GetService(svs_name);
	if(pService==NULL)
	{
		const char* msg = pError->GetLastError();
		auge::WebExceptionResponse* pExResponse = auge::augeCreateWebExceptionResponse();
		pExResponse->SetMessage(msg);
		AUGE_SAFE_RELEASE(pExResponse);		
		AUGE_SAFE_RELEASE(pRequest);
		AUGE_SAFE_RELEASE(pWriter);
		return;
	}

	pService->Start();
	pResponse = pService->Execute(pRequest);
	if(pResponse!=NULL)
	{
		//pResponse->Write(pWriter);
	}

	AUGE_SAFE_RELEASE(pRequest);
	AUGE_SAFE_RELEASE(pResponse);

	pService->Shutdown();
	AUGE_SAFE_RELEASE(pWriter);

}

void ServiceTest::DoGet()
{

}

void ServiceTest::DoPost()
{

}

void ServiceTest::ManagerTest()
{
	const char* request = "service=wms&version=1.3.0&request=GetMap&layers=cities,rivers,country&styles=point,line,polygon&bbox=-180,-90,180,90&width=800&height=600&transparent=true";

	const char* srs_type = "wms";
	const char* svs_name = "world";

	auge::GError*	pError = NULL;
	pError = auge::augeGetErrorInstance();

	auge::GLogger*	pLogger = NULL;
	pLogger = auge::augeGetLoggerInstance();
	pLogger->Initialize();

	auge::ServiceManager* pServiceManager = NULL;
	pServiceManager = auge::augeGetServiceManagerInstance();

	auge::WebEngineManager* pWebEngineManager = NULL;
	pWebEngineManager = auge::augeGetWebEngineManagerInstance();

	auge::WebEngine* pWebEngine = NULL;
	pWebEngine = pWebEngineManager->GetEngine(srs_type);
	if(pWebEngine==NULL)
	{
		const char* msg = pError->GetLastError();
		auge::WebExceptionResponse* pExResponse = auge::augeCreateWebExceptionResponse();
		pExResponse->SetMessage(msg);
		////cleanup
		return;
	}

	auge::WebRequest	*pRequest = NULL;
	auge::WebResponse	*pResponse = NULL;
	auge::WebWriter		*pWriter = auge::augeCreateWebWriter();

	pRequest = pWebEngine->ParseRequest("");
	if(pRequest==NULL)
	{
		const char* msg = pError->GetLastError();
		auge::WebExceptionResponse* pExResponse = auge::augeCreateWebExceptionResponse();
		pExResponse->SetMessage(msg);
		pExResponse->Write(pWriter);
		AUGE_SAFE_RELEASE(pExResponse);
		AUGE_SAFE_RELEASE(pWriter);
		return;
	}

	auge::Service* pService = NULL;
	pService = pServiceManager->GetService(svs_name);
	if(pService==NULL)
	{
		const char* msg = pError->GetLastError();
		auge::WebExceptionResponse* pExResponse = auge::augeCreateWebExceptionResponse();
		pExResponse->SetMessage(msg);
		AUGE_SAFE_RELEASE(pExResponse);		
		AUGE_SAFE_RELEASE(pRequest);
		AUGE_SAFE_RELEASE(pWriter);
		return;
	}

	pResponse = pService->Execute(pRequest);
	if(pResponse!=NULL)
	{
		pResponse->Write(pWriter);
	}

	AUGE_SAFE_RELEASE(pRequest);
	AUGE_SAFE_RELEASE(pResponse);
	AUGE_SAFE_RELEASE(pService);
	AUGE_SAFE_RELEASE(pWriter);
}

auge::Map* CreateMap()
{
	auge::Map* pMap = NULL;
	auge::FeatureLayer* pFLayer = NULL;
	auge::CartoFactory* pCartoFactory = auge::augeGetCartoFactoryInstance();

	pMap = pCartoFactory->CreateMap();
	pMap->SetName("word");

	pFLayer = pCartoFactory->CreateFeatureLayer();
	pFLayer->SetName("cities");
	pMap->AddLayer(pFLayer);

	return pMap;
}
