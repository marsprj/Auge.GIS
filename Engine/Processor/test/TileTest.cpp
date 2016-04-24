#include "TileTest.h"
#include "AugeCore.h"
#include "AugeField.h"
#include "AugeData.h"
#include "AugeCarto.h"
#include "AugeProcessor.h"
#include <iostream>

CPPUNIT_TEST_SUITE_REGISTRATION(TileTest);

void TileTest::setUp() 
{
	RESULTCODE rc = AG_FAILURE;

	auge::GLogger	*pLogger = auge::augeGetLoggerInstance();
	pLogger->Initialize();

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

	auge::SymbolManager* pSymbolManager = auge::augeGetSymbolManagerInstance();
	pSymbolManager->Initialize(m_pConnection);

	//const char* path = "DATABASE=G:\\temp\\quad";	
	//pEngine = pEngineManager->GetEngine("TileFS");
	
	const char* path = "SERVER=192.168.111.160;INSTANCE=27017;DATABASE=tfss;USER=user;PASSWORD=qwer1234";
	pEngine = pEngineManager->GetEngine("TileMGO");
	m_pWorkspace = dynamic_cast<auge::TileWorkspace*>(pEngine->CreateWorkspace());
	m_pWorkspace->SetConnectionString(path);

	//pEngine = pEngineManager->GetEngine("TileMgo");	
	//m_pWorkspace = dynamic_cast<auge::TileWorkspace*>(pEngine->CreateWorkspace());
	//const char* constr = "SERVER=127.0.0.1;INSTANCE=27017;DATABASE=tfs;USER=user;PASSWORD=qwer1234";
	//m_pWorkspace->SetConnectionString(constr);

	rc = m_pWorkspace->Open();
	CPPUNIT_ASSERT(rc==AG_SUCCESS);

	printf("setUp\n");
}

void TileTest::tearDown()
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

void TileTest::WriteTest()
{	
	auge::Map* pMap = NULL;
	auge::CartoManager* pCartoManager = auge::augeGetCartoManagerInstance();
	auge::CartoFactory* pCartoFactory = auge::augeGetCartoFactoryInstance();

	pMap = pCartoManager->LoadMap(2,"country");
	auge::Canvas* pCanvas = NULL;//pCartoFactory->CreateCanvas2D(256,256);
	
	char t_path[AUGE_PATH_MAX] = {0};
	//strcpy(t_path, "G:\\temp\\map\\map.png");
	//auge::TileStore *pTileStore = m_pWorkspace->GetTileStore(NULL);
	//m_pWorkspace->CreateTileStore(NULL,auge::augeTilePGIS,1,5,pMap->GetExtent());

	const char* tn = "t4";
	m_pWorkspace->CreateTileStore(tn,auge::augeTileGoogleCRS84Quad,1,8,pMap->GetExtent());
	auge::TileStore *pTileStore = m_pWorkspace->OpenTileStore(tn);
	auge::GEnvelope viewer = pTileStore->GetExtent();
	//pCanvas->SetViewer(viewer);
	//pCanvas->Draw(pMap);

	DWORD ts = GetTickCount();

	auge::MapTileStoreGenerator* pProcessor = NULL;
	auge::GProcessorFactory* pFactory = auge::augeGetGeoProcessorFactoryInstance();
	//pProcessor = pFactory->CreateTileStoreGenerator();
	pProcessor = pFactory->CreateMongoTileStoreGenerator();

	pProcessor->SetMap(pMap);
	pProcessor->SetTileStore(pTileStore);
	pProcessor->SetStartLevel(1);
	pProcessor->SetEndLevel(8);
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



void TileTest::BuildTileStorePGIS_FS()
{	
	auge::DataEngineManager* pEngineManager = auge::augeGetDataEngineManagerInstance();
	auge::DataEngine* pEngine = pEngineManager->GetEngine("TileFS");	
	auge::TileWorkspace* pWorkspace = dynamic_cast<auge::TileWorkspace*>(pEngine->CreateWorkspace());
	const char* constr = "DATABASE=G:\\temp\\quad";
	pWorkspace->SetConnectionString(constr);

	auge::Map* pMap = NULL;
	auge::CartoManager* pCartoManager = auge::augeGetCartoManagerInstance();
	auge::CartoFactory* pCartoFactory = auge::augeGetCartoFactoryInstance();

	pMap = pCartoManager->LoadMap(2, "world");
	auge::Canvas* pCanvas = NULL;//pCartoFactory->CreateCanvas2D(256,256);

	char t_path[AUGE_PATH_MAX] = {0};
	m_pWorkspace->CreateTileStore(NULL,auge::augeTilePGIS,1,5,pMap->GetExtent());
	auge::TileStore *pTileStore = pWorkspace->OpenTileStore("store1");
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

	DWORD te = GetTickCount();
	printf("[时间]:%d毫秒\n", te-ts);
	printf("[时间/tile]:%8.3f毫秒\n", (float)(te-ts)/(float)counter);
	 
	AUGE_SAFE_RELEASE(pMap);
	AUGE_SAFE_RELEASE(pWorkspace);
}


void TileTest::BuildTileStorePGIS_Mongo()
{	
	auge::DataEngineManager* pEngineManager = auge::augeGetDataEngineManagerInstance();
	auge::DataEngine* pEngine = pEngineManager->GetEngine("TileMGO");	
	auge::TileWorkspace* pWorkspace = dynamic_cast<auge::TileWorkspace*>(pEngine->CreateWorkspace());
	const char* constr = "SERVER=127.0.0.1;INSTANCE=27017;DATABASE=tfs;USER=user;PASSWORD=qwer1234";
	//const char* constr = "SERVER=192.168.111.160;INSTANCE=27017;DATABASE=tfs;USER=user;PASSWORD=qwer1234";
	pWorkspace->SetConnectionString(constr);
	pWorkspace->Open();

	//auge::EnumTileStore* pTileStores = NULL;
	//pTileStores = pWorkspace->GetTileStores();
	//pTileStores->Release();
	
	auge::Map* pMap = NULL;
	auge::CartoManager* pCartoManager = auge::augeGetCartoManagerInstance();
	auge::CartoFactory* pCartoFactory = auge::augeGetCartoFactoryInstance();

	pMap = pCartoManager->LoadMap(2, "world");
	auge::Canvas* pCanvas = NULL;//pCartoFactory->CreateCanvas2D(256,256);

	char t_path[AUGE_PATH_MAX] = {0};
	pWorkspace->CreateTileStore("world",auge::augeTileGoogleCRS84Quad,1,9,pMap->GetExtent());
	auge::TileStore *pTileStore = pWorkspace->OpenTileStore("world");
	auge::GEnvelope viewer = pTileStore->GetExtent();
	//pCanvas->SetViewer(viewer);
	//pCanvas->Draw(pMap);

	DWORD ts = GetTickCount();

	auge::MapTileStoreGenerator* pProcessor = NULL;
	auge::GProcessorFactory* pFactory = auge::augeGetGeoProcessorFactoryInstance();
	pProcessor = pFactory->CreateMongoTileStoreGenerator();

	pProcessor->SetMap(pMap);
	pProcessor->SetTileStore(pTileStore);
	pProcessor->SetStartLevel(0);
	pProcessor->SetEndLevel(9);
	pProcessor->SetViewer(viewer);

	g_uint counter = pProcessor->Execute();

	pProcessor->Release();

	DWORD te = GetTickCount();
	printf("[时间]:%d毫秒\n", te-ts);
	printf("[时间/tile]:%8.3f毫秒\n", (float)(te-ts)/(float)counter);

	AUGE_SAFE_RELEASE(pMap);
	AUGE_SAFE_RELEASE(pWorkspace);
}

