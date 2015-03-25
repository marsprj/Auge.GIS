#include "LabelTest.h"
#include "AugeGeometry.h"
#include "AugeCore.h"
#include "AugeCarto.h"
#include "AugeFeature.h"
#include "AugeData.h"

CPPUNIT_TEST_SUITE_REGISTRATION(LabelTest);

auge::Map*	CreateMapObj();
auge::Map*	CreateMapObj_SLD();
auge::Style* LoadSLD(const char* path);

void LabelTest::setUp() 
{
	printf("setUp\n");
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

	auge::CartoManager* pCartoManager = NULL;
	pCartoManager = auge::augeGetCartoManagerInstance();
	pCartoManager->Initialize(m_pConnection);

}

void LabelTest::tearDown()
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

void LabelTest::DrawMap()
{
	auge::GConnection	*m_pConnection = NULL;
	auge::DataEngine	*pEngine = NULL;
	auge::DataEngineManager* pEngineManager = NULL;
	pEngineManager = auge::augeGetDataEngineManagerInstance();
	auge::ConnectionManager* pConnManager = NULL;
	pConnManager = auge::augeGetConnectionManagerInstance();
	
	auge::CartoManager* pCartoManager = NULL;
	pCartoManager = auge::augeGetCartoManagerInstance();
	
	auge::Canvas* pCanvas = NULL;
	auge::CartoFactory* pCartoFactory = auge::augeGetCartoFactoryInstance();

	auge::GEnvelope viewer(-180.f,-90.f,180.f,90.f);
	pCanvas = pCartoFactory->CreateCanvas2D(1600, 1200);
	pCanvas->SetViewer(viewer);

	auge::GColor bgColor(255,0,0,0);
	pCanvas->DrawBackground(bgColor);

	auge::FeatureWorksapce* pWorkspace = NULL;
	pWorkspace = (auge::FeatureWorksapce*)pConnManager->GetWorkspace("db1");
	auge::FeatureClass* pFeatureClass = pWorkspace->OpenFeatureClass("cities");

	//const char* path = "E:\\Research\\Auge.GIS\\Engine\\Carto\\sld\\point_label.xml";
	const char* path = "E:\\Research\\Auge.GIS\\Engine\\Carto\\sld\\point_theme_label.xml";
	auge::Style* pStyle = LoadSLD(path, pFeatureClass);
	auge::FeatureLayer* pFLayer = pCartoFactory->CreateFeatureLayer();
	pFLayer->SetName("cities");
	pFLayer->SetFeatureClass(pFeatureClass);
	pFLayer->SetStyle(pStyle);
	
	auge::Map* pMap = NULL;
	pMap = pCartoFactory->CreateMap();
	pMap->SetName("world");
	pMap->AddLayer(pFLayer);

	pCanvas->Draw(pMap);
	pCanvas->Save("g:\\temp\\map\\map.png");
	//pCanvas->Save("/home/renyc/map/map.png");

	AUGE_SAFE_RELEASE(pMap);
	AUGE_SAFE_RELEASE(pCanvas);
}

auge::Style* LabelTest::LoadSLD(const char* path, auge::FeatureClass* pFeatureClass)
{
	//const char* path = "E:\\Research\\Auge.GIS\\Engine\\Carto\\sld\\point_user_4.xml";

	auge::CartoFactory* pCartoFactory = NULL;
	pCartoFactory = auge::augeGetCartoFactoryInstance();

	auge::StyleFactory* pStyleFactory = NULL;
	pStyleFactory = auge::augeGetStyleFactoryInstance();

	auge::StyleReader* reader = NULL;
	reader = pStyleFactory->CreateStyleReader();

	auge::Style* pStyle = NULL;
	pStyle = reader->Read(path, pFeatureClass);

	return pStyle;
}
