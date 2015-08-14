#include "FeatureTest.h"
#include "AugeCore.h"
#include "AugeField.h"
#include "AugeData.h"
#include "AugeCarto.h"
#include "AugeProcessor.h"
#include <iostream>

CPPUNIT_TEST_SUITE_REGISTRATION(FeatureTest);

void FeatureTest::setUp() 
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
	//m_pConnection->SetConnectionString("SERVER=127.0.0.1;INSTANCE=5432;DATABASE=auge;USER=postgres;PASSWORD=qwer1234;ENCODING=UTF-8");
	m_pConnection->SetConnectionString("SERVER=192.168.111.160;INSTANCE=5432;DATABASE=auge;USER=postgres;PASSWORD=qwer1234;ENCODING=GBK");
	m_pConnection->Open();

	auge::ConnectionManager* pConnManager = NULL;
	pConnManager = auge::augeGetConnectionManagerInstance();
	pConnManager->Initialize(m_pConnection);

	printf("setUp\n");
}

void FeatureTest::tearDown()
{
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

void FeatureTest::FeatureImportTest()
{	
	DWORD ts = GetTickCount();

	const char* shp_path = "D:\\GeoBeans\\data\\world";
	const char* shp_name = "country";
	const char* source_name = "db1";

	auge::GProcessorFactory* factory = auge::augeGetGeoProcessorFactoryInstance();
	auge::FeatureImportProcessor* processor = factory->CreateFeatureImportProcessor();

	processor->SetShapePath(shp_path);
	processor->SetShapeName(shp_name);
	processor->SetDataSourceName(source_name);
	processor->SetFeatureClassName("country");

	processor->Execute();

	//auge::TileStoreGenerator* pProcessor = NULL;
	//auge::GProcessorFactory* pFactory = auge::augeGetGeoProcessorFactoryInstance();
	//pProcessor = pFactory->CreateTileStoreGenerator();

	//pProcessor->SetMap(pMap);
	//pProcessor->SetTileStore(pTileStore);
	//pProcessor->SetStartLevel(1);
	//pProcessor->SetEndLevel(5);
	//pProcessor->SetViewer(viewer);

	//g_uint counter = pProcessor->Execute();

	//pProcessor->Release();
	//
	////auge::GColor bgColor(255,255,255,255);
	////g_uint levels = 8;
	////for(g_uint l=1; l<levels; l++)
	////{
	////	g_uint rows = m_pWorkspace->GetRows(l);
	////	g_uint cols = m_pWorkspace->GetCols(l);

	////	for(g_uint r=0; r<rows; r++)
	////	{
	////		for(g_uint c=0; c<cols; c++)
	////		{
	////			printf("\r[%d]:%d-%d-%d",counter++,l,r,c);
	////			
	////			m_pWorkspace->GetExtent(viewer, l, r,c);
	////			m_pWorkspace->GetTilePath(t_path, AUGE_PATH_MAX, l, r, c);

	////			pCanvas = pCartoFactory->CreateCanvas2D(256,256);
	////			pCanvas->SetViewer(viewer);
	////			//pCanvas->DrawBackground(bgColor);
	////			pCanvas->Draw(pMap);
	////			pCanvas->Save(t_path);
	////			pCanvas->Release();
	////		}
	////	}
	////}
	
	DWORD te = GetTickCount();
	printf("[时间]:%d毫秒\n", te-ts);

	AUGE_SAFE_RELEASE(processor);

}

void FeatureTest::FeatureExportTest()
{	
	DWORD ts = GetTickCount();

	const char* shp_path = "g:\\temp\\aqi";
	const char* shp_name = "aqi_ranking_2015_02_05_07";
	const char* source_name = "160_ourgis";

	auge::GProcessorFactory* factory = auge::augeGetGeoProcessorFactoryInstance();
	auge::FeatureExportProcessor* processor = factory->CreateFeatureExportProcessor();

	processor->SetUser(2);
	processor->SetShapePath(shp_path);
	processor->SetShapeName(shp_name);
	processor->SetDataSourceName(source_name);
	processor->SetFeatureClassName(shp_name);

	processor->Execute();


	DWORD te = GetTickCount();
	printf("[时间]:%d毫秒\n", te-ts);

	AUGE_SAFE_RELEASE(processor);

}

void FeatureTest::CsvImportTest()
{
	auge::ConnectionManager* pConnManager = NULL;
	pConnManager = auge::augeGetConnectionManagerInstance();
	auge::DataEngineManager* pEngineManager = NULL;
	pEngineManager = auge::augeGetDataEngineManagerInstance();

	const char* className = "people_age_60";
	auge::FeatureClass* pFeatureClass = NULL;
	auge::FeatureWorkspace* pFeatureWorkspace = dynamic_cast<auge::FeatureWorkspace*>(pConnManager->GetWorkspace("db1"));
	pFeatureClass = pFeatureWorkspace->OpenFeatureClass(className);
	if(pFeatureClass==NULL)
	{
		auge::GField* pField = NULL;
		auge::GField_2* pField_2 = NULL;
		auge::GFields* pFields = NULL;
		auge::FieldFactory* pFieldFactory = auge::augeGetFieldFactoryInstance();

		pFields = pFieldFactory->CreateFields();

		pField = pFieldFactory->CreateField();
		pField_2 = pField->Field_2();
		pField_2->SetName("地区");
		pField_2->SetType(augeFieldTypeString);
		pField_2->SetLength(32);
		pFields->Add(pField);

		pField = pFieldFactory->CreateField();
		pField_2 = pField->Field_2();
		pField_2->SetName("合计");
		pField_2->SetType(augeFieldTypeInt);
		pFields->Add(pField);

		pField = pFieldFactory->CreateField();
		pField_2 = pField->Field_2();
		pField_2->SetName("健康");
		pField_2->SetType(augeFieldTypeInt);
		pFields->Add(pField);

		pField = pFieldFactory->CreateField();
		pField_2 = pField->Field_2();
		pField_2->SetName("基本健康");
		pField_2->SetType(augeFieldTypeInt);
		pFields->Add(pField);

		pField = pFieldFactory->CreateField();
		pField_2 = pField->Field_2();
		pField_2->SetName("不健康生活能自理");
		pField_2->SetType(augeFieldTypeInt);
		pFields->Add(pField);

		pField = pFieldFactory->CreateField();
		pField_2 = pField->Field_2();
		pField_2->SetName("生活不能自理");
		pField_2->SetType(augeFieldTypeInt);
		pFields->Add(pField);

		pFeatureClass = pFeatureWorkspace->CreateFeatureClass(className,pFields);
	}
	pFeatureClass->Release();

	auge::CsvImportProcessor* pcsvImporter = NULL;
	auge::GProcessorFactory* pProcessorFactory = auge::augeGetGeoProcessorFactoryInstance();
	pcsvImporter = pProcessorFactory->CreateCsvImportProcessor();

	pcsvImporter->SetCvsPath("G:\\temp\\60人口.csv");
	pcsvImporter->SetDataSource("db1");
	pcsvImporter->SetDatasetName(className);

	pcsvImporter->Execute();

	pcsvImporter->Release();
}

void FeatureTest::ProjectTest()
{
	DWORD ts = GetTickCount();

	auge::FeatureProjectProcessor* pProcessor = NULL;
	auge::GProcessorFactory* pFactory = auge::augeGetGeoProcessorFactoryInstance();
	pProcessor = pFactory->CreateFeatureProjectProcessor();

	pProcessor->SetInputDataSource("test");
	pProcessor->SetInputFeatureClass("cities");

	pProcessor->SetOutputDataSource("test");
	pProcessor->SetOutputFeatureClass("cities_900913");
	pProcessor->SetOutputSRID(900913);
	
	RESULTCODE rc = pProcessor->Execute();

	pProcessor->Release();

	DWORD te = GetTickCount();
	printf("[时间]:%d毫秒\n", te-ts);
}
