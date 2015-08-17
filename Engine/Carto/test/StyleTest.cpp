#include "StyleTest.h"
#include "AugeGeometry.h"
#include "AugeCore.h"
#include "AugeCarto.h"
#include "AugeFeature.h"
#include "AugeData.h"

//CPPUNIT_TEST_SUITE_REGISTRATION(StyleTest);

auge::Map*	CreateMapObj();
auge::Style* LoadSLD(const char* path);

void StyleTest::setUp() 
{
	auge::DataEngine* pEngine = NULL;
	auge::DataEngineManager* pEngineManager = NULL;
	pEngineManager = auge::augeGetDataEngineManagerInstance();
	pEngineManager->Load();
	pEngine = pEngineManager->GetEngine("Postgres");
	m_pConnection = pEngine->CreateConnection();
	//m_pConnection->SetConnectionString("SERVER=127.0.0.1;INSTANCE=5432;DATABASE=auge;USER=postgres;PASSWORD=qwer1234;ENCODING=GBK");
	m_pConnection->SetConnectionString("SERVER=192.168.111.160;INSTANCE=5432;DATABASE=auge;USER=postgres;PASSWORD=qwer1234;ENCODING=GBK");
	m_pConnection->Open();

	auge::CartoManager* pCartoManager = auge::augeGetCartoManagerInstance();
	pCartoManager->Initialize(m_pConnection);
	printf("setUp\n");
}

void StyleTest::tearDown()
{
	m_pConnection->Close();
	AUGE_SAFE_RELEASE(m_pConnection);
	
	auge::CartoManager* pCartoManager = auge::augeGetCartoManagerInstance();
	pCartoManager->Cleanup();

	auge::DataEngineManager* pEngineManager = NULL;
	pEngineManager = auge::augeGetDataEngineManagerInstance();
	pEngineManager->Unload();

	printf("tearDown\n");
}

void StyleTest::CreateSLD()
{
	g_uint id = 0;
	auge::Style* pStyle = NULL;	
	auge::CartoManager* pCartoManager = auge::augeGetCartoManagerInstance();
	
	//pStyle = LoadSLD("E:\\Research\\Auge.GIS\\Engine\\Carto\\sld\\point_user_4.xml");
	pStyle = LoadSLD("/home/renyc/code/auge.gis/Engine/Carto/sld/point_user_4.xml");
	id = pCartoManager->CreateStyle("point", pStyle,auge::augeGTPoint);
	printf("[Style ID]:%d\n", id);

	//pStyle = LoadSLD("E:\\Research\\Auge.GIS\\Engine\\Carto\\sld\\cities_label.xml");
	pStyle = LoadSLD("/home/renyc/code/auge.gis/Engine/Carto/sld/cities_label.xml");
	id = pCartoManager->CreateStyle("cities_label", pStyle, auge::augeGTPoint);
	printf("[Style ID]:%d\n", id);

	//pStyle = LoadSLD("E:\\Research\\Auge.GIS\\Engine\\Carto\\sld\\line.xml");
	pStyle = LoadSLD("/home/renyc/code/auge.gis/Engine/Carto/sld/line.xml");
	id = pCartoManager->CreateStyle("line", pStyle, auge::augeGTLineString);
	printf("[Style ID]:%d\n", id);

	//pStyle = LoadSLD("E:\\Research\\Auge.GIS\\Engine\\Carto\\sld\\polygon_2.xml");
	pStyle = LoadSLD("/home/renyc/code/auge.gis/Engine/Carto/sld/polygon_2.xml");
	id = pCartoManager->CreateStyle("polygon", pStyle, auge::augeGTPolygon);
	printf("[Style ID]:%d\n", id);

	//pStyle = LoadSLD("E:\\Research\\Auge.GIS\\Engine\\Carto\\sld\\polygon_red.xml");
	pStyle = LoadSLD("/home/renyc/code/auge.gis/Engine/Carto/sld/polygon_red.xml");
	id = pCartoManager->CreateStyle("polygon_red", pStyle, auge::augeGTPolygon);
	printf("[Style ID]:%d\n", id);

	pStyle->Release();
}

void StyleTest::UpdateSLD()
{
	auge::Style* pStyle = NULL;
	auge::CartoManager* pCartoManager = auge::augeGetCartoManagerInstance();
	g_uint gid;

	//pStyle = LoadSLD("E:\\Research\\Auge.GIS\\Engine\\Carto\\sld\\point_user_4.xml");
	pStyle = LoadSLD("/home/renyc/code/auge.gis/Engine/Carto/sld/point_user_4.xml");

	//pCartoManager->GetStyle();
	//id = pCartoManager->UpdateStyle("s1", pStyle);


	pStyle->Release();
}

void StyleTest::RemoveSLD()
{
	auge::CartoManager* pCartoManager = auge::augeGetCartoManagerInstance();

	g_uint id = pCartoManager->RemoveStyle("s1");
}

void StyleTest::ReadSLD()
{
	//const char* path = "E:\\Research\\Auge.GIS\\Engine\\Carto\\sld\\point_user_6.xml";
	//const char* path = "E:\\Research\\Auge.GIS\\Engine\\Carto\\sld\\polygon.xml";
	//const char* path = "E:\\Research\\Auge.GIS\\Engine\\Carto\\sld\\line.xml";
	//const char* path = "/home/renyc/code/auge.gis/Engine/Carto/sld/line.xml";
	//const char* path = "E:\\Research\\Auge.GIS\\Engine\\Carto\\sld\\point_theme_2.xml";
	//const char* path = "E:\\Research\\Auge.GIS\\Engine\\Carto\\sld\\point_star.xml";
	//const char* path = "E:\\Research\\Auge.GIS\\Engine\\Carto\\sld\\line-railway.xml";
	const char* path = "E:\\Research\\Auge.GIS\\Engine\\Carto\\sld\\polygon_grass.xml";

	auge::CartoFactory* pCartoFactory = NULL;
	pCartoFactory = auge::augeGetCartoFactoryInstance();
	
	auge::StyleFactory* pStyleFactory = NULL;
	pStyleFactory = auge::augeGetStyleFactoryInstance();

	auge::StyleReader* reader = NULL;
	reader = pStyleFactory->CreateStyleReader();

	auge::Style* pStyle = NULL;
	pStyle = reader->Read(path, NULL);
	CPPUNIT_ASSERT(pStyle!=NULL);

	auge::StyleWriter* writer = NULL;
	writer = pStyleFactory->CreateStyleWriter();

	auge::XDocument* pxDoc = NULL;
	pxDoc = writer->Write(pStyle);
	writer->Release();

	pxDoc->Save("g:\\temp\\map\\sld.xml", "GBK", 1);
	//pxDoc->Save("/home/renyc/map/sld.xml", "GBK", 1);
	pxDoc->Release();

	AUGE_SAFE_RELEASE(pStyle);
	AUGE_SAFE_RELEASE(reader);
}

auge::Style* LoadSLD(const char* path)
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
