#include "SymbolTest.h"
#include "AugeGeometry.h"
#include "AugeCore.h"
#include "AugeCarto.h"
#include "AugeFeature.h"
#include "AugeData.h"

CPPUNIT_TEST_SUITE_REGISTRATION(SymbolTest);

void SymbolTest::setUp() 
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
	pMarker = pSymbolManager->CreateMarkerSymbol(auge::augeMarkerStar);

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

	AUGE_SAFE_RELEASE(pMarker);
	AUGE_SAFE_RELEASE(pGeometry);
	AUGE_SAFE_RELEASE(pCanvas);
}

void SymbolTest::DrawStars()
{
	auge::Canvas* pCanvas = NULL;
	auge::CartoFactory* pCartoFactory = auge::augeGetCartoFactoryInstance();

	//auge::Geometry* pGeometry = NULL;
	//auge::GeometryFactory* pGeometryFactory = auge::augeGetGeometryFactoryInstance();
	//pGeometry = pGeometryFactory->CreateGeometryFromWKT("POINT(116 39)");

	auge::MarkerSymbol* pMarker = NULL;
	auge::SymbolManager* pSymbolManager = auge::augeGetSymbolManagerInstance();
	pMarker = pSymbolManager->CreateMarkerSymbol(auge::augeMarkerStar);

	auge::GEnvelope viewer(-180.f,-90.f,180.f,90.f);
	pCanvas = pCartoFactory->CreateCanvas2D(800, 600);
	pCanvas->SetViewer(viewer);

	auge::GColor bgColor(255,0,0,0);
	pCanvas->DrawBackground(bgColor);

	RESULTCODE rc = AG_FAILURE;
	auge::FeatureWorksapce*	 pWorkspace = NULL;
	auge::ConnectionManager* pConnectionManager = auge::augeGetConnectionManagerInstance();
	
	pWorkspace = dynamic_cast<auge::FeatureWorksapce*>(pConnectionManager->GetWorkspace("db1"));
	
	auge::FeatureClass* pFeatureClass = NULL;
	pFeatureClass = pWorkspace->OpenFeatureClass("cities");
	CPPUNIT_ASSERT(pFeatureClass!=NULL);

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
	

	pCanvas->Save("g:\\temp\\map\\star.png");
	//pCanvas->Save("/home/renyc/map/map.png");

	AUGE_SAFE_RELEASE(pMarker);
	AUGE_SAFE_RELEASE(pCursor);
	AUGE_SAFE_RELEASE(pFeatureClass);
	AUGE_SAFE_RELEASE(pCanvas);
}

