#include "PgRasterTest.h"
#include "AugeCore.h"
#include "AugeData.h"
#include "AugeRaster.h"

CPPUNIT_TEST_SUITE_REGISTRATION(PgRasterTest);

void PgRasterTest::setUp() 
{
	//const char* path = "SERVER=127.0.0.1;INSTANCE=5432;DATABASE=GISDB;USER=postgres;PASSWORD=qwer1234;ENCODING=GBK";
	//const char* path = "SERVER=127.0.0.1;INSTANCE=5432;DATABASE=gisdb;USER=postgres;PASSWORD=qwer1234;ENCODING=GBK";
	//const char* path = "SERVER=127.0.0.1;INSTANCE=5432;DATABASE=gaode;USER=postgres;PASSWORD=qwer1234;ENCODING=GBK";
	const char* path = "SERVER=192.168.111.160;INSTANCE=5432;DATABASE=gisdb;USER=postgres;PASSWORD=qwer1234;ENCODING=GBK;repository=E:\\Research\\Auge.GIS\\Dist\\32_x86_win_vc10\\binD\\user\\user1\\rds";
	//const char* path = "SERVER=182.92.114.80;INSTANCE=5432;DATABASE=gisdb;USER=postgres;PASSWORD=qwer1234;ENCODING=GBK";

	auge::GLogger	*pLogger = auge::augeGetLoggerInstance();
	pLogger->Initialize();

	RESULTCODE rc = AG_FAILURE;
	auge::DataEngine* pDataEngine = NULL;
	pDataEngine = auge::augeGetDataEngineInstance();
	
	m_pWorkspace = dynamic_cast<auge::RasterWorkspace*>(pDataEngine->CreateWorkspace());
	m_pWorkspace->SetConnectionString(path);

	rc = m_pWorkspace->Open();
	CPPUNIT_ASSERT(rc==AG_SUCCESS);

	printf("setUp\n");
}

void PgRasterTest::tearDown()
{
	m_pWorkspace->Close();	
	AUGE_SAFE_RELEASE(m_pWorkspace);
	printf("tearDown\n");
}

void PgRasterTest::CreateRasterDataset()
{
	const char* name = "ds1";
	auge::RasterDataset* pDataset = NULL;
	pDataset = m_pWorkspace->CreateRasterDataset(name);
	CPPUNIT_ASSERT(pDataset!=NULL);
	pDataset->Release();
}

void PgRasterTest::RemoveRasterDataset()
{
	const char* name = "ds1";
	auge::RasterDataset* pDataset = NULL;
	RESULTCODE rc = m_pWorkspace->RemoverRasterDataset(name);
	CPPUNIT_ASSERT(rc!=AG_SUCCESS);
}

void PgRasterTest::RenameRasterDataset()
{

}

void PgRasterTest::EnumRasterDataset()
{
	auge::EnumDataSet* pDatasets = m_pWorkspace->GetRasterDatasets();
	CPPUNIT_ASSERT(pDatasets!=NULL);
}

void PgRasterTest::ReadRaster()
{
	auge::Raster* pRaster = NULL;
	auge::RasterDataset* pRasterDataset = NULL;
	
	//pRasterDataset = m_pWorkspace->OpenRasterDataset();
	//pFeatureClass = m_pWorkspace->OpenFeatureClass("高等院校");
	//CPPUNIT_ASSERT(pFeatureClass!=NULL);

	//auge::FeatureCursor* pCursor = NULL;
	//pCursor = pFeatureClass->Query();
	//CPPUNIT_ASSERT(pCursor!=NULL);

	//int counter = 1;
	//g_uchar* wkb = NULL;
	//auge::Geometry	*pGeometry = NULL;
	//auge::Feature	*pFeature = NULL;
	//while((pFeature=pCursor->NextFeature())!=NULL)
	//{	
	//	printf("\r%d", counter++);
	//	//float val = pFeature->GetFloat("co");
	//	pGeometry = pFeature->GetGeometry();
	//	//wkb = pGeometry->AsBinary();

	//	//printf("[Name]:%s\n", pFeature->GetString("name"));
	//	//printf("[%d]:%s\n", counter++, pFeature->GetString("name"));
	//	//auge::WKBPoint* pWKBPoint = (auge::WKBPoint*)wkb;
	//	//printf("[%d]:%f,%f\n", pFeature->GetFID(),pWKBPoint->point.x, pWKBPoint->point.y);

	//	pFeature->Release();
	//}
	//printf("\n");

	//AUGE_SAFE_RELEASE(pCursor);
	//AUGE_SAFE_RELEASE(pFeatureClass);

	
}
