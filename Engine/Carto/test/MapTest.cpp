#include "MapTest.h"
#include "AugeGeometry.h"
#include "AugeCore.h"
#include "AugeCarto.h"
#include "AugeFeature.h"
#include "AugeData.h"

CPPUNIT_TEST_SUITE_REGISTRATION(MapTest);

auge::Map*	CreateMapObj();
auge::Map*	CreateMapObj_SLD();
auge::Style* LoadSLD(const char* path);

void MapTest::setUp() 
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

void MapTest::tearDown()
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

void MapTest::DrawMap()
{
	auge::GConnection	*m_pConnection = NULL;
	auge::DataEngine	*pEngine = NULL;
	auge::DataEngineManager* pEngineManager = NULL;
	pEngineManager = auge::augeGetDataEngineManagerInstance();
	auge::ConnectionManager* pConnManager = NULL;
	pConnManager = auge::augeGetConnectionManagerInstance();
	auge::Workspace* pWorkspace = NULL;
	pWorkspace = pConnManager->GetWorkspace("db1");

	auge::CartoManager* pCartoManager = NULL;
	pCartoManager = auge::augeGetCartoManagerInstance();

	/*auge::GConnection	*m_pConnection = NULL;
	auge::DataEngine	*pEngine = NULL;
	auge::DataEngineManager* pEngineManager = NULL;
	pEngineManager = auge::augeGetDataEngineManagerInstance();
	pEngineManager->Load();
	pEngine = pEngineManager->GetEngine("Postgres");
	m_pConnection = pEngine->CreateConnection();
	m_pConnection->SetConnectionString("SERVER=127.0.0.1;INSTANCE=5432;DATABASE=gisdb;USER=postgres;PASSWORD=qwer1234;ENCODING=GBK");
	m_pConnection->Open();

	auge::ConnectionManager* pConnManager = NULL;
	pConnManager = auge::augeGetConnectionManagerInstance();
	pConnManager->Initialize(m_pConnection);

	auge::CartoManager* pCartoManager = NULL;
	pCartoManager = auge::augeGetCartoManagerInstance();
	pCartoManager->Initialize(m_pConnection);
	*/

	auge::Canvas* pCanvas = NULL;
	auge::CartoFactory* pCartoFactory = auge::augeGetCartoFactoryInstance();

	auge::GEnvelope viewer(-180.f,-90.f,180.f,90.f);
	pCanvas = pCartoFactory->CreateCanvas2D(800, 600);
	pCanvas->SetViewer(viewer);

	auge::GColor bgColor(255,0,0,0);
	pCanvas->DrawBackground(bgColor);

	auge::Map* pMap = NULL;
	pMap = pCartoManager->LoadMap("world");

	pCanvas->Draw(pMap);
	pCanvas->Save("g:\\temp\\map\\map.png");
	//pCanvas->Save("/home/renyc/map/map.png");

	//m_pConnection->Close();
	//AUGE_SAFE_RELEASE(m_pConnection);
	AUGE_SAFE_RELEASE(pMap);
	AUGE_SAFE_RELEASE(pCanvas);
}



void MapTest::DrawMapObj()
{
	/*auge::GConnection	*m_pConnection = NULL;
	auge::DataEngine	*pEngine = NULL;
	auge::DataEngineManager* pEngineManager = NULL;
	pEngineManager = auge::augeGetDataEngineManagerInstance();
	pEngineManager->Load();
	pEngine = pEngineManager->GetEngine("Postgres");
	m_pConnection = pEngine->CreateConnection();
	m_pConnection->SetConnectionString("SERVER=127.0.0.1;INSTANCE=5432;DATABASE=gisdb;USER=postgres;PASSWORD=qwer1234;ENCODING=GBK");
	m_pConnection->Open();

	auge::ConnectionManager* pConnectionManager = NULL;
	pConnectionManager = auge::augeGetConnectionManagerInstance();
	pConnectionManager->Initialize(m_pConnection);

	auge::Canvas* pCanvas = NULL;
	auge::CartoFactory* pCartoFactory = auge::augeGetCartoFactoryInstance();

	auge::GEnvelope viewer(-180.f,-90.f,180.f,90.f);
	pCanvas = pCartoFactory->CreateCanvas2D(800, 600);
	pCanvas = pCartoFactory->CreateCanvas2D(800, 600);
	pCanvas->SetViewer(viewer);

	auge::GColor bgColor(255,255,0,100);
	pCanvas->DrawBackground(bgColor);

	auge::Map* pMap = CreateMapObj();
	pCanvas->Draw(pMap);
	pCanvas->Save("g:\\temp\\map\\map.png");

	m_pConnection->Close();
	AUGE_SAFE_RELEASE(m_pConnection);
	AUGE_SAFE_RELEASE(pMap);
	AUGE_SAFE_RELEASE(pCanvas);*/
}

void MapTest::DrawMapSLD()
{
	/*auge::GConnection	*m_pConnection = NULL;
	auge::DataEngine	*pEngine = NULL;
	auge::DataEngineManager* pEngineManager = NULL;
	pEngineManager = auge::augeGetDataEngineManagerInstance();
	pEngineManager->Load();
	pEngine = pEngineManager->GetEngine("Postgres");
	m_pConnection = pEngine->CreateConnection();
	m_pConnection->SetConnectionString("SERVER=127.0.0.1;INSTANCE=5432;DATABASE=gisdb;USER=postgres;PASSWORD=qwer1234;ENCODING=GBK");
	m_pConnection->Open();

	auge::ConnectionManager* pConnectionManager = NULL;
	pConnectionManager = auge::augeGetConnectionManagerInstance();
	pConnectionManager->Initialize(m_pConnection);

	auge::Canvas* pCanvas = NULL;
	auge::CartoFactory* pCartoFactory = auge::augeGetCartoFactoryInstance();

	auge::GEnvelope viewer(-180.f,-90.f,180.f,90.f);
	pCanvas = pCartoFactory->CreateCanvas2D(800, 600);
	pCanvas = pCartoFactory->CreateCanvas2D(800, 600);
	pCanvas->SetViewer(viewer);

	auge::GColor bgColor(255,255,0,100);
	pCanvas->DrawBackground(bgColor);

	auge::Map* pMap = CreateMapObj_SLD();
	pCanvas->Draw(pMap);
	pCanvas->Save("g:\\temp\\map\\map_SLD.png");

	m_pConnection->Close();
	AUGE_SAFE_RELEASE(m_pConnection);
	AUGE_SAFE_RELEASE(pMap);
	AUGE_SAFE_RELEASE(pCanvas);*/
}

void MapTest::CreateMap()
{

	auge::CartoManager* pCartoManager = NULL;
	pCartoManager = auge::augeGetCartoManagerInstance();

	auge::Map* pMap = NULL;
	pMap = pCartoManager->CreateMap("world");

	AUGE_SAFE_RELEASE(pMap);

	//m_pConnection->Close();
	//AUGE_SAFE_RELEASE(m_pConnection);
}

void MapTest::Draw_Map_Point_Label()
{
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
	pCanvas = pCartoFactory->CreateCanvas2D(800, 600);
	pCanvas->SetViewer(viewer);

	auge::GColor bgColor(255,0,0,0);
	pCanvas->DrawBackground(bgColor);

	auge::Map* pMap = NULL;
	pMap = pCartoManager->LoadMap("city_label");

	pCanvas->Draw(pMap);
	//pCanvas->Save("g:\\temp\\map\\map.png");
	pCanvas->Save("/home/renyc/map/map.png");

	//m_pConnection->Close();
	//AUGE_SAFE_RELEASE(m_pConnection);
	AUGE_SAFE_RELEASE(pMap);
	AUGE_SAFE_RELEASE(pCanvas);
}

void MapTest::Draw_Map_Polygon_Label()
{
	const char* className = "country";
	auge::DataEngine	*pEngine = NULL;
	auge::DataEngineManager* pEngineManager = NULL;
	pEngineManager = auge::augeGetDataEngineManagerInstance();
	auge::ConnectionManager* pConnManager = NULL;
	pConnManager = auge::augeGetConnectionManagerInstance();
	auge::CartoManager* pCartoManager = NULL;
	pCartoManager = auge::augeGetCartoManagerInstance();
	auge::CartoFactory* pCartoFactory = auge::augeGetCartoFactoryInstance();

	auge::Canvas* pCanvas = NULL;
	auge::GEnvelope viewer(-180.f,-90.f,180.f,90.f);
	pCanvas = pCartoFactory->CreateCanvas2D(800, 600);
	pCanvas->SetViewer(viewer);

	auge::GColor bgColor(255,0,0,0);
	pCanvas->DrawBackground(bgColor);

	auge::FeatureWorksapce* pWorkspace = NULL;
	pWorkspace = (auge::FeatureWorksapce*)pConnManager->GetWorkspace("db1");
	auge::FeatureClass* pFeatureClass = pWorkspace->OpenFeatureClass(className);

	auge::Style* pStyle = NULL;
	pStyle = LoadSLD("E:\\Research\\Auge.GIS\\Engine\\Carto\\sld\\polygon_label.xml");

	auge::FeatureLayer* pLayer = pCartoFactory->CreateFeatureLayer();
	pLayer->SetName(className);
	pLayer->SetFeatureClass(pFeatureClass);
	pLayer->SetStyle(pStyle);

	auge::Map* pMap = NULL;
	pMap = pCartoFactory->CreateMap();
	pMap->AddLayer(pLayer);

	pCanvas->Draw(pMap);
	pCanvas->Save("g:\\temp\\map\\country.png");
	//pCanvas->Save("/home/renyc/map/map.png");

	//m_pConnection->Close();
	//AUGE_SAFE_RELEASE(m_pConnection);
	AUGE_SAFE_RELEASE(pMap);
	AUGE_SAFE_RELEASE(pCanvas);
}

void MapTest::Draw_Map_Point_Graphic()
{
	const char* className = "cities";
	auge::DataEngine	*pEngine = NULL;
	auge::DataEngineManager* pEngineManager = NULL;
	pEngineManager = auge::augeGetDataEngineManagerInstance();
	auge::ConnectionManager* pConnManager = NULL;
	pConnManager = auge::augeGetConnectionManagerInstance();
	auge::CartoManager* pCartoManager = NULL;
	pCartoManager = auge::augeGetCartoManagerInstance();
	auge::CartoFactory* pCartoFactory = auge::augeGetCartoFactoryInstance();

	auge::Canvas* pCanvas = NULL;
	auge::GEnvelope viewer(-180.f,-90.f,180.f,90.f);
	pCanvas = pCartoFactory->CreateCanvas2D(800, 600);
	pCanvas->SetViewer(viewer);

	auge::GColor bgColor(255,0,0,0);
	pCanvas->DrawBackground(bgColor);

	auge::FeatureWorksapce* pWorkspace = NULL;
	pWorkspace = (auge::FeatureWorksapce*)pConnManager->GetWorkspace("db1");
	auge::FeatureClass* pFeatureClass = pWorkspace->OpenFeatureClass(className);

	auge::Style* pStyle = NULL;
	pStyle = LoadSLD("E:\\Research\\Auge.GIS\\Engine\\Carto\\sld\\point_graphic.xml");

	auge::FeatureLayer* pLayer = pCartoFactory->CreateFeatureLayer();
	pLayer->SetName(className);
	pLayer->SetFeatureClass(pFeatureClass);
	pLayer->SetStyle(pStyle);

	auge::Map* pMap = NULL;
	pMap = pCartoFactory->CreateMap();
	pMap->AddLayer(pLayer);

#ifdef WIN32
	DWORD ts = GetTickCount();
#endif

	pCanvas->Draw(pMap);
	pCanvas->Save("g:\\temp\\map\\point_graphic.png");
	//pCanvas->Save("/home/renyc/map/map.png");

#ifdef WIN32
	DWORD te = GetTickCount();
	printf("[Time]:%fºÁÃë\n", (te-ts)/1000.0f);
#endif

	//m_pConnection->Close();
	//AUGE_SAFE_RELEASE(m_pConnection);
	AUGE_SAFE_RELEASE(pMap);
	AUGE_SAFE_RELEASE(pCanvas);
}


void MapTest::Create_Map_Point_Label()
{
	auge::GConnection	*m_pConnection = NULL;
	auge::DataEngine	*pEngine = NULL;
	auge::DataEngineManager* pEngineManager = NULL;
	pEngineManager = auge::augeGetDataEngineManagerInstance();
	auge::ConnectionManager* pConnManager = NULL;
	pConnManager = auge::augeGetConnectionManagerInstance();
	auge::CartoManager* pCartoManager = NULL;
	pCartoManager = auge::augeGetCartoManagerInstance();

	auge::Workspace* pWorkspace = NULL;
	pWorkspace = pConnManager->GetWorkspace("db1");

	auge::Map* pMap = NULL;
	pMap = pCartoManager->GetMap("city_label");
	if(pMap==NULL)
	{
		pMap = pCartoManager->CreateMap("city_label");
	}

	auge::Style* pStyle = NULL;
	pStyle = pCartoManager->GetStyle("cities_label", NULL);

	auge::Layer *pLayer = NULL;
	pLayer = pCartoManager->CreateLayer("cities",auge::augeLayerFeature, "cities", pMap->GetID(), pWorkspace->GetID(),pStyle->GetID());
	pMap->AddLayer(pLayer);

	AUGE_SAFE_RELEASE(pMap);

}

void MapTest::Draw_Map_Point_Label_Anchor()
{
	auge::DataEngine	*pEngine = NULL;
	auge::DataEngineManager* pEngineManager = NULL;
	pEngineManager = auge::augeGetDataEngineManagerInstance();
	auge::ConnectionManager* pConnManager = NULL;
	pConnManager = auge::augeGetConnectionManagerInstance();
	auge::CartoManager* pCartoManager = NULL;
	pCartoManager = auge::augeGetCartoManagerInstance();
	auge::CartoFactory* pCartoFactory = auge::augeGetCartoFactoryInstance();

	auge::Canvas* pCanvas = NULL;
	auge::GEnvelope viewer(-180.f,-90.f,180.f,90.f);
	pCanvas = pCartoFactory->CreateCanvas2D(800, 600);
	pCanvas->SetViewer(viewer);

	auge::GColor bgColor(255,0,0,0);
	pCanvas->DrawBackground(bgColor);

	auge::FeatureWorksapce* pWorkspace = NULL;
	pWorkspace = (auge::FeatureWorksapce*)pConnManager->GetWorkspace("db1");
	auge::FeatureClass* pFeatureClass = pWorkspace->OpenFeatureClass("cities");

	auge::Style* pStyle = NULL;
	pStyle = LoadSLD("E:\\Research\\Auge.GIS\\Engine\\Carto\\sld\\point_label_top_left.xml");
	//pStyle = LoadSLD("E:\\Research\\Auge.GIS\\Engine\\Carto\\sld\\point.xml");
	//pStyle = LoadSLD("E:\\Research\\Auge.GIS\\Engine\\Carto\\sld\\point_2.xml");

	auge::FeatureLayer* pLayer = pCartoFactory->CreateFeatureLayer();
	pLayer->SetName("cities");
	pLayer->SetFeatureClass(pFeatureClass);
	pLayer->SetStyle(pStyle);

	auge::Map* pMap = NULL;
	pMap = pCartoFactory->CreateMap();
	pMap->AddLayer(pLayer);

	pCanvas->Draw(pMap);
	pCanvas->Save("g:\\temp\\map\\map.png");
	//pCanvas->Save("/home/renyc/map/map.png");

	//m_pConnection->Close();
	//AUGE_SAFE_RELEASE(m_pConnection);
	AUGE_SAFE_RELEASE(pMap);
	AUGE_SAFE_RELEASE(pCanvas);
}

void MapTest::Draw_Line()
{
	auge::DataEngine	*pEngine = NULL;
	auge::DataEngineManager* pEngineManager = NULL;
	pEngineManager = auge::augeGetDataEngineManagerInstance();
	auge::ConnectionManager* pConnManager = NULL;
	pConnManager = auge::augeGetConnectionManagerInstance();
	auge::CartoManager* pCartoManager = NULL;
	pCartoManager = auge::augeGetCartoManagerInstance();
	auge::CartoFactory* pCartoFactory = auge::augeGetCartoFactoryInstance();

	auge::Canvas* pCanvas = NULL;
	auge::GEnvelope viewer(-180.f,-90.f,180.f,90.f);
	pCanvas = pCartoFactory->CreateCanvas2D(800, 600);
	pCanvas->SetViewer(viewer);

	auge::GColor bgColor(255,0,0,0);
	pCanvas->DrawBackground(bgColor);

	auge::FeatureWorksapce* pWorkspace = NULL;
	pWorkspace = (auge::FeatureWorksapce*)pConnManager->GetWorkspace("db1");
	auge::FeatureClass* pFeatureClass = pWorkspace->OpenFeatureClass("cities");

	auge::Style* pStyle = NULL;
	pStyle = LoadSLD("E:\\Research\\Auge.GIS\\Engine\\Carto\\sld\\point_label_top_left.xml");
	//pStyle = LoadSLD("E:\\Research\\Auge.GIS\\Engine\\Carto\\sld\\point.xml");
	//pStyle = LoadSLD("E:\\Research\\Auge.GIS\\Engine\\Carto\\sld\\point_2.xml");

	auge::FeatureLayer* pLayer = pCartoFactory->CreateFeatureLayer();
	pLayer->SetName("cities");
	pLayer->SetFeatureClass(pFeatureClass);
	pLayer->SetStyle(pStyle);

	auge::Map* pMap = NULL;
	pMap = pCartoFactory->CreateMap();
	pMap->AddLayer(pLayer);

	pCanvas->Draw(pMap);
	pCanvas->Save("g:\\temp\\map\\map.png");
	//pCanvas->Save("/home/renyc/map/map.png");

	//m_pConnection->Close();
	//AUGE_SAFE_RELEASE(m_pConnection);
	AUGE_SAFE_RELEASE(pMap);
	AUGE_SAFE_RELEASE(pCanvas);
}

void MapTest::CreateLayer()
{
	//auge::GConnection	*m_pConnection = NULL;
	//auge::DataEngine	*pEngine = NULL;
	//auge::DataEngineManager* pEngineManager = NULL;
	//pEngineManager = auge::augeGetDataEngineManagerInstance();
	//pEngineManager->Load();
	//pEngine = pEngineManager->GetEngine("Postgres");
	//m_pConnection = pEngine->CreateConnection();
	//m_pConnection->SetConnectionString("SERVER=127.0.0.1;INSTANCE=5432;DATABASE=gisdb;USER=postgres;PASSWORD=qwer1234;ENCODING=GBK");
	//m_pConnection->Open();

	auge::ConnectionManager* pConnManager = NULL;
	pConnManager = auge::augeGetConnectionManagerInstance();
	auge::Workspace* pWorkspace = NULL;
	pWorkspace = pConnManager->GetWorkspace("db1");

	auge::CartoManager* pCartoManager = NULL;
	pCartoManager = auge::augeGetCartoManagerInstance();
	pCartoManager->Initialize(m_pConnection);

	auge::Map* pMap = NULL;
	pMap = pCartoManager->LoadMap("world");

	auge::Layer *pLayer = NULL;
	pLayer = pCartoManager->CreateLayer("cities",auge::augeLayerFeature, "cities", pMap->GetID(), pWorkspace->GetID(),1);
	pMap->AddLayer(pLayer);
	pLayer = pCartoManager->CreateLayer("rivers",auge::augeLayerFeature, "rivers", pMap->GetID(), pWorkspace->GetID(),3);
	pMap->AddLayer(pLayer);
	pLayer = pCartoManager->CreateLayer("country",auge::augeLayerFeature, "country", pMap->GetID(), pWorkspace->GetID(),4);
	pMap->AddLayer(pLayer);

	//AUGE_SAFE_RELEASE(pMap);

	//m_pConnection->Close();
	//AUGE_SAFE_RELEASE(m_pConnection);
}

auge::Map* CreateMapObj()
{
	auge::Workspace *pWorkspace = NULL;
	auge::FeatureWorksapce* pFWorkspace = NULL;
	auge::FeatureClass* pFeatureClass = NULL;

	auge::ConnectionManager* pConnectionManager = NULL;
	pConnectionManager = auge::augeGetConnectionManagerInstance();

	pWorkspace = pConnectionManager->GetWorkspace("db1");
	pFWorkspace = static_cast<auge::FeatureWorksapce*>(pWorkspace);

	auge::Map* pMap = NULL;
	auge::FeatureLayer* pFLayer = NULL;

	auge::CartoFactory* pCartoFactory = NULL;
	pCartoFactory = auge::augeGetCartoFactoryInstance();

	auge::FeatureStyle* pStyle = NULL;
	auge::Rule *pRule = NULL;
	auge::Symbolizer* pSymbolizer = NULL;
	auge::StyleFactory* pStyleFactory = auge::augeGetStyleFactoryInstance();

	pMap= pCartoFactory->CreateMap();
	pMap->SetName("world");

	pFeatureClass = pFWorkspace->OpenFeatureClass("cities");
	pSymbolizer = pStyleFactory->CreatePointSymbolizer();
	((auge::PointSymbolizer*)pSymbolizer)->SetSize(2);
	pRule = pStyleFactory->CreateRule();
	pRule->SetSymbolizer(pSymbolizer);
	pStyle = pStyleFactory->CreateFeatureStyle();
	pStyle->AddRule(pRule);
	pFLayer = pCartoFactory->CreateFeatureLayer();
	pFLayer->SetName("cities");
	pFLayer->SetFeatureClass(pFeatureClass);
	pFLayer->SetStyle(pStyle);	
	pMap->AddLayer(pFLayer);

	pFeatureClass = pFWorkspace->OpenFeatureClass("rivers");
	pSymbolizer = pStyleFactory->CreateLineSymbolizer();
	pRule = pStyleFactory->CreateRule();
	pRule->SetSymbolizer(pSymbolizer);
	pStyle = pStyleFactory->CreateFeatureStyle();
	pStyle->AddRule(pRule);
	pFLayer = pCartoFactory->CreateFeatureLayer();
	pFLayer->SetName("rivers");
	pFLayer->SetFeatureClass(pFeatureClass);
	pFLayer->SetStyle(pStyle);	
	pMap->AddLayer(pFLayer);

	pFeatureClass = pFWorkspace->OpenFeatureClass("country");
	pSymbolizer = pStyleFactory->CreatePolygonSymbolizer();
	pRule = pStyleFactory->CreateRule();
	pRule->SetSymbolizer(pSymbolizer);
	pStyle = pStyleFactory->CreateFeatureStyle();
	pStyle->AddRule(pRule);
	pFLayer = pCartoFactory->CreateFeatureLayer();
	pFLayer->SetName("country");
	pFLayer->SetFeatureClass(pFeatureClass);
	pFLayer->SetStyle(pStyle);
	pMap->AddLayer(pFLayer);

	return pMap;
}

auge::Map* CreateMapObj_SLD()
{
	//const char* path = "E:\\Research\\Auge.GIS\\Engine\\Carto\\sld\\cities_1.xml";
	const char* path = "/home/renyc/auge.gis/Engine/Carto/sld/cities_1.xml";

	auge::Workspace *pWorkspace = NULL;
	auge::FeatureWorksapce* pFWorkspace = NULL;
	auge::FeatureClass* pFeatureClass = NULL;

	auge::ConnectionManager* pConnectionManager = NULL;
	pConnectionManager = auge::augeGetConnectionManagerInstance();

	pWorkspace = pConnectionManager->GetWorkspace("db1");
	pFWorkspace = static_cast<auge::FeatureWorksapce*>(pWorkspace);

	auge::Map* pMap = NULL;
	auge::FeatureLayer* pFLayer = NULL;

	auge::CartoFactory* pCartoFactory = NULL;
	pCartoFactory = auge::augeGetCartoFactoryInstance();

	auge::Style* pStyle = NULL;
	auge::Rule *pRule = NULL;
	auge::Symbolizer* pSymbolizer = NULL;
	auge::StyleFactory	*pStyleFactory = auge::augeGetStyleFactoryInstance();
	auge::StyleReader	*reader = pStyleFactory->CreateStyleReader();

	pStyle = reader->Read(path, NULL);

	reader->Release();

	pMap= pCartoFactory->CreateMap();
	pMap->SetName("world");

	pFeatureClass = pFWorkspace->OpenFeatureClass("cities");
	pFLayer = pCartoFactory->CreateFeatureLayer();
	pFLayer->SetName("cities");
	pFLayer->SetFeatureClass(pFeatureClass);
	pFLayer->SetStyle(pStyle);	
	pMap->AddLayer(pFLayer);

	auge::StyleWriter* writer = NULL;
	writer = pStyleFactory->CreateStyleWriter();

	auge::XDocument* pxDoc = NULL;
	pxDoc = writer->Write(pStyle,"1.0.0");
	writer->Release();

	//pxDoc->Save("g:\\temp\\map\\sld.xml", "GBK");
	pxDoc->Save("/home/renyc/map/sld.xml", "GBK");
	pxDoc->Release();

	return pMap;
}

auge::Style* MapTest::LoadSLD(const char* path)
{
	//const char* path = "E:\\Research\\Auge.GIS\\Engine\\Carto\\sld\\point_user_4.xml";

	auge::CartoFactory* pCartoFactory = NULL;
	pCartoFactory = auge::augeGetCartoFactoryInstance();

	auge::StyleFactory* pStyleFactory = NULL;
	pStyleFactory = auge::augeGetStyleFactoryInstance();

	auge::StyleReader* reader = NULL;
	reader = pStyleFactory->CreateStyleReader();

	auge::Style* pStyle = NULL;
	pStyle = reader->Read(path, NULL);

	return pStyle;
}

void MapTest::DrawCountry()
{
	const char* className = "country";
	auge::DataEngine	*pEngine = NULL;
	auge::DataEngineManager* pEngineManager = NULL;
	pEngineManager = auge::augeGetDataEngineManagerInstance();
	auge::ConnectionManager* pConnManager = NULL;
	pConnManager = auge::augeGetConnectionManagerInstance();
	auge::CartoManager* pCartoManager = NULL;
	pCartoManager = auge::augeGetCartoManagerInstance();
	auge::CartoFactory* pCartoFactory = auge::augeGetCartoFactoryInstance();

	auge::Canvas* pCanvas = NULL;
	auge::GEnvelope viewer(-180.f,-90.f,180.f,90.f);
	pCanvas = pCartoFactory->CreateCanvas2D(800, 600);
	pCanvas->SetViewer(viewer);

	auge::GColor bgColor(255,0,0,0);
	pCanvas->DrawBackground(bgColor);

	auge::FeatureWorksapce* pWorkspace = NULL;
	pWorkspace = (auge::FeatureWorksapce*)pConnManager->GetWorkspace("db1");
	auge::FeatureClass* pFeatureClass = pWorkspace->OpenFeatureClass(className);

	auge::Style* pStyle = NULL;
	pStyle = LoadSLD("E:\\Research\\Auge.GIS\\Engine\\Carto\\sld\\polygon_red.xml");

	auge::FeatureLayer* pLayer = pCartoFactory->CreateFeatureLayer();
	pLayer->SetName(className);
	pLayer->SetFeatureClass(pFeatureClass);
	pLayer->SetStyle(pStyle);

	auge::Map* pMap = NULL;
	pMap = pCartoFactory->CreateMap();
	pMap->AddLayer(pLayer);

	pCanvas->Draw(pMap);
	pCanvas->Save("g:\\temp\\map\\country.png");
	//pCanvas->Save("/home/renyc/map/map.png");

	//m_pConnection->Close();
	//AUGE_SAFE_RELEASE(m_pConnection);
	AUGE_SAFE_RELEASE(pMap);
	AUGE_SAFE_RELEASE(pCanvas);
}


void MapTest::DrawCities()
{
	const char* className = "cities";
	auge::DataEngine	*pEngine = NULL;
	auge::DataEngineManager* pEngineManager = NULL;
	pEngineManager = auge::augeGetDataEngineManagerInstance();
	auge::ConnectionManager* pConnManager = NULL;
	pConnManager = auge::augeGetConnectionManagerInstance();
	auge::CartoManager* pCartoManager = NULL;
	pCartoManager = auge::augeGetCartoManagerInstance();
	auge::CartoFactory* pCartoFactory = auge::augeGetCartoFactoryInstance();

	auge::Canvas* pCanvas = NULL;
	auge::GEnvelope viewer(-180.f,-90.f,180.f,90.f);
	pCanvas = pCartoFactory->CreateCanvas2D(800, 600);
	pCanvas->SetViewer(viewer);

	auge::GColor bgColor(255,0,0,0);
	pCanvas->DrawBackground(bgColor);

	auge::FeatureWorksapce* pWorkspace = NULL;
	pWorkspace = (auge::FeatureWorksapce*)pConnManager->GetWorkspace("db1");
	auge::FeatureClass* pFeatureClass = pWorkspace->OpenFeatureClass(className);

	auge::Style* pStyle = NULL;
	pStyle = LoadSLD("E:\\Research\\Auge.GIS\\Engine\\Carto\\sld\\point.xml");
	//pStyle = LoadSLD("E:\\Research\\Auge.GIS\\Engine\\Carto\\sld\\point_theme_label.xml");

	auge::FeatureLayer* pLayer = pCartoFactory->CreateFeatureLayer();
	pLayer->SetName(className);
	pLayer->SetFeatureClass(pFeatureClass);
	pLayer->SetStyle(pStyle);

	auge::Map* pMap = NULL;
	pMap = pCartoFactory->CreateMap();
	pMap->AddLayer(pLayer);

	pCanvas->Draw(pMap);
	pCanvas->Save("g:\\temp\\map\\point_theme_label.png");
	//pCanvas->Save("/home/renyc/map/map.png");

	//m_pConnection->Close();
	//AUGE_SAFE_RELEASE(m_pConnection);
	AUGE_SAFE_RELEASE(pMap);
	AUGE_SAFE_RELEASE(pCanvas);
}

