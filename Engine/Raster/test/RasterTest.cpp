#include "RasterTest.h"
#include "AugeCore.h"
#include "AugeData.h"
#include "AugeCarto.h"
#include "AugeGeometry.h"
#include "AugeProcessor.h"
#include <iostream>

//CPPUNIT_TEST_SUITE_REGISTRATION(RasterTest);

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

void RasterTest::Read_png()
{
	const char* path = "E:\\Research\\Auge.GIS\\Dist\\32_x86_win_vc10\\binD\\upload\\Koala.png";

	auge::Raster* pRaster = NULL;
	auge::RasterIO* io = auge::augeGetRasterIOInstance();
	pRaster = io->Read(path);

	pRaster->Release();
}

void RasterTest::Read_tif()
{
	const char* path = "E:\\Research\\Auge.GIS\\Dist\\32_x86_win_vc10\\binD\\upload\\world.tif";

	auge::Raster* pRaster = NULL;
	auge::RasterIO* io = auge::augeGetRasterIOInstance();
	pRaster = io->Read(path);

	pRaster->Release();
}

void RasterTest::Create_Raster()
{
	const char* o_name = "world_new.tif";

	//const char* i_path = "E:\\Research\\Auge.GIS\\Dist\\32_x86_win_vc10\\binD\\upload\\world.tif";
	//const char* o_path = "E:\\Research\\Auge.GIS\\Dist\\32_x86_win_vc10\\binD\\upload\\world_new.tif";

	//const char* i_path = "E:\\Research\\Auge.GIS\\Dist\\32_x86_win_vc10\\binD\\upload\\Koala.png";
	//const char* o_path = "E:\\Research\\Auge.GIS\\Dist\\32_x86_win_vc10\\binD\\upload\\Koala_new.png";

	const char* i_path = "E:\\Research\\Auge.GIS\\Dist\\32_x86_win_vc10\\binD\\upload\\Koala.jpeg";
	const char* o_path = "E:\\Research\\Auge.GIS\\Dist\\32_x86_win_vc10\\binD\\upload\\Koala_new.jpeg";

	auge::Raster* pinRaster = NULL;
	auge::Raster* poutRaster = NULL;
	auge::RasterIO* io = auge::augeGetRasterIOInstance();
	pinRaster = io->Read(i_path);

	auge::GEnvelope extent(0,0,90,90);
	//auge::GEnvelope extent(-180,-90,180,90);
	auge::RasterFactory* pRasterFactory = auge::augeGetRasterFactoryInstance();
	poutRaster = pRasterFactory->CreateRaster(o_name, extent, pinRaster);
	poutRaster->Save(o_path);

	pinRaster->Release();
}
