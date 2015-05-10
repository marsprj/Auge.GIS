#include "RsTest.h"
#include "AugeCore.h"
#include "AugeData.h"
#include "AugeRaster.h"
#include "AugeGeometry.h"
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

void RsTest::OpenRaster()
{
	//const char* name = "tj.jpg";
	//const char* name = "9634_39954_16.png";
	//const char* name = "16km.png";
	//
	//auge::Raster* pRaster = m_pWorkspace->OpenRaster(name);
	//auge::GEnvelope& extent = pRaster->GetExtent();

	//std::cout<<"[Name]:"<<pRaster->GetName()<<std::endl;
	//std::cout<<"[Width]:"<<pRaster->GetWidth()<<std::endl;
	//std::cout<<"[Height]:"<<pRaster->GetHeight()<<std::endl;
	//std::cout<<"[Bands]:"<<pRaster->GetBandCount()<<std::endl;
	//std::cout<<"[Extent]:"<<extent.m_xmin<<","<<extent.m_ymin<<","<<extent.m_xmax<<","<<extent.m_ymax<<std::endl;
}

void RsTest::OpenRasterDataset()
{
	//const char* name = "tj.jpg";
	//const char* name = "9634_39954_16.png";
	//const char* name = "16km.png";

	//auge::Raster* pRaster = m_pWorkspace->OpenRaster(name);
	//auge::GEnvelope& extent = pRaster->GetExtent();

	//std::cout<<"[Name]:"<<pRaster->GetName()<<std::endl;
	//std::cout<<"[Width]:"<<pRaster->GetWidth()<<std::endl;
	//std::cout<<"[Height]:"<<pRaster->GetHeight()<<std::endl;
	//std::cout<<"[Bands]:"<<pRaster->GetBandCount()<<std::endl;
	//std::cout<<"[Extent]:"<<extent.m_xmin<<","<<extent.m_ymin<<","<<extent.m_xmax<<","<<extent.m_ymax<<std::endl;
}

/*
	g_uint		m_gid;
	g_uint		m_nband;
	g_int		m_srid;
	g_uint		m_width;
	g_uint		m_height;
	std::string	m_name;
	std::string	m_alias;
	std::string	m_format;
	std::string	m_path;
	std::string	m_uuid;
	GEnvelope	m_extent;
	*/
void RsTest::GetDatasets()
{
	auge::DataSet* pDataset = NULL;
	auge::EnumDataSet* pEnumSets = m_pWorkspace->GetDataSets();
	
	pEnumSets->Reset();
	while((pDataset=pEnumSets->Next())!=NULL)
	{
		auge::RasterDataset* pRasterDataset = static_cast<auge::RasterDataset*>(pDataset);
		std::cout<<"["<<pDataset->GetID()<<"]:"<<pRasterDataset->GetName()<<std::endl;
		std::cout<<"\t[Alias]:"<<pRasterDataset->GetAlias()<<std::endl;
		std::cout<<"\t[SRID]:"<<pRasterDataset->GetSRID()<<std::endl;
		std::cout<<"\t[Format]:"<<pRasterDataset->GetFormat()<<std::endl;
		std::cout<<"\t[Path]:"<<pRasterDataset->GetPath()<<std::endl;
		std::cout<<"\t[Bands]:"<<pRasterDataset->GetBandCount()<<std::endl;
		std::cout<<"\t[Width]:"<<pRasterDataset->GetWidth()<<std::endl;
		std::cout<<"\t[Height]:"<<pRasterDataset->GetHeight()<<std::endl;
		auge::GEnvelope& extent = pRasterDataset->GetExtent();
		std::cout<<"\t[Extent]:"<<std::endl;
		std::cout<<"\t\t[xmin]:"<<extent.m_xmin<<std::endl;
		std::cout<<"\t\t[ymin]:"<<extent.m_ymin<<std::endl;
		std::cout<<"\t\t[xmax]:"<<extent.m_xmax<<std::endl;
		std::cout<<"\t\t[ymax]:"<<extent.m_ymax<<std::endl;
		auge::Raster *pRaster = pRasterDataset->GetRaster();
	}

	pEnumSets->Release();
}

void RsTest::AddRasterTest()
{
	const char* rs_path = "G:\\temp\\td\\16km.tif";

	RESULTCODE rc = AG_SUCCESS;
	auge::Raster* pRaster = NULL;
	auge::RasterIO* pRasterIO = auge::augeGetRasterIOInstance();
	
	//pRaster = pRasterIO->Read(rs_path);
	//RESULTCODE rc = m_pWorkspace->AddRaster(pRaster);
	//CPPUNIT_ASSERT(rc==AG_SUCCESS);

	//pRaster = pRasterIO->Read("G:\\temp\\td\\TrueMarble.16km.2700x1350.tif");
	//rc = m_pWorkspace->AddRaster(pRaster);
	//CPPUNIT_ASSERT(rc==AG_SUCCESS);

	//pRaster = pRasterIO->Read("G:\\temp\\td\\16km.tif");
	//rc = m_pWorkspace->AddRaster(pRaster);
	//CPPUNIT_ASSERT(rc==AG_SUCCESS);

	pRaster = pRasterIO->Read("G:\\temp\\td\\16km.png");
	rc = m_pWorkspace->AddRaster(pRaster);
	CPPUNIT_ASSERT(rc==AG_SUCCESS);

	pRaster->Release();
}