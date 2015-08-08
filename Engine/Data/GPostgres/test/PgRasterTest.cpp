#include "PgRasterTest.h"
#include "AugeCore.h"
#include "AugeData.h"
#include "AugeRaster.h"

//CPPUNIT_TEST_SUITE_REGISTRATION(PgRasterTest);

void PgRasterTest::setUp() 
{
	//const char* path = "SERVER=127.0.0.1;INSTANCE=5432;DATABASE=GISDB;USER=postgres;PASSWORD=qwer1234;ENCODING=GBK";
	//const char* path = "SERVER=127.0.0.1;INSTANCE=5432;DATABASE=gisdb;USER=postgres;PASSWORD=qwer1234;ENCODING=GBK";
	//const char* path = "SERVER=127.0.0.1;INSTANCE=5432;DATABASE=gaode;USER=postgres;PASSWORD=qwer1234;ENCODING=GBK";
	//const char* path = "SERVER=192.168.111.160;INSTANCE=5432;DATABASE=gisdb;USER=postgres;PASSWORD=qwer1234;ENCODING=GBK;repository=E:\\Research\\Auge.GIS\\Dist\\32_x86_win_vc10\\binD\\user\\user1\\rds";
	const char* path = "SERVER=127.0.0.1;INSTANCE=5432;DATABASE=rsdb2;USER=postgres;PASSWORD=qwer1234;ENCODING=GBK;repository=E:\\Research\\Auge.GIS\\Dist\\32_x86_win_vc10\\binD\\user\\user1\\rds";
	//const char* path = "SERVER=192.168.111.160;INSTANCE=5432;DATABASE=rsdb;USER=postgres;PASSWORD=qwer1234;ENCODING=GBK;repository=E:\\Research\\Auge.GIS\\Dist\\32_x86_win_vc10\\binD\\user\\user1\\rds";
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

void PgRasterTest::CreateRasterFolder()
{
	auge::RasterFolder* pRoot = m_pWorkspace->GetRootFolder();
	CPPUNIT_ASSERT(pRoot!=NULL);

	const char* name = "ds1";
	auge::RasterFolder* pFolder = pRoot->CreateFolder(name);
	//auge::RasterFolder* pFolder = pRoot->GetFolder(name);
	CPPUNIT_ASSERT(pFolder!=NULL);	
	pFolder->Release();

	name = "ds2";
	pFolder = pRoot->CreateFolder(name);
	CPPUNIT_ASSERT(pFolder!=NULL);	

	auge::RasterFolder* pFolder_2 = pFolder->CreateFolder("ds_1_1");
	CPPUNIT_ASSERT(pFolder_2!=NULL);	
	pFolder_2->Release();

	pFolder_2 = pFolder->CreateFolder("ds_1_2");
	CPPUNIT_ASSERT(pFolder_2!=NULL);	
	pFolder_2->Release();


	pFolder->Release();
	pRoot->Release();
}

void PgRasterTest::RemoveRasterFolder()
{
	const char* name = "ds1";
	auge::RasterDataset* pDataset = NULL;
	//RESULTCODE rc = m_pWorkspace->RemoverRasterDataset(name);
	//CPPUNIT_ASSERT(rc!=AG_SUCCESS);
}

void PgRasterTest::RenameRasterFolder()
{

}

void PgRasterTest::EnumRasterFolder()
{
	//auge::EnumDataSet* pDatasets = m_pWorkspace->GetRasterDatasets();
	//CPPUNIT_ASSERT(pDatasets!=NULL);
	auge::RasterFolder* pRoot = m_pWorkspace->GetRootFolder();
	auge::EnumRasterFolder* pFolders = pRoot->GetFolders();

	auge::RasterFolder* pFolder = NULL;
	pFolders->Reset();
	while((pFolder=pFolders->Next()))
	{
		printf("%s\n", pFolder->GetName());
	}

	pFolders->Release();
	pRoot->Release();
}

void PgRasterTest::AddRaster()
{
	RESULTCODE rc = AG_FAILURE;

	auge::Raster* pRaster = NULL;
	auge::RasterIO* io = auge::augeGetRasterIOInstance();	

	auge::RasterFolder* pRoot = m_pWorkspace->GetRootFolder();
	CPPUNIT_ASSERT(pRoot!=NULL);

	const char* name = "ds1";
	//auge::RasterFolder* pFolder = pRoot->CreateFolder(name);
	auge::RasterFolder* pFolder = pRoot->GetFolder(name);
	CPPUNIT_ASSERT(pFolder!=NULL);	

	auge::RasterDataset* pRasterDataset = pFolder->GetRasterDataset();

	pRaster = io->Read("E:\\Research\\Auge.GIS\\Dist\\32_x86_win_vc10\\binD\\upload\\Desert.jpg");
	CPPUNIT_ASSERT(pRaster!=NULL);
	rc = pRasterDataset->AddRaster("Desert.jpg", pRaster);
	CPPUNIT_ASSERT(rc==AG_SUCCESS);
	pRaster->Release();

	pRaster = io->Read("E:\\Research\\Auge.GIS\\Dist\\32_x86_win_vc10\\binD\\upload\\Koala-2.png");
	CPPUNIT_ASSERT(pRaster!=NULL);
	rc = pRasterDataset->AddRaster("Koala.png", pRaster);
	CPPUNIT_ASSERT(rc==AG_SUCCESS);

	pRaster = io->Read("G:\\Data\\srtm_58_05\\srtm_58_05.tif");
	CPPUNIT_ASSERT(pRaster!=NULL);
	rc = pRoot->GetRasterDataset()->AddRaster("srtm_58_05.tif", pRaster);
	CPPUNIT_ASSERT(rc==AG_SUCCESS);

	pFolder->Release();
	pRoot->Release();
}

void PgRasterTest::ReadRaster()
{
	auge::Raster* pRaster = NULL;
	auge::RasterDataset* pRasterDataset = NULL;
	
	//pRasterDataset = m_pWorkspace->OpenRasterDataset();
	//pFeatureClass = m_pWorkspace->OpenFeatureClass("�ߵ�ԺУ");
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

void PgRasterTest::GetRasterByPath()
{
	const char* path = "/s1/Koala.png";
	auge::Raster* pRaster = m_pWorkspace->GetRaster(path);
	CPPUNIT_ASSERT(pRaster!=NULL);

	pRaster->Release();
}