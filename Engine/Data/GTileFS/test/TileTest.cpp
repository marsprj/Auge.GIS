#include "TileTest.h"
#include "AugeCore.h"
#include "AugeData.h"
#include "AugeCarto.h"
#include <iostream>

CPPUNIT_TEST_SUITE_REGISTRATION(TileTest);

void TileTest::setUp() 
{
	const char* path = "DATABASE=G:\\temp\\quad";

	RESULTCODE rc = AG_FAILURE;

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

	pMap = pCartoManager->LoadMap("world");
	auge::Canvas* pCanvas = NULL;//pCartoFactory->CreateCanvas2D(256,256);
	
	char t_path[AUGE_PATH_MAX] = {0};
	//strcpy(t_path, "G:\\temp\\map\\map.png");
	auge::GEnvelope viewer = m_pWorkspace->GetExtent();
	//pCanvas->SetViewer(viewer);
	//pCanvas->Draw(pMap);

	DWORD ts = GetTickCount();
	g_uint counter = 0;
	
	auge::GColor bgColor(255,255,255,255);
	g_uint levels = 8;
	for(g_uint l=1; l<levels; l++)
	{
		g_uint rows = m_pWorkspace->GetRows(l);
		g_uint cols = m_pWorkspace->GetCols(l);

		for(g_uint r=0; r<rows; r++)
		{
			for(g_uint c=0; c<cols; c++)
			{
				printf("\r[%d]:%d-%d-%d",counter++,l,r,c);
				
				m_pWorkspace->GetExtent(viewer, l, r,c);
				m_pWorkspace->GetTilePath(t_path, AUGE_PATH_MAX, l, r, c);

				pCanvas = pCartoFactory->CreateCanvas2D(256,256);
				pCanvas->SetViewer(viewer);
				//pCanvas->DrawBackground(bgColor);
				pCanvas->Draw(pMap);
				pCanvas->Save(t_path);
				pCanvas->Release();
			}
		}
	}
	
	DWORD te = GetTickCount();
	printf("[ʱ��]:%d����\n", te-ts);
	printf("[ʱ��/tile]:%8.3f����\n", (float)(te-ts)/(float)counter);

	AUGE_SAFE_RELEASE(pMap);

}
