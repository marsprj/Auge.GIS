#include "RasterTest.h"
#include "AugeGeometry.h"
#include "AugeCore.h"
#include "AugeCarto.h"
#include "AugeRaster.h"
#include "AugeData.h"

//CPPUNIT_TEST_SUITE_REGISTRATION(RasterTest);

auge::Map*	CreateMapObj();
auge::Map*	CreateMapObj_SLD();
auge::Style* LoadSLD(const char* path);

void RasterTest::setUp() 
{
	printf("setUp\n");
	auge::DataEngine	*pEngine = NULL;
	auge::DataEngineManager* pEngineManager = NULL;
	pEngineManager = auge::augeGetDataEngineManagerInstance();
	pEngineManager->Load();
	pEngine = pEngineManager->GetEngine("Postgres");
	m_pConnection = pEngine->CreateConnection();
	m_pConnection->SetConnectionString("SERVER=127.0.0.1;INSTANCE=5432;DATABASE=auge;USER=postgres;PASSWORD=qwer1234;ENCODING=GBK");
	//m_pConnection->SetConnectionString("SERVER=192.168.111.160;INSTANCE=5432;DATABASE=auge;USER=postgres;PASSWORD=qwer1234;ENCODING=GBK");
	m_pConnection->Open();

	auge::ConnectionManager* pConnManager = NULL;
	pConnManager = auge::augeGetConnectionManagerInstance();
	pConnManager->Initialize(m_pConnection);

	auge::CartoManager* pCartoManager = NULL;
	pCartoManager = auge::augeGetCartoManagerInstance();
	pCartoManager->Initialize(m_pConnection);
}

void RasterTest::tearDown()
{
	auge::CartoManager* pCartoManager = NULL;
	pCartoManager = auge::augeGetCartoManagerInstance();
	pCartoManager->Cleanup();

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

void RasterTest::DrawMap()
{
	auge::CartoFactory* pCartoFactory = auge::augeGetCartoFactoryInstance();
	auge::Map* pMap = NULL;
	pMap = pCartoFactory->CreateMap();
	pMap->SetName("world");

	auge::FeatureLayer* pFeatureLayer = CreateFeatureLayer();
	pMap->AddLayer(pFeatureLayer);

	auge::RasterLayer* pRasterLayer = CreateRasterLayer();
	pMap->AddLayer(pRasterLayer);

	//auge::GEnvelope viewer(-180.f,-90.f,180.0f,90.f);
	//auge::GEnvelope viewer(0.0f,0.0f,180.0f,90.0f);
	//auge::GEnvelope viewer(-180.f,0.0f,0.0f,90.0f);
	auge::GEnvelope viewer(-256.0f,-256.0f,256.0f,256.0f);
	//auge::GEnvelope viewer(-20.0f,-12.0f,42.0f,45.0f);
	//auge::GEnvelope& viewer = pMap->GetExtent();
	//auge::Canvas* pCanvas = pCartoFactory->CreateCanvas2D(800, 600);
	auge::Canvas* pCanvas = pCartoFactory->CreateCanvas2D(1600, 1200);
	pCanvas->SetViewer(viewer);

	auge::GColor bgColor(255,0,0,0);
	pCanvas->DrawBackground(bgColor);

	DWORD ts = GetTickCount();
	pCanvas->Draw(pMap);
	DWORD te = GetTickCount();
	pCanvas->Save("g:\\temp\\map\\map.png");
	
	printf("[Ê±¼ä]:%dºÁÃë",te-ts);

	AUGE_SAFE_RELEASE(pMap);
	AUGE_SAFE_RELEASE(pCanvas);
	//AUGE_SAFE_RELEASE(pWorkspace);
}

auge::RasterLayer* RasterTest::CreateRasterLayer()
{
	auge::DataEngine	*pEngine = NULL; 
	auge::DataEngineManager* pEngineManager = NULL;
	pEngineManager = auge::augeGetDataEngineManagerInstance();
	pEngine = pEngineManager->GetEngine("Raster");

	auge::CartoFactory* pCartoFactory = auge::augeGetCartoFactoryInstance();

	//const char* dataName = "tj.jpg";
	//const char* connstr = "DATABASE=G:\\Data\\GF-1\\tianjin";
	//const char* dataName = "tj.png";
	//const char* connstr = "DATABASE=G:\\Data\\GF-1\\tianjin_png";
	//const char* dataName = "16km.png";
	//const char* dataName = "16km.tif";
	const char* dataName = "16km.jpg";
	//const char* dataName = "16km_24.png";
	//const char* dataName = "16km_64.png";
	const char* connstr = "DATABASE=G:\\temp\\td";

	auge::RasterWorkspace* pWorkspace = dynamic_cast<auge::RasterWorkspace*>(pEngine->CreateWorkspace());
	pWorkspace->SetConnectionString(connstr);
	pWorkspace->Open();

	//auge::RasterDataset* pDataset = pWorkspace->OpenRasterDataset(dataName);
	//auge::Raster* pRaster = pWorkspace->OpenRaster(dataName);
	auge::RasterLayer* pRasterLayer = pCartoFactory->CreateRasterLayer();
	pRasterLayer->SetName(dataName);
	//pRasterLayer->SetRasterDataset(pDataset);
	return pRasterLayer;
}

auge::FeatureLayer* RasterTest::CreateFeatureLayer()
{
	const char* className = "country";
	auge::DataEngine	*pEngine = NULL;
	auge::DataEngineManager* pEngineManager = NULL;
	pEngineManager = auge::augeGetDataEngineManagerInstance();
	auge::ConnectionManager* pConnManager = NULL;
	pConnManager = auge::augeGetConnectionManagerInstance();
	auge::CartoFactory* pCartoFactory = auge::augeGetCartoFactoryInstance();

	
	auge::FeatureWorksapce* pWorkspace = NULL;
	pWorkspace = dynamic_cast<auge::FeatureWorksapce*>(pConnManager->GetWorkspace("db1"));
	auge::FeatureClass* pFeatureClass = pWorkspace->OpenFeatureClass(className);

	auge::Style* pStyle = NULL;
	pStyle = LoadSLD("E:\\Research\\Auge.GIS\\Engine\\Carto\\sld\\polygon_5.xml");

	auge::FeatureLayer* pLayer = pCartoFactory->CreateFeatureLayer();
	pLayer->SetName(className);
	pLayer->SetFeatureClass(pFeatureClass);
	pLayer->SetStyle(pStyle);

	return pLayer;
}