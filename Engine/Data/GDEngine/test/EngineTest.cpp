#include "EngineTest.h"
#include "AugeData.h"
#include "AugeCore.h"

CPPUNIT_TEST_SUITE_REGISTRATION(EngineTest);

void EngineTest::setUp() 
{
	printf("setUp\\n");
	auge::GLogger* pLogger = auge::augeGetLoggerInstance();
	pLogger->Initialize();

	auge::DataEngine* pEngine = NULL;
	auge::DataEngineManager* pEngineManager = NULL;
	pEngineManager = auge::augeGetDataEngineManagerInstance();
	pEngineManager->Load();

	const char* constr = "SERVER=127.0.0.1;INSTANCE=5432;DATABASE=auge;USER=postgres;PASSWORD=qwer1234;ENCODING=GBK";
	pEngine = pEngineManager->GetEngine("Postgres");
	m_pConnection = pEngine->CreateConnection();
	m_pConnection->SetConnectionString(constr);
	m_pConnection->Open();

	auge::ConnectionManager* pConnManager = NULL;
	pConnManager = auge::augeGetConnectionManagerInstance();
	pConnManager->Initialize(m_pConnection);
}

void EngineTest::tearDown()
{
	auge::ConnectionManager* pConnManager = NULL;
	pConnManager = auge::augeGetConnectionManagerInstance();
	pConnManager->Unload();

	auge::DataEngineManager* pEngineManager = NULL;
	pEngineManager = auge::augeGetDataEngineManagerInstance();
	pEngineManager->Unload();

	m_pConnection->Release();
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
	auge::ConnectionManager* pConnManager = NULL;
	pConnManager = auge::augeGetConnectionManagerInstance();

	const char* constr = "SERVER=127.0.0.1;INSTANCE=5432;DATABASE=gisdb;USER=postgres;PASSWORD=qwer1234;ENCODING=GBK";
	pConnManager->Register("db1", "Postgres", constr);

	auge::Workspace* pWorkspace = NULL;
	pWorkspace = pConnManager->GetWorkspace("db1");

}

void EngineTest::ConnectionTest_2()
{
	const char* name = "test";
	//const char* constr = "SERVER=127.0.0.1;INSTANCE=5432;DATABASE=gisdb;USER=postgres;PASSWORD=qwer1234;ENCODING=GBK";
	const char* constr = "SERVER=123.57.207.198;INSTANCE=5432;DATABASE=gisdb;USER=postgres;PASSWORD=qwer1234;ENCODING=GBK";

	auge::ConnectionManager* pConnManager = NULL;
	pConnManager = auge::augeGetConnectionManagerInstance();
	
	auge::Workspace* pWorkspace = NULL;
	pWorkspace = pConnManager->GetWorkspace(name);
	CPPUNIT_ASSERT(pWorkspace!=NULL);

	pConnManager->Update(pWorkspace->GetID(), name, "Postgres", constr);

	pWorkspace = pConnManager->GetWorkspace(name);
	CPPUNIT_ASSERT(pWorkspace!=NULL);
}