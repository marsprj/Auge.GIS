#include "RasterTileTest.h"
#include "AugeCore.h"
#include "AugeData.h"
#include "AugeCarto.h"
#include "AugeProcessor.h"
#include <iostream>

//CPPUNIT_TEST_SUITE_REGISTRATION(RasterTileTest);

void RasterTileTest::setUp() 
{
	const char* path = "DATABASE=G:\\temp\\quad";

	RESULTCODE rc = AG_FAILURE;

	auge::GLogger	*pLogger = auge::augeGetLoggerInstance();
	pLogger->Initialize();

	auge::DataEngine	*pEngine = NULL;
	auge::DataEngineManager* pEngineManager = NULL;
	pEngineManager = auge::augeGetDataEngineManagerInstance();
	pEngineManager->Load();

	pEngine = pEngineManager->GetEngine("RasterFS");
	m_pConnection = pEngine->CreateConnection();
	m_pConnection->SetConnectionString("DATABASE=G:\\Data\\raster\\jpg");
	m_pConnection->Open();

	auge::ConnectionManager* pConnManager = NULL;
	pConnManager = auge::augeGetConnectionManagerInstance();
	pConnManager->Initialize(m_pConnection);

	auge::CartoManager* pCartoManager = NULL;
	pCartoManager = auge::augeGetCartoManagerInstance();
	pCartoManager->Initialize(m_pConnection);

	pEngine = pEngineManager->GetEngine("TileFS");	
	m_pWorkspace = dynamic_cast<auge::TileWorkspace*>(pEngine->CreateWorkspace());
	m_pWorkspace->SetConnectionString(path);

	rc = m_pWorkspace->Open();
	CPPUNIT_ASSERT(rc==AG_SUCCESS);

	printf("setUp\n");
}

void RasterTileTest::tearDown()
{
	m_pWorkspace->Close();	
	AUGE_SAFE_RELEASE(m_pWorkspace);

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

void RasterTileTest::BuildRasterPyramid()
{	
	auge::Map* pMap = NULL;
	auge::CartoManager* pCartoManager = auge::augeGetCartoManagerInstance();
	auge::CartoFactory* pCartoFactory = auge::augeGetCartoFactoryInstance();

	//pMap = pCartoManager->LoadMap("world");
	auge::Canvas* pCanvas = NULL;//pCartoFactory->CreateCanvas2D(256,256);
	
	char t_path[AUGE_PATH_MAX] = {0};
	//strcpy(t_path, "G:\\temp\\map\\map.png");
	//auge::TileStore *pTileStore = m_pWorkspace->GetTileStore(NULL);
	//m_pWorkspace->CreateTileStore(NULL,auge::augeTilePGIS,1,5,pMap->GetExtent());
	m_pWorkspace->CreateTileStore(NULL,auge::augeTileGoogleCRS84Quad,1,3,pMap->GetExtent());
	auge::TileStore *pTileStore = m_pWorkspace->OpenTileStore(NULL);
	auge::GEnvelope viewer = pTileStore->GetExtent();
	//pCanvas->SetViewer(viewer);
	//pCanvas->Draw(pMap);

	DWORD ts = GetTickCount();

	auge::MapTileStoreGenerator* pProcessor = NULL;
	auge::GProcessorFactory* pFactory = auge::augeGetGeoProcessorFactoryInstance();
	pProcessor = pFactory->CreateTileStoreGenerator();

	pProcessor->SetMap(pMap);
	pProcessor->SetTileStore(pTileStore);
	pProcessor->SetStartLevel(1);
	pProcessor->SetEndLevel(5);
	pProcessor->SetViewer(viewer);

	g_uint counter = pProcessor->Execute();

	pProcessor->Release();
	
	//auge::GColor bgColor(255,255,255,255);
	//g_uint levels = 8;
	//for(g_uint l=1; l<levels; l++)
	//{
	//	g_uint rows = m_pWorkspace->GetRows(l);
	//	g_uint cols = m_pWorkspace->GetCols(l);

	//	for(g_uint r=0; r<rows; r++)
	//	{
	//		for(g_uint c=0; c<cols; c++)
	//		{
	//			printf("\r[%d]:%d-%d-%d",counter++,l,r,c);
	//			
	//			m_pWorkspace->GetExtent(viewer, l, r,c);
	//			m_pWorkspace->GetTilePath(t_path, AUGE_PATH_MAX, l, r, c);

	//			pCanvas = pCartoFactory->CreateCanvas2D(256,256);
	//			pCanvas->SetViewer(viewer);
	//			//pCanvas->DrawBackground(bgColor);
	//			pCanvas->Draw(pMap);
	//			pCanvas->Save(t_path);
	//			pCanvas->Release();
	//		}
	//	}
	//}
	
	DWORD te = GetTickCount();
	printf("[时间]:%d毫秒\n", te-ts);
	printf("[时间/tile]:%8.3f毫秒\n", (float)(te-ts)/(float)counter);

	AUGE_SAFE_RELEASE(pMap);

}
