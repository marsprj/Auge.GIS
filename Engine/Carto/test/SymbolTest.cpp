#include "SymbolTest.h"
#include "AugeGeometry.h"
#include "AugeCore.h"
#include "AugeCarto.h"
#include "AugeFeature.h"
#include "AugeData.h"

//CPPUNIT_TEST_SUITE_REGISTRATION(SymbolTest);

void SymbolTest::setUp() 
{
	printf("setUp\n");
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

	auge::CartoManager* pCartoManager = NULL;
	pCartoManager = auge::augeGetCartoManagerInstance();
	pCartoManager->Initialize(m_pConnection);

	auge::SymbolManager* pSymbolManager = NULL;
	pSymbolManager = auge::augeGetSymbolManagerInstance();
	pSymbolManager->Initialize(m_pConnection);

	pSymbolManager->GetMarkerSymbols();
}

void SymbolTest::tearDown()
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

void SymbolTest::DrawStar()
{
	auge::Canvas* pCanvas = NULL;
	auge::CartoFactory* pCartoFactory = auge::augeGetCartoFactoryInstance();

	auge::Geometry* pGeometry = NULL;
	auge::GeometryFactory* pGeometryFactory = auge::augeGetGeometryFactoryInstance();
	pGeometry = pGeometryFactory->CreateGeometryFromWKT("POINT(116 39)");

	auge::MarkerSymbol* pMarker = NULL;
	auge::SymbolManager* pSymbolManager = auge::augeGetSymbolManagerInstance();
	//pMarker = pSymbolManager->CreateMarkerSymbol(auge::augeMarkerStar);
	pMarker = pSymbolManager->GetMarker("b020200");
	pMarker->SetRotation(10.0);
	pMarker->SetSize(20.0);

	//pSymbolManager->GetMarkerSymbols();

	auge::GEnvelope viewer(-180.f,-90.f,180.f,90.f);
	//auge::GEnvelope viewer(90.46,22.41,127.08,41.15);
	//auge::GEnvelope viewer(82.41,18.29,135.14,45.27);
	pCanvas = pCartoFactory->CreateCanvas2D(800, 600);
	pCanvas->SetViewer(viewer);

	auge::GColor bgColor(255,0,0,0);
	pCanvas->DrawBackground(bgColor);

	pCanvas->DrawSymbol(pGeometry, pMarker);

	pCanvas->Save("g:\\temp\\map\\star.png");
	//pCanvas->Save("/home/renyc/map/map.png");

	//AUGE_SAFE_RELEASE(pMarker);
	AUGE_SAFE_RELEASE(pGeometry);
	AUGE_SAFE_RELEASE(pCanvas);
}

void SymbolTest::DrawStars()
{
	DWORD ts, te;

	auge::Canvas* pCanvas = NULL;
	auge::CartoFactory* pCartoFactory = auge::augeGetCartoFactoryInstance();

	//auge::Geometry* pGeometry = NULL;
	//auge::GeometryFactory* pGeometryFactory = auge::augeGetGeometryFactoryInstance();
	//pGeometry = pGeometryFactory->CreateGeometryFromWKT("POINT(116 39)");

	auge::MarkerSymbol* pMarker = NULL;
	auge::SymbolManager* pSymbolManager = auge::augeGetSymbolManagerInstance();
	//pMarker = pSymbolManager->CreateMarkerSymbol(auge::augeMarkerStar);
	//pMarker = pSymbolManager->CreateMarkerSymbol(auge::augeMarkerCircle);
	pMarker = pSymbolManager->CreateMarkerSymbol(auge::augeMarkerTriangle);
	//pMarker = pSymbolManager->CreateMarkerSymbol(auge::augeMarkerSquare);
	//pMarker = pSymbolManager->CreateMarkerSymbol(auge::augeMarkerPentagon);
	//pMarker = pSymbolManager->CreateMarkerSymbol(auge::augeMarkerCapital);
	pMarker->SetRotation(10.0f);


	auge::GEnvelope viewer(-180.f,-90.f,180.f,90.f);
	pCanvas = pCartoFactory->CreateCanvas2D(1600, 1200);
	pCanvas->SetViewer(viewer);

	auge::GColor bgColor(255,0,0,0);
	pCanvas->DrawBackground(bgColor);

	RESULTCODE rc = AG_FAILURE;
	auge::FeatureWorkspace*	 pWorkspace = NULL;
	auge::ConnectionManager* pConnectionManager = auge::augeGetConnectionManagerInstance();
	
	pWorkspace = dynamic_cast<auge::FeatureWorkspace*>(pConnectionManager->GetWorkspace("db1"));
	
	auge::FeatureClass* pFeatureClass = NULL;
	pFeatureClass = pWorkspace->OpenFeatureClass("cities");
	CPPUNIT_ASSERT(pFeatureClass!=NULL);

	auge::Style* pStyle = NULL;
	pStyle = LoadSLD("E:\\Research\\Auge.GIS\\Engine\\Carto\\sld\\point.xml");

	auge::FeatureLayer* pLayer = pCartoFactory->CreateFeatureLayer();
	pLayer->SetName("cities");
	pLayer->SetFeatureClass(pFeatureClass);
	pLayer->SetStyle(pStyle);
	pFeatureClass->AddRef();

	auge::Map* pMap = NULL;
	pMap = pCartoFactory->CreateMap();
	pMap->AddLayer(pLayer);

	ts = GetTickCount();
	pCanvas->Draw(pMap);
	te = GetTickCount();
	printf("[时间]:%8.3f\n", (te-ts)/1000.0f);

	ts = GetTickCount();

	auge::FeatureCursor* pCursor = NULL;
	pCursor = pFeatureClass->Query();
	CPPUNIT_ASSERT(pCursor!=NULL);

	auge::Geometry	*pGeometry = NULL;
	auge::Feature	*pFeature = NULL;
	while((pFeature=pCursor->NextFeature())!=NULL)
	{	
		pGeometry = pFeature->GetGeometry();

		pCanvas->DrawSymbol(pGeometry, pMarker);

		pFeature->Release();
	}
	
	te = GetTickCount();
	printf("[时间]:%8.3f\n", (te-ts)/1000.0f);

	pCanvas->Save("g:\\temp\\map\\star.png");
	//pCanvas->Save("/home/renyc/map/map.png");

	AUGE_SAFE_RELEASE(pMarker);
	AUGE_SAFE_RELEASE(pCursor);
	AUGE_SAFE_RELEASE(pFeatureClass);
	AUGE_SAFE_RELEASE(pCanvas);
}

void SymbolTest::DrawRailways()
{
	DWORD ts, te;

	auge::Canvas* pCanvas = NULL;
	auge::CartoFactory* pCartoFactory = auge::augeGetCartoFactoryInstance();

	//auge::Geometry* pGeometry = NULL;
	//auge::GeometryFactory* pGeometryFactory = auge::augeGetGeometryFactoryInstance();
	//pGeometry = pGeometryFactory->CreateGeometryFromWKT("POINT(116 39)");

	auge::LineSymbol* pLineSymbol = NULL;
	auge::SymbolManager* pSymbolManager = auge::augeGetSymbolManagerInstance();
	pLineSymbol = pSymbolManager->CreateLineSymbol(auge::augeLineRailway);

	auge::GEnvelope viewer(-180.f,-90.f,180.f,90.f);
	pCanvas = pCartoFactory->CreateCanvas2D(8000, 6000);
	pCanvas->SetViewer(viewer);

	auge::GColor bgColor(255,0,0,0);
	pCanvas->DrawBackground(bgColor);

	RESULTCODE rc = AG_FAILURE;
	auge::FeatureWorkspace*	 pWorkspace = NULL;
	auge::ConnectionManager* pConnectionManager = auge::augeGetConnectionManagerInstance();

	pWorkspace = dynamic_cast<auge::FeatureWorkspace*>(pConnectionManager->GetWorkspace("db1"));

	auge::FeatureClass* pFeatureClass = NULL;
	pFeatureClass = pWorkspace->OpenFeatureClass("rivers");
	CPPUNIT_ASSERT(pFeatureClass!=NULL);

	//auge::Style* pStyle = NULL;
	//pStyle = LoadSLD("E:\\Research\\Auge.GIS\\Engine\\Carto\\sld\\point.xml");

	//auge::FeatureLayer* pLayer = pCartoFactory->CreateFeatureLayer();
	//pLayer->SetName("cities");
	//pLayer->SetFeatureClass(pFeatureClass);
	//pLayer->SetStyle(pStyle);
	//pFeatureClass->AddRef();

	//auge::Map* pMap = NULL;
	//pMap = pCartoFactory->CreateMap();
	//pMap->AddLayer(pLayer);

	//ts = GetTickCount();
	//pCanvas->Draw(pMap);
	//te = GetTickCount();
	//printf("[时间]:%8.3f\n", (te-ts)/1000.0f);

	ts = GetTickCount();

	auge::FeatureCursor* pCursor = NULL;
	pCursor = pFeatureClass->Query();
	CPPUNIT_ASSERT(pCursor!=NULL);

	auge::Geometry	*pGeometry = NULL;
	auge::Feature	*pFeature = NULL;
	while((pFeature=pCursor->NextFeature())!=NULL)
	{	
		pGeometry = pFeature->GetGeometry();

		pCanvas->DrawSymbol(pGeometry, pLineSymbol);

		pFeature->Release();
	}

	te = GetTickCount();
	printf("[时间]:%8.3f\n", (te-ts)/1000.0f);

	pCanvas->Save("g:\\temp\\map\\railway.png");
	//pCanvas->Save("/home/renyc/map/map.png");

	AUGE_SAFE_RELEASE(pLineSymbol);
	AUGE_SAFE_RELEASE(pCursor);
	AUGE_SAFE_RELEASE(pFeatureClass);
	AUGE_SAFE_RELEASE(pCanvas);
}

void SymbolTest::DrawFillSymbol()
{
	DWORD ts, te;

	auge::Canvas* pCanvas = NULL;
	auge::CartoFactory* pCartoFactory = auge::augeGetCartoFactoryInstance();

	//auge::Geometry* pGeometry = NULL;
	//auge::GeometryFactory* pGeometryFactory = auge::augeGetGeometryFactoryInstance();
	//pGeometry = pGeometryFactory->CreateGeometryFromWKT("POINT(116 39)");

	auge::FillSymbol* pFillSymbol = NULL;
	auge::SymbolManager* pSymbolManager = auge::augeGetSymbolManagerInstance();
	//pFillSymbol = pSymbolManager->GetFillSymbol("BeddedLimestone");
	pFillSymbol = pSymbolManager->GetFillSymbol("open_pasture");

	auge::GColor  color(0,0,0,255);
	auge::Stroke* pStroke = auge::augeGetStyleFactoryInstance()->CreateStroke();
	pStroke->SetColor(color);
	pStroke->SetWidth(0.5);
	pFillSymbol->SetStroke(pStroke);
	

	auge::GEnvelope viewer(-180.f,-90.f,180.f,90.f);
	pCanvas = pCartoFactory->CreateCanvas2D(800, 600);
	pCanvas->SetViewer(viewer);

	auge::GColor bgColor(255,0,0,0);
	pCanvas->DrawBackground(bgColor);

	RESULTCODE rc = AG_FAILURE;
	auge::FeatureWorkspace*	 pWorkspace = NULL;
	auge::ConnectionManager* pConnectionManager = auge::augeGetConnectionManagerInstance();

	pWorkspace = dynamic_cast<auge::FeatureWorkspace*>(pConnectionManager->GetWorkspace("db1"));

	auge::FeatureClass* pFeatureClass = NULL;
	pFeatureClass = pWorkspace->OpenFeatureClass("country");
	CPPUNIT_ASSERT(pFeatureClass!=NULL);

	ts = GetTickCount();

	auge::FeatureCursor* pCursor = NULL;
	pCursor = pFeatureClass->Query();
	CPPUNIT_ASSERT(pCursor!=NULL);

	auge::Geometry	*pGeometry = NULL;
	auge::Feature	*pFeature = NULL;
	while((pFeature=pCursor->NextFeature())!=NULL)
	{	
		pGeometry = pFeature->GetGeometry();
		pCanvas->DrawSymbol(pGeometry, pFillSymbol);
		pFeature->Release();
	}

	te = GetTickCount();
	printf("[时间]:%8.3f\n", (te-ts)/1000.0f);

	pCanvas->Save("g:\\temp\\map\\country.png");
	//pCanvas->Save("/home/renyc/map/map.png");

	AUGE_SAFE_RELEASE(pFillSymbol);
	AUGE_SAFE_RELEASE(pCursor);
	AUGE_SAFE_RELEASE(pFeatureClass);
	AUGE_SAFE_RELEASE(pCanvas);
}

auge::Style* SymbolTest::LoadSLD(const char* path)
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

void SymbolTest::SymbolMangerTest()
{
	
}

void SymbolTest::AddSymbolFillTest()
{
	auge::SymbolManager* pSymbolManager = auge::augeGetSymbolManagerInstance();
	//pSymbolManager->AddMarkerSymbol("民生银行", "E:\\Research\\Auge.GIS\\Dist\\32_x86_win_vc10\\binD\\symbol\\graphic\\marker\\民生银行.png");
	//pSymbolManager->AddMarkerSymbol("大学学院", "E:\\Research\\Auge.GIS\\Dist\\32_x86_win_vc10\\binD\\symbol\\graphic\\marker\\大学学院.png");
	//pSymbolManager->AddMarkerSymbol("011",                                       "G:\\temp\\marker\\011.png");
	//pSymbolManager->AddMarkerSymbol("033",                                       "G:\\temp\\marker\\033.png");
	//pSymbolManager->AddMarkerSymbol("046",                                       "G:\\temp\\marker\\046.png");
	//pSymbolManager->AddMarkerSymbol("051",                                       "G:\\temp\\marker\\051.png");
	//pSymbolManager->AddMarkerSymbol("058",                                       "G:\\temp\\marker\\058.png");
	//pSymbolManager->AddMarkerSymbol("060",                                       "G:\\temp\\marker\\060.png");
	//pSymbolManager->AddMarkerSymbol("061",                                       "G:\\temp\\marker\\061.png");
	//pSymbolManager->AddMarkerSymbol("063",                                       "G:\\temp\\marker\\063.png");
	//pSymbolManager->AddMarkerSymbol("080",                                       "G:\\temp\\marker\\080.png");
	//pSymbolManager->AddMarkerSymbol("building",                                  "G:\\temp\\marker\\building.png");
	//pSymbolManager->AddMarkerSymbol("cinema",                                    "G:\\temp\\marker\\cinema.png");
	//pSymbolManager->AddMarkerSymbol("garbage",                                   "G:\\temp\\marker\\garbage.png");
	//pSymbolManager->AddMarkerSymbol("ktv",                                       "G:\\temp\\marker\\ktv.png");
	//pSymbolManager->AddMarkerSymbol("school",                                    "G:\\temp\\marker\\school.png");
	//pSymbolManager->AddMarkerSymbol("train_station",                             "G:\\temp\\marker\\train_station.png");
	//pSymbolManager->AddMarkerSymbol("中信银行",                                  "G:\\temp\\marker\\中信银行.png");
	//pSymbolManager->AddMarkerSymbol("中国人民银行",                              "G:\\temp\\marker\\中国人民银行.png");
	//pSymbolManager->AddMarkerSymbol("中国移动",                                  "G:\\temp\\marker\\中国移动.png");
	//pSymbolManager->AddMarkerSymbol("中国银行",                                  "G:\\temp\\marker\\中国银行.png");
	//pSymbolManager->AddMarkerSymbol("交通银行",                                  "G:\\temp\\marker\\交通银行.png");
	//pSymbolManager->AddMarkerSymbol("体育",                                      "G:\\temp\\marker\\体育.png");
	//pSymbolManager->AddMarkerSymbol("体育馆",                                    "G:\\temp\\marker\\体育馆.png");
	//pSymbolManager->AddMarkerSymbol("停车场",                                    "G:\\temp\\marker\\停车场.png");
	//pSymbolManager->AddMarkerSymbol("公交",                                      "G:\\temp\\marker\\公交.png");
	//pSymbolManager->AddMarkerSymbol("公安",                                      "G:\\temp\\marker\\公安.png");
	//pSymbolManager->AddMarkerSymbol("兴业银行",                                  "G:\\temp\\marker\\兴业银行.png");
	//pSymbolManager->AddMarkerSymbol("兽医站",                                    "G:\\temp\\marker\\兽医站.png");
	//pSymbolManager->AddMarkerSymbol("农业银行",                                  "G:\\temp\\marker\\农业银行.png");
	//pSymbolManager->AddMarkerSymbol("农村商业银行",                              "G:\\temp\\marker\\农村商业银行.png");
	//pSymbolManager->AddMarkerSymbol("加油站",                                    "G:\\temp\\marker\\加油站.png");
	//pSymbolManager->AddMarkerSymbol("动物园",                                    "G:\\temp\\marker\\动物园.png");
	//pSymbolManager->AddMarkerSymbol("北京银行",                                  "G:\\temp\\marker\\北京银行.png");
	//pSymbolManager->AddMarkerSymbol("医院门诊",                                  "G:\\temp\\marker\\医院门诊.png");
	//pSymbolManager->AddMarkerSymbol("华夏",                                      "G:\\temp\\marker\\华夏.png");
	//pSymbolManager->AddMarkerSymbol("博物馆",                                    "G:\\temp\\marker\\博物馆.png");
	//pSymbolManager->AddMarkerSymbol("厕所",                                      "G:\\temp\\marker\\厕所.png");
	//pSymbolManager->AddMarkerSymbol("售票处",                                    "G:\\temp\\marker\\售票处.png");
	//pSymbolManager->AddMarkerSymbol("地铁出口",                                  "G:\\temp\\marker\\地铁出口.png");
	//pSymbolManager->AddMarkerSymbol("地铁站",                                    "G:\\temp\\marker\\地铁站.png");
	//pSymbolManager->AddMarkerSymbol("垃圾站",                                    "G:\\temp\\marker\\垃圾站.png");
	//pSymbolManager->AddMarkerSymbol("墓碑",                                      "G:\\temp\\marker\\墓碑.png");
	//pSymbolManager->AddMarkerSymbol("大厦",                                      "G:\\temp\\marker\\大厦.png");
	//pSymbolManager->AddMarkerSymbol("大学学院",                                  "G:\\temp\\marker\\大学学院.png");
	//pSymbolManager->AddMarkerSymbol("客运站",                                    "G:\\temp\\marker\\客运站.png");
	//pSymbolManager->AddMarkerSymbol("宾馆旅馆",                                  "G:\\temp\\marker\\宾馆旅馆.png");
	//pSymbolManager->AddMarkerSymbol("小区",                                      "G:\\temp\\marker\\小区.png");
	//pSymbolManager->AddMarkerSymbol("工商银行",                                  "G:\\temp\\marker\\工商银行.png");
	//pSymbolManager->AddMarkerSymbol("市场",                                      "G:\\temp\\marker\\市场.png");
	//pSymbolManager->AddMarkerSymbol("幼儿园小学",                                "G:\\temp\\marker\\幼儿园小学.png");
	//pSymbolManager->AddMarkerSymbol("广发银行",                                  "G:\\temp\\marker\\广发银行.png");
	//pSymbolManager->AddMarkerSymbol("广大银行",                                  "G:\\temp\\marker\\广大银行.png");
	//pSymbolManager->AddMarkerSymbol("建设银行",                                  "G:\\temp\\marker\\建设银行.png");
	//pSymbolManager->AddMarkerSymbol("报厅",                                      "G:\\temp\\marker\\报厅.png");
	//pSymbolManager->AddMarkerSymbol("招商银行",                                  "G:\\temp\\marker\\招商银行.png");
	//pSymbolManager->AddMarkerSymbol("收费站",                                    "G:\\temp\\marker\\收费站.png");
	//pSymbolManager->AddMarkerSymbol("政府机构",                                  "G:\\temp\\marker\\政府机构.png");
	//pSymbolManager->AddMarkerSymbol("服务区",                                    "G:\\temp\\marker\\服务区.png");
	//pSymbolManager->AddMarkerSymbol("民生银行",                                  "G:\\temp\\marker\\民生银行.png");
	//pSymbolManager->AddMarkerSymbol("洗衣房",                                    "G:\\temp\\marker\\洗衣房.png");
	//pSymbolManager->AddMarkerSymbol("浦发银行",                                  "G:\\temp\\marker\\浦发银行.png");
	//pSymbolManager->AddMarkerSymbol("游泳",                                      "G:\\temp\\marker\\游泳.png");
	//pSymbolManager->AddMarkerSymbol("火车站",                                    "G:\\temp\\marker\\火车站.png");
	//pSymbolManager->AddMarkerSymbol("电信",                                      "G:\\temp\\marker\\电信.png");
	//pSymbolManager->AddMarkerSymbol("电影院",                                    "G:\\temp\\marker\\电影院.png");
	//pSymbolManager->AddMarkerSymbol("码头",                                      "G:\\temp\\marker\\码头.png");
	//pSymbolManager->AddMarkerSymbol("科学科技馆",                                "G:\\temp\\marker\\科学科技馆.png");
	//pSymbolManager->AddMarkerSymbol("科研",                                      "G:\\temp\\marker\\科研.png");
	//pSymbolManager->AddMarkerSymbol("维修",                                      "G:\\temp\\marker\\维修.png");
	//pSymbolManager->AddMarkerSymbol("联通",                                      "G:\\temp\\marker\\联通.png");
	//pSymbolManager->AddMarkerSymbol("花园公圆",                                  "G:\\temp\\marker\\花园公圆.png");
	//pSymbolManager->AddMarkerSymbol("茶咖啡饮品",                                "G:\\temp\\marker\\茶咖啡饮品.png");
	//pSymbolManager->AddMarkerSymbol("药房",                                      "G:\\temp\\marker\\药房.png");
	//pSymbolManager->AddMarkerSymbol("西站D口",                                   "G:\\temp\\marker\\西站D口.png");
	//pSymbolManager->AddMarkerSymbol("警卫厅",                                    "G:\\temp\\marker\\警卫厅.png");
	//pSymbolManager->AddMarkerSymbol("超市",                                      "G:\\temp\\marker\\超市.png");
	//pSymbolManager->AddMarkerSymbol("车",                                        "G:\\temp\\marker\\车.png");
	//pSymbolManager->AddMarkerSymbol("邮局",                                      "G:\\temp\\marker\\邮局.png");
	//pSymbolManager->AddMarkerSymbol("邮政",                                      "G:\\temp\\marker\\邮政.png");
	//pSymbolManager->AddMarkerSymbol("酒吧",                                      "G:\\temp\\marker\\酒吧.png");
	//pSymbolManager->AddMarkerSymbol("飞机场",                                    "G:\\temp\\marker\\飞机场.png");
	//pSymbolManager->AddMarkerSymbol("饭店餐馆",                                  "G:\\temp\\marker\\饭店餐馆.png");
	//pSymbolManager->AddMarkerSymbol("饭馆",                                      "G:\\temp\\marker\\饭馆.png");
	//pSymbolManager->AddMarkerSymbol("高尔夫",                                    "G:\\temp\\marker\\高尔夫.png");
	//pSymbolManager->AddMarkerSymbol("高速出口",                                  "G:\\temp\\marker\\高速出口.png");
	//pSymbolManager->AddMarkerSymbol("高速进口",                                  "G:\\temp\\marker\\高速进口.png");

		pSymbolManager->AddFillSymbol("3001"            ,"g:\\temp\\fill\\3001.png");                  
	pSymbolManager->AddFillSymbol("BeddedLimestone" ,"g:\\temp\\fill\\BeddedLimestone.png");       
	pSymbolManager->AddFillSymbol("brick1"          ,"g:\\temp\\fill\\brick1.png");                
	pSymbolManager->AddFillSymbol("cemetery"        ,"g:\\temp\\fill\\cemetery.png");              
	pSymbolManager->AddFillSymbol("check"           ,"g:\\temp\\fill\\check.png");                 
	pSymbolManager->AddFillSymbol("Chert"           ,"g:\\temp\\fill\\Chert.png");                 
	pSymbolManager->AddFillSymbol("Chert02"         ,"g:\\temp\\fill\\Chert02.png");               
	pSymbolManager->AddFillSymbol("ChertyLimestone" ,"g:\\temp\\fill\\ChertyLimestone.png");       
	pSymbolManager->AddFillSymbol("circle1"         ,"g:\\temp\\fill\\circle1.png");               
	pSymbolManager->AddFillSymbol("circle2"         ,"g:\\temp\\fill\\circle2.png");               
	pSymbolManager->AddFillSymbol("circle3"         ,"g:\\temp\\fill\\circle3.png");               
	pSymbolManager->AddFillSymbol("circle4"         ,"g:\\temp\\fill\\circle4.png");               
	pSymbolManager->AddFillSymbol("circle5"         ,"g:\\temp\\fill\\circle5.png");               
	pSymbolManager->AddFillSymbol("circle6"         ,"g:\\temp\\fill\\circle6.png");               
	pSymbolManager->AddFillSymbol("circle7"         ,"g:\\temp\\fill\\circle7.png");               
	pSymbolManager->AddFillSymbol("cropland"        ,"g:\\temp\\fill\\cropland.png");              
	pSymbolManager->AddFillSymbol("dot5"            ,"g:\\temp\\fill\\dot5.png");                  
	pSymbolManager->AddFillSymbol("dot6"            ,"g:\\temp\\fill\\dot6.png");                  
	pSymbolManager->AddFillSymbol("glacier"         ,"g:\\temp\\fill\\glacier.png");               
	pSymbolManager->AddFillSymbol("grass"           ,"g:\\temp\\fill\\grass.png");                 
	pSymbolManager->AddFillSymbol("grass1"          ,"g:\\temp\\fill\\grass1.png");                
	pSymbolManager->AddFillSymbol("grass2"          ,"g:\\temp\\fill\\grass2.png");                
	pSymbolManager->AddFillSymbol("grass3"          ,"g:\\temp\\fill\\grass3.png");                
	pSymbolManager->AddFillSymbol("gravel"          ,"g:\\temp\\fill\\gravel.png");                
	pSymbolManager->AddFillSymbol("hacshd11"        ,"g:\\temp\\fill\\hacshd11.png");              
	pSymbolManager->AddFillSymbol("hacshd20"        ,"g:\\temp\\fill\\hacshd20.png");              
	pSymbolManager->AddFillSymbol("hacshd23"        ,"g:\\temp\\fill\\hacshd23.png");              
	pSymbolManager->AddFillSymbol("hacshd24"        ,"g:\\temp\\fill\\hacshd24.png");              
	pSymbolManager->AddFillSymbol("hacshd25"        ,"g:\\temp\\fill\\hacshd25.png");              
	pSymbolManager->AddFillSymbol("hacshd26"        ,"g:\\temp\\fill\\hacshd26.png");              
	pSymbolManager->AddFillSymbol("hacshd28"        ,"g:\\temp\\fill\\hacshd28.png");              
	pSymbolManager->AddFillSymbol("hacshd29"        ,"g:\\temp\\fill\\hacshd29.png");              
	pSymbolManager->AddFillSymbol("hacshd30"        ,"g:\\temp\\fill\\hacshd30.png");              
	pSymbolManager->AddFillSymbol("hacshd31"        ,"g:\\temp\\fill\\hacshd31.png");              
	pSymbolManager->AddFillSymbol("hacshd32"        ,"g:\\temp\\fill\\hacshd32.png");              
	pSymbolManager->AddFillSymbol("hacshd33"        ,"g:\\temp\\fill\\hacshd33.png");              
	pSymbolManager->AddFillSymbol("hacshd34"        ,"g:\\temp\\fill\\hacshd34.png");              
	pSymbolManager->AddFillSymbol("hacshd35"        ,"g:\\temp\\fill\\hacshd35.png");              
	pSymbolManager->AddFillSymbol("hacshd36"        ,"g:\\temp\\fill\\hacshd36.png");              
	pSymbolManager->AddFillSymbol("herring1"        ,"g:\\temp\\fill\\herring1.png");              
	pSymbolManager->AddFillSymbol("herring2"        ,"g:\\temp\\fill\\herring2.png");              
	pSymbolManager->AddFillSymbol("herring3"        ,"g:\\temp\\fill\\herring3.png");              
	pSymbolManager->AddFillSymbol("historic"        ,"g:\\temp\\fill\\historic.png");              
	pSymbolManager->AddFillSymbol("historic2"       ,"g:\\temp\\fill\\historic2.png");             
	pSymbolManager->AddFillSymbol("mangrove"        ,"g:\\temp\\fill\\mangrove.png");              
	pSymbolManager->AddFillSymbol("mangrove1"       ,"g:\\temp\\fill\\mangrove1.png");             
	pSymbolManager->AddFillSymbol("mangrove2"       ,"g:\\temp\\fill\\mangrove2.png");             
	pSymbolManager->AddFillSymbol("marsh"           ,"g:\\temp\\fill\\marsh.png");                 
	pSymbolManager->AddFillSymbol("mud"             ,"g:\\temp\\fill\\mud.png");                   
	pSymbolManager->AddFillSymbol("open_pasture"    ,"g:\\temp\\fill\\open_pasture.png");          
	pSymbolManager->AddFillSymbol("orchard"         ,"g:\\temp\\fill\\orchard.png");               
	pSymbolManager->AddFillSymbol("ordered10"       ,"g:\\temp\\fill\\ordered10.png");             
	pSymbolManager->AddFillSymbol("ordered20"       ,"g:\\temp\\fill\\ordered20.png");             
	pSymbolManager->AddFillSymbol("ordered30"       ,"g:\\temp\\fill\\ordered30.png");             
	pSymbolManager->AddFillSymbol("ordered40"       ,"g:\\temp\\fill\\ordered40.png");             
	pSymbolManager->AddFillSymbol("ordered50"       ,"g:\\temp\\fill\\ordered50.png");             
	pSymbolManager->AddFillSymbol("ordered60"       ,"g:\\temp\\fill\\ordered60.png");             
	pSymbolManager->AddFillSymbol("ordered70"       ,"g:\\temp\\fill\\ordered70.png");             
	pSymbolManager->AddFillSymbol("ordered80"       ,"g:\\temp\\fill\\ordered80.png");             
	pSymbolManager->AddFillSymbol("ordered90"       ,"g:\\temp\\fill\\ordered90.png");             
	pSymbolManager->AddFillSymbol("pat010"          ,"g:\\temp\\fill\\pat010.png");                
	pSymbolManager->AddFillSymbol("pat011"          ,"g:\\temp\\fill\\pat011.png");                
	pSymbolManager->AddFillSymbol("pat012"          ,"g:\\temp\\fill\\pat012.png");                
	pSymbolManager->AddFillSymbol("pat013"          ,"g:\\temp\\fill\\pat013.png");                
	pSymbolManager->AddFillSymbol("pat014"          ,"g:\\temp\\fill\\pat014.png");                
	pSymbolManager->AddFillSymbol("pat015"          ,"g:\\temp\\fill\\pat015.png");                
	pSymbolManager->AddFillSymbol("pat016"          ,"g:\\temp\\fill\\pat016.png");                
	pSymbolManager->AddFillSymbol("pat017"          ,"g:\\temp\\fill\\pat017.png");                
	pSymbolManager->AddFillSymbol("pat018"          ,"g:\\temp\\fill\\pat018.png");                
	pSymbolManager->AddFillSymbol("pat019"          ,"g:\\temp\\fill\\pat019.png");                
	pSymbolManager->AddFillSymbol("pat020"          ,"g:\\temp\\fill\\pat020.png");                
	pSymbolManager->AddFillSymbol("pat023"          ,"g:\\temp\\fill\\pat023.png");                
	pSymbolManager->AddFillSymbol("pat024"          ,"g:\\temp\\fill\\pat024.png");                
	pSymbolManager->AddFillSymbol("pat025"          ,"g:\\temp\\fill\\pat025.png");                
	pSymbolManager->AddFillSymbol("pat026"          ,"g:\\temp\\fill\\pat026.png");                
	pSymbolManager->AddFillSymbol("pat027"          ,"g:\\temp\\fill\\pat027.png");                
	pSymbolManager->AddFillSymbol("pat028"          ,"g:\\temp\\fill\\pat028.png");                
	pSymbolManager->AddFillSymbol("pat029"          ,"g:\\temp\\fill\\pat029.png");                
	pSymbolManager->AddFillSymbol("pat030"          ,"g:\\temp\\fill\\pat030.png");                
	pSymbolManager->AddFillSymbol("pat031"          ,"g:\\temp\\fill\\pat031.png");                
	pSymbolManager->AddFillSymbol("pat032"          ,"g:\\temp\\fill\\pat032.png");                
	pSymbolManager->AddFillSymbol("pat033"          ,"g:\\temp\\fill\\pat033.png");                
	pSymbolManager->AddFillSymbol("pat034"          ,"g:\\temp\\fill\\pat034.png");                
	pSymbolManager->AddFillSymbol("pat035"          ,"g:\\temp\\fill\\pat035.png");                
	pSymbolManager->AddFillSymbol("pat036"          ,"g:\\temp\\fill\\pat036.png");                
	pSymbolManager->AddFillSymbol("pat037"          ,"g:\\temp\\fill\\pat037.png");                
	pSymbolManager->AddFillSymbol("pat038"          ,"g:\\temp\\fill\\pat038.png");                
	pSymbolManager->AddFillSymbol("pat039"          ,"g:\\temp\\fill\\pat039.png");                
	pSymbolManager->AddFillSymbol("pat040"          ,"g:\\temp\\fill\\pat040.png");                
	pSymbolManager->AddFillSymbol("pat041"          ,"g:\\temp\\fill\\pat041.png");                
	pSymbolManager->AddFillSymbol("pat042"          ,"g:\\temp\\fill\\pat042.png");                
	pSymbolManager->AddFillSymbol("pat043"          ,"g:\\temp\\fill\\pat043.png");                
	pSymbolManager->AddFillSymbol("pat044"          ,"g:\\temp\\fill\\pat044.png");                
	pSymbolManager->AddFillSymbol("pat045"          ,"g:\\temp\\fill\\pat045.png");                
	pSymbolManager->AddFillSymbol("pat046"          ,"g:\\temp\\fill\\pat046.png");                
	pSymbolManager->AddFillSymbol("pat047"          ,"g:\\temp\\fill\\pat047.png");                
	pSymbolManager->AddFillSymbol("pat048"          ,"g:\\temp\\fill\\pat048.png");                
	pSymbolManager->AddFillSymbol("pat049"          ,"g:\\temp\\fill\\pat049.png");                
	pSymbolManager->AddFillSymbol("pat050"          ,"g:\\temp\\fill\\pat050.png");                
	pSymbolManager->AddFillSymbol("pat051"          ,"g:\\temp\\fill\\pat051.png");                
	pSymbolManager->AddFillSymbol("pat052"          ,"g:\\temp\\fill\\pat052.png");                
	pSymbolManager->AddFillSymbol("pat053"          ,"g:\\temp\\fill\\pat053.png");                
	pSymbolManager->AddFillSymbol("pat054"          ,"g:\\temp\\fill\\pat054.png");                
	pSymbolManager->AddFillSymbol("pat055"          ,"g:\\temp\\fill\\pat055.png");                
	pSymbolManager->AddFillSymbol("pat056"          ,"g:\\temp\\fill\\pat056.png");                
	pSymbolManager->AddFillSymbol("pat057"          ,"g:\\temp\\fill\\pat057.png");                
	pSymbolManager->AddFillSymbol("pat058"          ,"g:\\temp\\fill\\pat058.png");                
	pSymbolManager->AddFillSymbol("pat059"          ,"g:\\temp\\fill\\pat059.png");                
	pSymbolManager->AddFillSymbol("pat060"          ,"g:\\temp\\fill\\pat060.png");                
	pSymbolManager->AddFillSymbol("pat061"          ,"g:\\temp\\fill\\pat061.png");                
	pSymbolManager->AddFillSymbol("pat062"          ,"g:\\temp\\fill\\pat062.png");                
	pSymbolManager->AddFillSymbol("pat063"          ,"g:\\temp\\fill\\pat063.png");                
	pSymbolManager->AddFillSymbol("pat064"          ,"g:\\temp\\fill\\pat064.png");                
	pSymbolManager->AddFillSymbol("pat065"          ,"g:\\temp\\fill\\pat065.png");                
	pSymbolManager->AddFillSymbol("pat066"          ,"g:\\temp\\fill\\pat066.png");                
	pSymbolManager->AddFillSymbol("pat067"          ,"g:\\temp\\fill\\pat067.png");                
	pSymbolManager->AddFillSymbol("pat068"          ,"g:\\temp\\fill\\pat068.png");                
	pSymbolManager->AddFillSymbol("pat069"          ,"g:\\temp\\fill\\pat069.png");                
	pSymbolManager->AddFillSymbol("pat070"          ,"g:\\temp\\fill\\pat070.png");                
	pSymbolManager->AddFillSymbol("pat071"          ,"g:\\temp\\fill\\pat071.png");                
	pSymbolManager->AddFillSymbol("pat072"          ,"g:\\temp\\fill\\pat072.png");                
	pSymbolManager->AddFillSymbol("pat073"          ,"g:\\temp\\fill\\pat073.png");                
	pSymbolManager->AddFillSymbol("pat074"          ,"g:\\temp\\fill\\pat074.png");                
	pSymbolManager->AddFillSymbol("pat075"          ,"g:\\temp\\fill\\pat075.png");                
	pSymbolManager->AddFillSymbol("pat076"          ,"g:\\temp\\fill\\pat076.png");                
	pSymbolManager->AddFillSymbol("pat077"          ,"g:\\temp\\fill\\pat077.png");                
	pSymbolManager->AddFillSymbol("pat078"          ,"g:\\temp\\fill\\pat078.png");                
	pSymbolManager->AddFillSymbol("pat079"          ,"g:\\temp\\fill\\pat079.png");                
	pSymbolManager->AddFillSymbol("rice"            ,"g:\\temp\\fill\\rice.png");                  
	pSymbolManager->AddFillSymbol("rice1"           ,"g:\\temp\\fill\\rice1.png");                 
	pSymbolManager->AddFillSymbol("rice2"           ,"g:\\temp\\fill\\rice2.png");                 
	pSymbolManager->AddFillSymbol("sand"            ,"g:\\temp\\fill\\sand.png");                  
	pSymbolManager->AddFillSymbol("sand1"           ,"g:\\temp\\fill\\sand1.png");                 
	pSymbolManager->AddFillSymbol("sand2"           ,"g:\\temp\\fill\\sand2.png");                 
	pSymbolManager->AddFillSymbol("sand3"           ,"g:\\temp\\fill\\sand3.png");                 
	pSymbolManager->AddFillSymbol("sand4"           ,"g:\\temp\\fill\\sand4.png");                 
	pSymbolManager->AddFillSymbol("scale1"          ,"g:\\temp\\fill\\scale1.png");                
	pSymbolManager->AddFillSymbol("scale2"          ,"g:\\temp\\fill\\scale2.png");                
	pSymbolManager->AddFillSymbol("scrub"           ,"g:\\temp\\fill\\scrub.png");                 
	pSymbolManager->AddFillSymbol("scrub2"          ,"g:\\temp\\fill\\scrub2.png");                
	pSymbolManager->AddFillSymbol("shd100"          ,"g:\\temp\\fill\\shd100.png");                
	pSymbolManager->AddFillSymbol("shd101"          ,"g:\\temp\\fill\\shd101.png");                
	pSymbolManager->AddFillSymbol("shd102"          ,"g:\\temp\\fill\\shd102.png");                
	pSymbolManager->AddFillSymbol("shd103"          ,"g:\\temp\\fill\\shd103.png");                
	pSymbolManager->AddFillSymbol("shd104"          ,"g:\\temp\\fill\\shd104.png");                
	pSymbolManager->AddFillSymbol("shd105"          ,"g:\\temp\\fill\\shd105.png");                
	pSymbolManager->AddFillSymbol("shd106"          ,"g:\\temp\\fill\\shd106.png");                
	pSymbolManager->AddFillSymbol("shd115"          ,"g:\\temp\\fill\\shd115.png");                
	pSymbolManager->AddFillSymbol("shd123"          ,"g:\\temp\\fill\\shd123.png");                
	pSymbolManager->AddFillSymbol("shd130"          ,"g:\\temp\\fill\\shd130.png");                
	pSymbolManager->AddFillSymbol("shd131"          ,"g:\\temp\\fill\\shd131.png");                
	pSymbolManager->AddFillSymbol("shd132"          ,"g:\\temp\\fill\\shd132.png");                
	pSymbolManager->AddFillSymbol("shd133"          ,"g:\\temp\\fill\\shd133.png");                
	pSymbolManager->AddFillSymbol("shd134"          ,"g:\\temp\\fill\\shd134.png");                
	pSymbolManager->AddFillSymbol("shd135"          ,"g:\\temp\\fill\\shd135.png");                
	pSymbolManager->AddFillSymbol("shd136"          ,"g:\\temp\\fill\\shd136.png");                
	pSymbolManager->AddFillSymbol("shd137"          ,"g:\\temp\\fill\\shd137.png");                
	pSymbolManager->AddFillSymbol("shd138"          ,"g:\\temp\\fill\\shd138.png");                
	pSymbolManager->AddFillSymbol("shd139"          ,"g:\\temp\\fill\\shd139.png");                
	pSymbolManager->AddFillSymbol("shd17"           ,"g:\\temp\\fill\\shd17.png");                 
	pSymbolManager->AddFillSymbol("shd24"           ,"g:\\temp\\fill\\shd24.png");                 
	pSymbolManager->AddFillSymbol("shd25"           ,"g:\\temp\\fill\\shd25.png");                 
	pSymbolManager->AddFillSymbol("shd28"           ,"g:\\temp\\fill\\shd28.png");                 
	pSymbolManager->AddFillSymbol("shd30"           ,"g:\\temp\\fill\\shd30.png");                 
	pSymbolManager->AddFillSymbol("shd31"           ,"g:\\temp\\fill\\shd31.png");                 
	pSymbolManager->AddFillSymbol("shd32"           ,"g:\\temp\\fill\\shd32.png");                 
	pSymbolManager->AddFillSymbol("shd33"           ,"g:\\temp\\fill\\shd33.png");                 
	pSymbolManager->AddFillSymbol("shd34"           ,"g:\\temp\\fill\\shd34.png");                 
	pSymbolManager->AddFillSymbol("shd35"           ,"g:\\temp\\fill\\shd35.png");                 
	pSymbolManager->AddFillSymbol("shd36"           ,"g:\\temp\\fill\\shd36.png");                 
	pSymbolManager->AddFillSymbol("shd37"           ,"g:\\temp\\fill\\shd37.png");                 
	pSymbolManager->AddFillSymbol("shd38"           ,"g:\\temp\\fill\\shd38.png");                 
	pSymbolManager->AddFillSymbol("shd39"           ,"g:\\temp\\fill\\shd39.png");                 
	pSymbolManager->AddFillSymbol("shd40"           ,"g:\\temp\\fill\\shd40.png");                 
	pSymbolManager->AddFillSymbol("shd41"           ,"g:\\temp\\fill\\shd41.png");                 
	pSymbolManager->AddFillSymbol("shd42"           ,"g:\\temp\\fill\\shd42.png");                 
	pSymbolManager->AddFillSymbol("shd43"           ,"g:\\temp\\fill\\shd43.png");                 
	pSymbolManager->AddFillSymbol("shd44"           ,"g:\\temp\\fill\\shd44.png");                 
	pSymbolManager->AddFillSymbol("shd47"           ,"g:\\temp\\fill\\shd47.png");                 
	pSymbolManager->AddFillSymbol("shd48"           ,"g:\\temp\\fill\\shd48.png");                 
	pSymbolManager->AddFillSymbol("shd51"           ,"g:\\temp\\fill\\shd51.png");                 
	pSymbolManager->AddFillSymbol("shd52"           ,"g:\\temp\\fill\\shd52.png");                 
	pSymbolManager->AddFillSymbol("shd53"           ,"g:\\temp\\fill\\shd53.png");                 
	pSymbolManager->AddFillSymbol("shd54"           ,"g:\\temp\\fill\\shd54.png");                 
	pSymbolManager->AddFillSymbol("shd55"           ,"g:\\temp\\fill\\shd55.png");                 
	pSymbolManager->AddFillSymbol("shd56"           ,"g:\\temp\\fill\\shd56.png");                 
	pSymbolManager->AddFillSymbol("shd57"           ,"g:\\temp\\fill\\shd57.png");                 
	pSymbolManager->AddFillSymbol("shd58"           ,"g:\\temp\\fill\\shd58.png");                 
	pSymbolManager->AddFillSymbol("shd59"           ,"g:\\temp\\fill\\shd59.png");                 
	pSymbolManager->AddFillSymbol("shd60"           ,"g:\\temp\\fill\\shd60.png");                 
	pSymbolManager->AddFillSymbol("shd61"           ,"g:\\temp\\fill\\shd61.png");                 
	pSymbolManager->AddFillSymbol("shd62"           ,"g:\\temp\\fill\\shd62.png");                 
	pSymbolManager->AddFillSymbol("shd63"           ,"g:\\temp\\fill\\shd63.png");                 
	pSymbolManager->AddFillSymbol("shd71"           ,"g:\\temp\\fill\\shd71.png");                 
	pSymbolManager->AddFillSymbol("shd72"           ,"g:\\temp\\fill\\shd72.png");                 
	pSymbolManager->AddFillSymbol("shd73"           ,"g:\\temp\\fill\\shd73.png");                 
	pSymbolManager->AddFillSymbol("shd74"           ,"g:\\temp\\fill\\shd74.png");                 
	pSymbolManager->AddFillSymbol("shd75"           ,"g:\\temp\\fill\\shd75.png");                 
	pSymbolManager->AddFillSymbol("shd76"           ,"g:\\temp\\fill\\shd76.png");                 
	pSymbolManager->AddFillSymbol("shd77"           ,"g:\\temp\\fill\\shd77.png");                 
	pSymbolManager->AddFillSymbol("shd78"           ,"g:\\temp\\fill\\shd78.png");                 
	pSymbolManager->AddFillSymbol("shd81"           ,"g:\\temp\\fill\\shd81.png");                 
	pSymbolManager->AddFillSymbol("shd82"           ,"g:\\temp\\fill\\shd82.png");                 
	pSymbolManager->AddFillSymbol("shd90"           ,"g:\\temp\\fill\\shd90.png");                 
	pSymbolManager->AddFillSymbol("shd91"           ,"g:\\temp\\fill\\shd91.png");                 
	pSymbolManager->AddFillSymbol("shd93"           ,"g:\\temp\\fill\\shd93.png");                 
	pSymbolManager->AddFillSymbol("shingle"         ,"g:\\temp\\fill\\shingle.png");               
	pSymbolManager->AddFillSymbol("smallwave"       ,"g:\\temp\\fill\\smallwave.png");             
	pSymbolManager->AddFillSymbol("smallwave1"      ,"g:\\temp\\fill\\smallwave1.png");            
	pSymbolManager->AddFillSymbol("smallwave2"      ,"g:\\temp\\fill\\smallwave2.png");            
	pSymbolManager->AddFillSymbol("snow_ice"        ,"g:\\temp\\fill\\snow_ice.png");              
	pSymbolManager->AddFillSymbol("surface1"        ,"g:\\temp\\fill\\surface1.png");              
	pSymbolManager->AddFillSymbol("surface2"        ,"g:\\temp\\fill\\surface2.png");              
	pSymbolManager->AddFillSymbol("swamp"           ,"g:\\temp\\fill\\swamp.png");                 
	pSymbolManager->AddFillSymbol("swamp1"          ,"g:\\temp\\fill\\swamp1.png");                
	pSymbolManager->AddFillSymbol("tailings"        ,"g:\\temp\\fill\\tailings.png");              
	pSymbolManager->AddFillSymbol("tic2"            ,"g:\\temp\\fill\\tic2.png");                  
	pSymbolManager->AddFillSymbol("trees01"         ,"g:\\temp\\fill\\trees01.png");               
	pSymbolManager->AddFillSymbol("trees02"         ,"g:\\temp\\fill\\trees02.png");               
	pSymbolManager->AddFillSymbol("trees03"         ,"g:\\temp\\fill\\trees03.png");               
	pSymbolManager->AddFillSymbol("trees04"         ,"g:\\temp\\fill\\trees04.png");               
	pSymbolManager->AddFillSymbol("trees05"         ,"g:\\temp\\fill\\trees05.png");               
	pSymbolManager->AddFillSymbol("trees06"         ,"g:\\temp\\fill\\trees06.png");               
	pSymbolManager->AddFillSymbol("trees07"         ,"g:\\temp\\fill\\trees07.png");               
	pSymbolManager->AddFillSymbol("trees08"         ,"g:\\temp\\fill\\trees08.png");               
	pSymbolManager->AddFillSymbol("trees09"         ,"g:\\temp\\fill\\trees09.png");               
	pSymbolManager->AddFillSymbol("trees10"         ,"g:\\temp\\fill\\trees10.png");               
	pSymbolManager->AddFillSymbol("trees11"         ,"g:\\temp\\fill\\trees11.png");               
	pSymbolManager->AddFillSymbol("trees12"         ,"g:\\temp\\fill\\trees12.png");               
	pSymbolManager->AddFillSymbol("trees13"         ,"g:\\temp\\fill\\trees13.png");               
	pSymbolManager->AddFillSymbol("trees14"         ,"g:\\temp\\fill\\trees14.png");               
	pSymbolManager->AddFillSymbol("trees15"         ,"g:\\temp\\fill\\trees15.png");               
	pSymbolManager->AddFillSymbol("trees16"         ,"g:\\temp\\fill\\trees16.png");               
	pSymbolManager->AddFillSymbol("trees17"         ,"g:\\temp\\fill\\trees17.png");               
	pSymbolManager->AddFillSymbol("trees18"         ,"g:\\temp\\fill\\trees18.png");               
	pSymbolManager->AddFillSymbol("trees19"         ,"g:\\temp\\fill\\trees19.png");               
	pSymbolManager->AddFillSymbol("usda553"         ,"g:\\temp\\fill\\usda553.png");               
	pSymbolManager->AddFillSymbol("usdaRRR"         ,"g:\\temp\\fill\\usdaRRR.png");               
	pSymbolManager->AddFillSymbol("usgs317"         ,"g:\\temp\\fill\\usgs317.png");               
	pSymbolManager->AddFillSymbol("usgs323"         ,"g:\\temp\\fill\\usgs323.png");               
	pSymbolManager->AddFillSymbol("usgs401"         ,"g:\\temp\\fill\\usgs401.png");               
	pSymbolManager->AddFillSymbol("usgs502"         ,"g:\\temp\\fill\\usgs502.png");               
	pSymbolManager->AddFillSymbol("usgs503"         ,"g:\\temp\\fill\\usgs503.png");               
	pSymbolManager->AddFillSymbol("usgs504"         ,"g:\\temp\\fill\\usgs504.png");               
	pSymbolManager->AddFillSymbol("usgs505"         ,"g:\\temp\\fill\\usgs505.png");               
	pSymbolManager->AddFillSymbol("usgs506"         ,"g:\\temp\\fill\\usgs506.png");               
	pSymbolManager->AddFillSymbol("usgs601"         ,"g:\\temp\\fill\\usgs601.png");               
	pSymbolManager->AddFillSymbol("usgs602"         ,"g:\\temp\\fill\\usgs602.png");               
	pSymbolManager->AddFillSymbol("usgs603"         ,"g:\\temp\\fill\\usgs603.png");               
	pSymbolManager->AddFillSymbol("usgs604"         ,"g:\\temp\\fill\\usgs604.png");               
	pSymbolManager->AddFillSymbol("usgs605"         ,"g:\\temp\\fill\\usgs605.png");               
	pSymbolManager->AddFillSymbol("usgs606"         ,"g:\\temp\\fill\\usgs606.png");               
	pSymbolManager->AddFillSymbol("usgs607"         ,"g:\\temp\\fill\\usgs607.png");               
	pSymbolManager->AddFillSymbol("usgs612"         ,"g:\\temp\\fill\\usgs612.png");               
	pSymbolManager->AddFillSymbol("usgs613"         ,"g:\\temp\\fill\\usgs613.png");               
	pSymbolManager->AddFillSymbol("usgs614"         ,"g:\\temp\\fill\\usgs614.png");               
	pSymbolManager->AddFillSymbol("usgs616"         ,"g:\\temp\\fill\\usgs616.png");               
	pSymbolManager->AddFillSymbol("usgs617"         ,"g:\\temp\\fill\\usgs617.png");               
	pSymbolManager->AddFillSymbol("usgs619"         ,"g:\\temp\\fill\\usgs619.png");               
	pSymbolManager->AddFillSymbol("usgs620"         ,"g:\\temp\\fill\\usgs620.png");               
	pSymbolManager->AddFillSymbol("usgs627"         ,"g:\\temp\\fill\\usgs627.png");               
	pSymbolManager->AddFillSymbol("usgs630"         ,"g:\\temp\\fill\\usgs630.png");               
	pSymbolManager->AddFillSymbol("usgs636"         ,"g:\\temp\\fill\\usgs636.png");               
	pSymbolManager->AddFillSymbol("usgs637"         ,"g:\\temp\\fill\\usgs637.png");               
	pSymbolManager->AddFillSymbol("usgs638"         ,"g:\\temp\\fill\\usgs638.png");               
	pSymbolManager->AddFillSymbol("usgs642"         ,"g:\\temp\\fill\\usgs642.png");               
	pSymbolManager->AddFillSymbol("usgs648"         ,"g:\\temp\\fill\\usgs648.png");               
	pSymbolManager->AddFillSymbol("usgs649"         ,"g:\\temp\\fill\\usgs649.png");               
	pSymbolManager->AddFillSymbol("usgs669"         ,"g:\\temp\\fill\\usgs669.png");               
	pSymbolManager->AddFillSymbol("usgs677"         ,"g:\\temp\\fill\\usgs677.png");               
	pSymbolManager->AddFillSymbol("usgs701"         ,"g:\\temp\\fill\\usgs701.png");               
	pSymbolManager->AddFillSymbol("usgs702"         ,"g:\\temp\\fill\\usgs702.png");               
	pSymbolManager->AddFillSymbol("usgs711"         ,"g:\\temp\\fill\\usgs711.png");               
	pSymbolManager->AddFillSymbol("usgs712"         ,"g:\\temp\\fill\\usgs712.png");               
	pSymbolManager->AddFillSymbol("usgs713"         ,"g:\\temp\\fill\\usgs713.png");               
	pSymbolManager->AddFillSymbol("usgs716"         ,"g:\\temp\\fill\\usgs716.png");               
	pSymbolManager->AddFillSymbol("usgs717"         ,"g:\\temp\\fill\\usgs717.png");               
	pSymbolManager->AddFillSymbol("usgs721"         ,"g:\\temp\\fill\\usgs721.png");               
	pSymbolManager->AddFillSymbol("usgs723"         ,"g:\\temp\\fill\\usgs723.png");               
	pSymbolManager->AddFillSymbol("usgs724"         ,"g:\\temp\\fill\\usgs724.png");               
	pSymbolManager->AddFillSymbol("usgs725"         ,"g:\\temp\\fill\\usgs725.png");               
	pSymbolManager->AddFillSymbol("usgs728"         ,"g:\\temp\\fill\\usgs728.png");               
	pSymbolManager->AddFillSymbol("usgs731"         ,"g:\\temp\\fill\\usgs731.png");               
	pSymbolManager->AddFillSymbol("vineyard"        ,"g:\\temp\\fill\\vineyard.png");              
	pSymbolManager->AddFillSymbol("water_int"       ,"g:\\temp\\fill\\water_int.png");             
	pSymbolManager->AddFillSymbol("woodland"        ,"g:\\temp\\fill\\woodland.png");              
	pSymbolManager->AddFillSymbol("woodland1"       ,"g:\\temp\\fill\\woodland1.png");             
	pSymbolManager->AddFillSymbol("woodland2"       ,"g:\\temp\\fill\\woodland2.png");             

}