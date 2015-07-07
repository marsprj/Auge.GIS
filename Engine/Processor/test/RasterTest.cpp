#include "RasterTest.h"
#include "AugeCore.h"
#include "AugeData.h"
#include "AugeCarto.h"
#include "AugeGeometry.h"
#include "AugeProcessor.h"
#include <iostream>

CPPUNIT_TEST_SUITE_REGISTRATION(RasterTest);

void RasterTest::setUp() 
{
	auge::GLogger	*pLogger = auge::augeGetLoggerInstance();
	pLogger->Initialize();

	auge::DataEngine	*pEngine = NULL;
	auge::DataEngineManager* pEngineManager = NULL;
	pEngineManager = auge::augeGetDataEngineManagerInstance();
	pEngineManager->Load();

	pEngine = pEngineManager->GetEngine("Postgres");
	m_pConnection = pEngine->CreateConnection();
	m_pConnection->SetConnectionString("SERVER=127.0.0.1;INSTANCE=5432;DATABASE=auge;USER=postgres;PASSWORD=qwer1234;ENCODING=GBK");
	m_pConnection->Open();

	auge::ConnectionManager* pConnManager = NULL;
	pConnManager = auge::augeGetConnectionManagerInstance();
	pConnManager->Initialize(m_pConnection);

	printf("setUp\n");
}

void RasterTest::tearDown()
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

void RasterTest::ExtractByRectangle()
{	
	DWORD ts = GetTickCount();

	auge::RasterExtractByRectangleProcessor* pProcessor = NULL;
	auge::GProcessorFactory* pFactory = auge::augeGetGeoProcessorFactoryInstance();
	pProcessor = pFactory->CreateRasterExtractByRectangleProcessor();

	pProcessor->SetInputDataSource("rsdb");
	pProcessor->SetInputRaster("Koala.png");

	pProcessor->SetOutputDataSource("rsdb");
	pProcessor->SetOutputRaster("Koala_clipped.png");

	auge::GEnvelope extent(100,100,500,500);
	pProcessor->SetInputRectangle(extent);

	RESULTCODE rc = pProcessor->Execute();

	pProcessor->Release();
	
	DWORD te = GetTickCount();
	printf("[ʱ��]:%d����\n", te-ts);
}

void RasterTest::Stretch()
{
	DWORD ts = GetTickCount();

	auge::RasterStretchProcessor* pProcessor = NULL;
	auge::GProcessorFactory* pFactory = auge::augeGetGeoProcessorFactoryInstance();
	pProcessor = pFactory->CreateRasterStretchProcessor();

	pProcessor->SetInputDataSource("rsdb");
	pProcessor->SetInputRaster("srtm_58_05.tif");

	pProcessor->SetOutputDataSource("rsdb");
	pProcessor->SetOutputRaster("srtm_58_05.png");

	auge::GColor start_color(0,255,0,255);
	pProcessor->SetStartColor(start_color);
	auge::GColor end_color(255,0,0,255);
	pProcessor->SetEndColor(end_color);

	RESULTCODE rc = pProcessor->Execute();

	pProcessor->Release();

	DWORD te = GetTickCount();
	printf("[ʱ��]:%d����\n", te-ts);
}