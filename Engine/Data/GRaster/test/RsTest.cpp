#include "RsTest.h"
#include "AugeCore.h"
#include "AugeRaster.h"
#include <iostream>

CPPUNIT_TEST_SUITE_REGISTRATION(RsTest);

void RsTest::setUp() 
{
	//const char* path = "DATABASE=G:\\Data\\GF-1\\tianjin";
	const char* path = "DATABASE=E:\\Project\\Project\\GA\\Data\\nodata";

	auge::GLogger	*pLogger = auge::augeGetLoggerInstance();
	pLogger->Initialize();

	RESULTCODE rc = AG_FAILURE;
	auge::DataEngine* pDataEngine = NULL;
	pDataEngine = auge::augeGetDataEngineInstance();
	
	m_pWorkspace = (auge::RasterWorkspace*)pDataEngine->CreateWorkspace();
	m_pWorkspace->SetConnectionString(path);

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

void RsTest::ReadTest()
{
	
}

void RsTest::GetDataSet()
{
	//const char* name = "tj.jpg";
	const char* name = "9634_39954.png";
	
	auge::Raster* pRaster = m_pWorkspace->OpenRaster(name);
	auge::GEnvelope& extent = pRaster->GetExtent();

	std::cout<<"[Name]:"<<pRaster->GetName()<<std::endl;
	std::cout<<"[Width]:"<<pRaster->GetWidth()<<std::endl;
	std::cout<<"[Height]:"<<pRaster->GetHeight()<<std::endl;
	std::cout<<"[Bands]:"<<pRaster->GetBandCount()<<std::endl;
	std::cout<<"[Extent]:"<<extent.m_xmin<<","<<extent.m_ymin<<","<<extent.m_xmax<<","<<extent.m_ymax<<std::endl;
	

}
