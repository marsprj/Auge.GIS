#include "EngineTest.h"
#include "AugeData.h"
#include "AugeCore.h"

CPPUNIT_TEST_SUITE_REGISTRATION(EngineTest);

void EngineTest::setUp() 
{
	printf("setUp\\n");
	auge::GLogger* pLogger = auge::augeGetLoggerInstance();
	pLogger->Initialize();
}

void EngineTest::tearDown()
{
	printf("tearDown\\n");
}

void EngineTest::RegistrTest()
{
	const char* path = "E:\\Research\\Auge.GIS\\Dist\\32_x86_win_vc10\\binD\\GShapeD.dll";
	
	auge::DataEngine* pEngine = NULL;
	auge::DataEngineManager* pEngineManager = NULL;
	pEngineManager = auge::augeGetDataEngineManagerInstance();

	pEngineManager->Load();

	pEngine = pEngineManager->Register(path);
	CPPUNIT_ASSERT(pEngineManager!=NULL);

	pEngineManager->Unload();
}

void EngineTest::ConnectionTest()
{
	const char* constr = "SERVER=127.0.0.1;INSTANCE=5432;DATABASE=gisdb;USER=postgres;PASSWORD=qwer1234;ENCODING=GBK";

	auge::DataEngine* pEngine = NULL;
	auge::DataEngineManager* pEngineManager = NULL;
	pEngineManager = auge::augeGetDataEngineManagerInstance();
	pEngineManager->Load();

	auge::GConnection* pConnection = NULL;
	pEngine = pEngineManager->GetEngine("Postgres");
	pConnection = pEngine->CreateConnection();
	pConnection->SetConnectionString(constr);
	pConnection->Open();

	auge::ConnectionManager* pConnManager = NULL;
	pConnManager = auge::augeGetConnectionManagerInstance();
	pConnManager->Initialize(pConnection);

	pConnManager->Register("db1", "Postgres", constr);

	auge::Workspace* pWorkspace = NULL;
	pWorkspace = pConnManager->GetWorkspace("db1");

	pConnection->Close();
	pConnection->Release();

}
