#include "RsTest.h"
#include "AugeCore.h"
#include "AugeData.h"
#include "AugeRaster.h"
#include <iostream>


CPPUNIT_TEST_SUITE_REGISTRATION(RsTest);

void RsTest::setUp() 
{
	const char* connstr = "SERVER=127.0.0.1;INSTANCE=5432;DATABASE=rsdb;USER=postgres;PASSWORD=qwer1234";

	auge::GLogger	*pLogger = auge::augeGetLoggerInstance();
	pLogger->Initialize();

	RESULTCODE rc = AG_FAILURE;
	auge::DataEngine* pgsDataEngine = NULL;
	auge::DataEngineManager* pDataEngineManager = auge::augeGetDataEngineManagerInstance();
	pDataEngineManager->Load();

	pgsDataEngine = pDataEngineManager->GetEngine("Postgres");
	auge::GConnection* pConnection = pgsDataEngine->CreateConnection();	
	pConnection->SetConnectionString(connstr);

	auge::DataEngine* prasterDataEngine = NULL;
	prasterDataEngine = pDataEngineManager->GetEngine("RasterDB");
	m_pWorkspace = static_cast<auge::RasterWorkspace*>(prasterDataEngine->CreateWorkspace());
	m_pWorkspace->SetConnection(pConnection);
	rc = m_pWorkspace->Open();
	CPPUNIT_ASSERT(rc==AG_SUCCESS);

	printf("setUp\n");
}

void RsTest::tearDown()
{
	m_pWorkspace->Close();	
	AUGE_SAFE_RELEASE(m_pWorkspace);
	printf("tearDown\n");
}

void RsTest::GetDataSet()
{
	//const char* name = "tj.jpg";
	//const char* name = "9634_39954_16.png";
	const char* name = "16km.png";
	
	auge::Raster* pRaster = m_pWorkspace->OpenRaster(name);
	auge::GEnvelope& extent = pRaster->GetExtent();

	std::cout<<"[Name]:"<<pRaster->GetName()<<std::endl;
	std::cout<<"[Width]:"<<pRaster->GetWidth()<<std::endl;
	std::cout<<"[Height]:"<<pRaster->GetHeight()<<std::endl;
	std::cout<<"[Bands]:"<<pRaster->GetBandCount()<<std::endl;
	std::cout<<"[Extent]:"<<extent.m_xmin<<","<<extent.m_ymin<<","<<extent.m_xmax<<","<<extent.m_ymax<<std::endl;
}

void RsTest::AddRasterTest()
{
	const char* rs_path = "G:\\temp\\td\\16km.tif";

	auge::Raster* pRaster = NULL;
	auge::RasterIO* pRasterIO = auge::augeGetRasterIOInstance();
	pRaster = pRasterIO->Read(rs_path);

	RESULTCODE rc = m_pWorkspace->AddRaster(pRaster);
	CPPUNIT_ASSERT(rc==AG_SUCCESS);
	
	pRaster->Release();
}