#include "CanvasTest.h"
#include "AugeGeometry.h"
#include "AugeCore.h"
#include "AugeCarto.h"
#include "AugeFeature.h"
#include "AugeData.h"

//#pragma comment(lib, "GPostgresD.lib")

//CPPUNIT_TEST_SUITE_REGISTRATION(CanvasTest);

void CanvasTest::setUp() 
{
	printf("setUp\n");
}

void CanvasTest::tearDown()
{
	printf("tearDown\n");
}

void CanvasTest::BackgroundTest()
{
	auge::Canvas* pCanvas = NULL;
	auge::CartoFactory* pCartoFactory = auge::augeGetCartoFactoryInstance();

	pCanvas = pCartoFactory->CreateCanvas2D(800, 600);

	auge::GColor bgColor(255,255,0,50);
	pCanvas->DrawBackground(bgColor);

	pCanvas->Save("g:\\temp\\map\\map_backgroud.png");
	
	AUGE_SAFE_RELEASE(pCanvas);
}

void CanvasTest::DrawPoint()
{
	const char* wkt = "POINT(0 0)";
	//const char* wkt = "LINESTRING(0 0,10 10,0 10)";

	auge::Canvas* pCanvas = NULL;
	auge::CartoFactory* pCartoFactory = auge::augeGetCartoFactoryInstance();

	auge::GEnvelope viewer(-180.f,-90.f,180.f,90.f);
	pCanvas = pCartoFactory->CreateCanvas2D(800, 600);
	pCanvas->SetViewer(viewer);

	auge::Geometry* pGeometry = NULL;
	auge::GeometryFactory* pFactory = NULL;
	pFactory = auge::augeGetGeometryFactoryInstance();

	pGeometry = pFactory->CreateGeometryFromWKT(wkt);
	auge::WKBPoint* pWKBPoint = (auge::WKBPoint*)pGeometry->AsBinary();

	auge::StyleFactory* pStyleFactory = auge::augeGetStyleFactoryInstance();
	auge::PointSymbolizer* pSymbolizer = NULL;
	pSymbolizer = pStyleFactory->CreatePointSymbolizer();
	pSymbolizer->SetSize(20);

	auge::GColor bgColor(255,255,255,255);
	pCanvas->DrawBackground(bgColor);
	
	auge::Renderer* pRenderer = pCanvas->GetRenderer();
	pRenderer->DrawPoint(pWKBPoint, pSymbolizer, pCanvas->GetTransform());

	pCanvas->Save("g:\\temp\\map\\point.png");

	AUGE_SAFE_RELEASE(pGeometry);
	AUGE_SAFE_RELEASE(pCanvas);
}

void CanvasTest::DrawLine()
{
	const char* wkt = "LINESTRING(-40 0,0 40,40 0,0 -40,-40 0)";

	auge::Canvas* pCanvas = NULL;
	auge::CartoFactory* pCartoFactory = auge::augeGetCartoFactoryInstance();

	auge::GEnvelope viewer(-180.f,-90.f,180.f,90.f);
	pCanvas = pCartoFactory->CreateCanvas2D(800, 600);
	pCanvas->SetViewer(viewer);

	auge::Geometry* pGeometry = NULL;
	auge::GeometryFactory* pFactory = NULL;
	pFactory = auge::augeGetGeometryFactoryInstance();

	pGeometry = pFactory->CreateGeometryFromWKT(wkt);
	auge::WKBLineString* pWKBLineString = (auge::WKBLineString*)pGeometry->AsBinary();

	auge::StyleFactory* pStyleFactory = auge::augeGetStyleFactoryInstance();
	auge::LineSymbolizer* pSymbolizer = NULL;
	pSymbolizer = pStyleFactory->CreateLineSymbolizer();

	auge::GColor bgColor(255,255,255,255);
	pCanvas->DrawBackground(bgColor);

	auge::Renderer* pRenderer = pCanvas->GetRenderer();
	pRenderer->DrawLine(pWKBLineString, pSymbolizer, pCanvas->GetTransform());

	pCanvas->Save("g:\\temp\\map\\line.png");

	AUGE_SAFE_RELEASE(pGeometry);
	AUGE_SAFE_RELEASE(pCanvas);
}


void CanvasTest::DrawPolygon()
{
	//const char* wkt = "POLYGON((-40 0,0 40,40 0,0 -40, -40 0))";
	const char* wkt = "POLYGON((-40 0,0 40,40 0,0 -40, -40 0),(-10 -10,10 -10,10 10,-10 10,-10 -10))";
	//const char* wkt = "POLYGON((-40 0,0 40,40 0,0 -40, -40 0),(-10 0,0 -10,10 0,0 10, -10 0))";

	auge::Canvas* pCanvas = NULL;
	auge::CartoFactory* pCartoFactory = auge::augeGetCartoFactoryInstance();

	auge::GEnvelope viewer(-180.f,-90.f,180.f,90.f);
	pCanvas = pCartoFactory->CreateCanvas2D(800, 600);
	pCanvas->SetViewer(viewer);

	auge::Geometry* pGeometry = NULL;
	auge::GeometryFactory* pFactory = NULL;
	pFactory = auge::augeGetGeometryFactoryInstance();

	pGeometry = pFactory->CreateGeometryFromWKT(wkt);
	auge::WKBPolygon* pWKBPolygon = (auge::WKBPolygon*)pGeometry->AsBinary();

	auge::StyleFactory* pStyleFactory = auge::augeGetStyleFactoryInstance();
	auge::PolygonSymbolizer* pSymbolizer = NULL;
	pSymbolizer = pStyleFactory->CreatePolygonSymbolizer();

	auge::GColor bgColor(255,255,255,255);
	pCanvas->DrawBackground(bgColor);

	auge::Renderer* pRenderer = pCanvas->GetRenderer();
	pRenderer->DrawPolygon(pWKBPolygon, pSymbolizer, pCanvas->GetTransform());

	pCanvas->Save("g:\\temp\\map\\polygon.png");

	AUGE_SAFE_RELEASE(pGeometry);
	AUGE_SAFE_RELEASE(pCanvas);
}

//void CanvasTest::DrawShpPoint()
//{
//	//const char* path = "DATABASE=d:\\geobeans\\data\\world";
//	const char* path = "SERVER=127.0.0.1;INSTANCE=5432;DATABASE=gisdb;USER=postgres;PASSWORD=qwer1234;ENCODING=GBK";
//
//	RESULTCODE rc = AG_FAILURE;
//	auge::DataEngine* pDataEngine = NULL;
//	pDataEngine = auge::augeGetDataEngineInstance();
//
//	auge::FeatureWorksapce* pWorkspace = NULL;
//	pWorkspace = pDataEngine->CreateFeatureWorkspace();
//	pWorkspace->SetConnectionString(path);
//
//	rc = pWorkspace->Open();
//	CPPUNIT_ASSERT(rc==AG_SUCCESS);
//
//	auge::FeatureClass* pFeatureClass = NULL;
//	pFeatureClass = pWorkspace->OpenFeatureClass("cities");
//	CPPUNIT_ASSERT(pFeatureClass!=NULL);
//
//	auge::FeatureCursor* pCursor = NULL;
//	pCursor = pFeatureClass->Query();
//	CPPUNIT_ASSERT(pCursor!=NULL);
//
//	auge::Canvas* pCanvas = NULL;
//	auge::CartoFactory* pCartoFactory = auge::augeGetCartoFactoryInstance();
//
//	auge::GEnvelope viewer(-180.f,-90.f,180.f,90.f);
//	pCanvas = pCartoFactory->CreateCanvas2D(800, 600);
//	pCanvas->SetViewer(viewer);
//	auge::Renderer* pRenderer = pCanvas->GetRenderer();
//
//	auge::StyleFactory* pStyleFactory = auge::augeGetStyleFactoryInstance();
//	auge::PointSymbolizer* pSymbolizer = NULL;
//	pSymbolizer = pStyleFactory->CreatePointSymbolizer();
//
//	auge::GColor bgColor(255,255,255,255);
//	pCanvas->DrawBackground(bgColor);
//
//	g_uchar* wkb = NULL;
//	auge::Geometry	*pGeometry = NULL;
//	auge::Feature	*pFeature = NULL;
//	while((pFeature=pCursor->NextFeature())!=NULL)
//	{	
//		pGeometry = pFeature->GetGeometry();
//		wkb = pGeometry->AsBinary();
//
//		auge::WKBPoint* pWKBPoint = (auge::WKBPoint*)wkb;
//		printf("[%d]:%f,%f\n", pFeature->GetFID(),pWKBPoint->point.x, pWKBPoint->point.y);
//
//		pRenderer->DrawPoint(pWKBPoint, pSymbolizer, pCanvas->GetTransform());
//
//		pFeature->Release();
//	}
//
//	pCanvas->Save("g:\\temp\\map\\shp_point.png");
//
//	AUGE_SAFE_RELEASE(pCanvas);
//
//	AUGE_SAFE_RELEASE(pCursor);
//	AUGE_SAFE_RELEASE(pFeatureClass);
//
//	pWorkspace->Close();	
//	AUGE_SAFE_RELEASE(pWorkspace);
//}
//
//void CanvasTest::DrawShpLine()
//{
//	//const char* path = "DATABASE=d:\\geobeans\\data\\world";
//	const char* path = "SERVER=127.0.0.1;INSTANCE=5432;DATABASE=gisdb;USER=postgres;PASSWORD=qwer1234;ENCODING=GBK";
//
//	RESULTCODE rc = AG_FAILURE;
//	auge::DataEngine* pDataEngine = NULL;
//	pDataEngine = auge::augeGetDataEngineInstance();
//
//	auge::FeatureWorksapce* pWorkspace = NULL;
//	pWorkspace = pDataEngine->CreateFeatureWorkspace();
//	pWorkspace->SetConnectionString(path);
//
//	rc = pWorkspace->Open();
//	CPPUNIT_ASSERT(rc==AG_SUCCESS);
//
//	auge::FeatureClass* pFeatureClass = NULL;
//	pFeatureClass = pWorkspace->OpenFeatureClass("rivers");
//	CPPUNIT_ASSERT(pFeatureClass!=NULL);
//
//	auge::FeatureCursor* pCursor = NULL;
//	pCursor = pFeatureClass->Query();
//	CPPUNIT_ASSERT(pCursor!=NULL);
//
//	auge::Canvas* pCanvas = NULL;
//	auge::CartoFactory* pCartoFactory = auge::augeGetCartoFactoryInstance();
//
//	auge::GEnvelope viewer(-180.f,-90.f,180.f,90.f);
//	pCanvas = pCartoFactory->CreateCanvas2D(800, 600);
//	pCanvas->SetViewer(viewer);
//	auge::Renderer* pRenderer = pCanvas->GetRenderer();
//
//	auge::StyleFactory* pStyleFactory = auge::augeGetStyleFactoryInstance();
//	auge::LineSymbolizer* pSymbolizer = NULL;
//	pSymbolizer = pStyleFactory->CreateLineSymbolizer();
//
//	auge::GColor bgColor(255,255,255,255);
//	pCanvas->DrawBackground(bgColor);
//
//	g_uchar* wkb = NULL;
//	auge::Geometry	*pGeometry = NULL;
//	auge::Feature	*pFeature = NULL;
//	while((pFeature=pCursor->NextFeature())!=NULL)
//	{	
//		pGeometry = pFeature->GetGeometry();
//		wkb = pGeometry->AsBinary();
//
//		pRenderer->Draw(wkb, pSymbolizer, pCanvas->GetTransform());
//
//		pFeature->Release();
//	}
//
//	pCanvas->Save("g:\\temp\\map\\shp_line.png");
//
//	AUGE_SAFE_RELEASE(pCanvas);
//
//	AUGE_SAFE_RELEASE(pCursor);
//	AUGE_SAFE_RELEASE(pFeatureClass);
//
//	pWorkspace->Close();	
//	AUGE_SAFE_RELEASE(pWorkspace);
//}
//
//void CanvasTest::DrawShpPolygon()
//{
//	//const char* path = "DATABASE=d:\\geobeans\\data\\world";
//	const char* path = "SERVER=127.0.0.1;INSTANCE=5432;DATABASE=gisdb;USER=postgres;PASSWORD=qwer1234;ENCODING=GBK";
//
//	RESULTCODE rc = AG_FAILURE;
//	auge::DataEngine* pDataEngine = NULL;
//	pDataEngine = auge::augeGetDataEngineInstance();
//
//	auge::FeatureWorksapce* pWorkspace = NULL;
//	pWorkspace = pDataEngine->CreateFeatureWorkspace();
//	pWorkspace->SetConnectionString(path);
//
//	rc = pWorkspace->Open();
//	CPPUNIT_ASSERT(rc==AG_SUCCESS);
//
//	auge::FeatureClass* pFeatureClass = NULL;
//	pFeatureClass = pWorkspace->OpenFeatureClass("country");
//	CPPUNIT_ASSERT(pFeatureClass!=NULL);
//
//	auge::FeatureCursor* pCursor = NULL;
//	pCursor = pFeatureClass->Query();
//	CPPUNIT_ASSERT(pCursor!=NULL);
//
//	auge::Canvas* pCanvas = NULL;
//	auge::CartoFactory* pCartoFactory = auge::augeGetCartoFactoryInstance();
//
//	auge::GEnvelope viewer(-180.f,-90.f,180.f,90.f);
//	pCanvas = pCartoFactory->CreateCanvas2D(800, 600);
//	pCanvas->SetViewer(viewer);
//	auge::Renderer* pRenderer = pCanvas->GetRenderer();
//
//	auge::StyleFactory* pStyleFactory = auge::augeGetStyleFactoryInstance();
//	auge::PolygonSymbolizer* pSymbolizer = NULL;
//	pSymbolizer = pStyleFactory->CreatePolygonSymbolizer();
//
//	auge::GColor bgColor(255,255,255,255);
//	pCanvas->DrawBackground(bgColor);
//
//	g_uchar* wkb = NULL;
//	auge::Geometry	*pGeometry = NULL;
//	auge::Feature	*pFeature = NULL;
//	while((pFeature=pCursor->NextFeature())!=NULL)
//	{	
//		pGeometry = pFeature->GetGeometry();
//		wkb = pGeometry->AsBinary();
//
//		pRenderer->Draw(wkb, pSymbolizer, pCanvas->GetTransform());
//
//		pFeature->Release();
//	}
//
//	pCanvas->Save("g:\\temp\\map\\shp_polygon.png");
//
//	AUGE_SAFE_RELEASE(pCanvas);
//
//	AUGE_SAFE_RELEASE(pCursor);
//	AUGE_SAFE_RELEASE(pFeatureClass);
//
//	pWorkspace->Close();	
//	AUGE_SAFE_RELEASE(pWorkspace);
//}

void CanvasTest::DrawShpPolygon()
{
	const char* libpath = "E:\\Research\\Auge.GIS\\Dist\\32_x86_win_vc10\\binD\\GPostgresD.dll";

	auge::DataEngine* pEngine = NULL;
	auge::DataEngineManager* pEngineManager = NULL;
	pEngineManager = auge::augeGetDataEngineManagerInstance();

	pEngine = pEngineManager->Register(libpath);
	CPPUNIT_ASSERT(pEngine!=NULL);

	pEngine = pEngineManager->GetEngine("postgres");

	//const char* path = "DATABASE=d:\\geobeans\\data\\world";
	const char* path = "SERVER=127.0.0.1;INSTANCE=5432;DATABASE=gisdb;USER=postgres;PASSWORD=qwer1234;ENCODING=GBK";

	RESULTCODE rc = AG_FAILURE;
	
	//auge::DataEngine* pEngine = NULL;
	//pEngine = auge::augeGetDataEngineInstance();

	auge::FeatureWorksapce* pWorkspace = NULL;
	pWorkspace = dynamic_cast<auge::FeatureWorksapce*>(pEngine->CreateWorkspace());
	pWorkspace->SetConnectionString(path);

	rc = pWorkspace->Open();
	CPPUNIT_ASSERT(rc==AG_SUCCESS);

	auge::FeatureClass* pFeatureClass = NULL;
	pFeatureClass = pWorkspace->OpenFeatureClass("country");
	CPPUNIT_ASSERT(pFeatureClass!=NULL);

	auge::FeatureCursor* pCursor = NULL;
	pCursor = pFeatureClass->Query();
	CPPUNIT_ASSERT(pCursor!=NULL);

	auge::Canvas* pCanvas = NULL;
	auge::CartoFactory* pCartoFactory = auge::augeGetCartoFactoryInstance();

	auge::GEnvelope viewer(-180.f,-90.f,180.f,90.f);
	pCanvas = pCartoFactory->CreateCanvas2D(800, 600);
	pCanvas->SetViewer(viewer);
	auge::Renderer* pRenderer = pCanvas->GetRenderer();

	auge::StyleFactory* pStyleFactory = auge::augeGetStyleFactoryInstance();
	auge::PolygonSymbolizer* pSymbolizer = NULL;
	pSymbolizer = pStyleFactory->CreatePolygonSymbolizer();

	auge::GColor bgColor(255,255,255,255);
	pCanvas->DrawBackground(bgColor);

	g_uchar* wkb = NULL;
	auge::Geometry	*pGeometry = NULL;
	auge::Feature	*pFeature = NULL;
	while((pFeature=pCursor->NextFeature())!=NULL)
	{	
		pGeometry = pFeature->GetGeometry();
		wkb = pGeometry->AsBinary();

		pRenderer->Draw(wkb, pSymbolizer, pCanvas->GetTransform());

		pFeature->Release();
	}

	pCanvas->Save("g:\\temp\\map\\shp_polygon.png");

	AUGE_SAFE_RELEASE(pCanvas);

	AUGE_SAFE_RELEASE(pCursor);
	AUGE_SAFE_RELEASE(pFeatureClass);

	pWorkspace->Close();	
	AUGE_SAFE_RELEASE(pWorkspace);

	pEngineManager->Unload();
}

void CanvasTest::DrawColorMap()
{
	auge::CartoFactory* pCartoFactory = auge::augeGetCartoFactoryInstance();
	auge::Renderer* pRenderer = pCartoFactory->CreateRenderer2D(100, 20);

	auge::StyleFactory* pStyleFactory = auge::augeGetStyleFactoryInstance();
	auge::GColor start(255,0,0,100);
	auge::GColor end(0,255,0,100);
	auge::ColorMap* pColorMap = pStyleFactory->CreateColorMap(10,start,end);

	//pColorMap->Release();

	pRenderer->DrawColorMap(pColorMap);
	pColorMap->Release();

	pRenderer->SaveAsImage("g:\\temp\\colormap.png");
	pRenderer->Release();
}

void CanvasTest::ReadColor()
{
	const char* str = "#FF0000FF";
	auge::GColor color;

	g_ulong val = atoi(str+1);
	color.Set(val);

	int r,g,b,a;
	sscanf(str,"#%2x%2x%2x%2x", &r,&g,&b,&a);

	color.Set(r,g,b,a);

	//g_byte r = color.GetRed();
	//g_byte g = color.GetGreen();
	//g_byte b = color.GetBlue();
	//g_byte a = color.GetAlpha();

}

void DrawRasterLayer()
{

}