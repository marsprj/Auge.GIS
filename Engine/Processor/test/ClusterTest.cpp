#include "ClusterTest.h"
#include "AugeCore.h"
#include "AugeData.h"
#include "AugeCarto.h"
#include "AugeProcessor.h"
#include <iostream>

//CPPUNIT_TEST_SUITE_REGISTRATION(ClusterTest);

void ClusterTest::setUp() 
{
	auge::GLogger	*pLogger = auge::augeGetLoggerInstance();
	pLogger->Initialize();

	auge::DataEngine	*pEngine = NULL;
	auge::DataEngineManager* pEngineManager = NULL;
	pEngineManager = auge::augeGetDataEngineManagerInstance();
	pEngineManager->Load();

	pEngine = pEngineManager->GetEngine("Postgres");
	m_pConnection = pEngine->CreateConnection();
	//m_pConnection->SetConnectionString("SERVER=127.0.0.1;INSTANCE=5432;DATABASE=auge;USER=postgres;PASSWORD=qwer1234;ENCODING=GBK");
	m_pConnection->SetConnectionString("SERVER=192.168.111.160;INSTANCE=5432;DATABASE=auge;USER=postgres;PASSWORD=qwer1234;ENCODING=GBK");
	m_pConnection->Open();

	auge::ConnectionManager* pConnManager = NULL;
	pConnManager = auge::augeGetConnectionManagerInstance();
	pConnManager->Initialize(m_pConnection);

	printf("setUp\n");
}

void ClusterTest::tearDown()
{
	auge::ConnectionManager* pConnManager = NULL;
	pConnManager = auge::augeGetConnectionManagerInstance();
	pConnManager->Unload();

	auge::DataEngineManager* pEngineManager = NULL;
	pEngineManager = auge::augeGetDataEngineManagerInstance();
	pEngineManager->Unload();

	//m_pConnection->Close();
	//m_pConnection->Release();
	m_pConnection = NULL;
	printf("tearDown\n");
}

void ClusterTest::KMeanTest()
{	
	DWORD ts = GetTickCount();

	auge::KMeansProcessor* pProcessor = NULL;
	auge::GProcessorFactory* pFactory = auge::augeGetGeoProcessorFactoryInstance();
	pProcessor = pFactory->CreateKMeansProcessor();

	pProcessor->SetInputDataSource("test");
	pProcessor->SetInputFeatureClass("cities");

	pProcessor->SetOutputDataSource("test");
	pProcessor->SetOutputFeatureClass("cities_cluster");

	pProcessor->SetK(10);

	RESULTCODE rc = pProcessor->Execute();

	pProcessor->Release();
	
	DWORD te = GetTickCount();
	printf("[时间]:%d毫秒\n", te-ts);
}

void ClusterTest::DBScanTest()
{
	DWORD ts = GetTickCount();

	auge::DBScanProcessor* pProcessor = NULL;
	auge::GProcessorFactory* pFactory = auge::augeGetGeoProcessorFactoryInstance();
	pProcessor = pFactory->CreateDBScanProcessor();

	pProcessor->SetInputDataSource("test");
	pProcessor->SetInputFeatureClass("cities");

	pProcessor->SetOutputDataSource("test");
	pProcessor->SetOutputFeatureClass("cities_cluster");

	pProcessor->SetEsplon(5.0f);
	pProcessor->SetMinPoints(5);

	RESULTCODE rc = pProcessor->Execute();

	pProcessor->Release();

	DWORD te = GetTickCount();
	printf("[时间]:%d毫秒\n", te-ts);
}