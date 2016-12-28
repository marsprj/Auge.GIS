#include "ShapeTest.h"
#include "AugeFeature.h"
#include "AugeField.h"

CPPUNIT_TEST_SUITE_REGISTRATION(ShapeTest);

void ShowFields(auge::GFields* pFields);

void ShapeTest::setUp() 
{
	printf("setUp\n");
}

void ShapeTest::tearDown()
{
	printf("tearDown\n");
}

void ShapeTest::ReadTest()
{
	//const char* path = "DATABASE=g:\\temp\\map";
	//const char* path = "DATABASE=d:\\geobeans\\data\\world";
	//const char* path = "DATABASE=F:\\Resource\\Data\\��ѧ����\\��������\\shape\\ȫ��6�����ϵ�������_1973_2015_wgs84";
	const char* path = "DATABASE=F:\\Project\\��Ŀʵʩ\\863-ȫ��ƽ̨-2011\\����ʵʩ\\��Ŀ����\\1.3����\\shp";
	//const char* path = "DATABASE=/home/auge/world";
	//const char* className = "rivers";
	//const char* path = "DATABASE=C:\\Users\\renyc\\Desktop\\aaa";
	//const char* path = "DATABASE=C:\\Users\\renyc\\Desktop\\aaa";
	//const char* className = "cities";
	//const char* className = "country";
	//const char* className = "earthquake_wgs84_1973_2015";
	const char* className = "��ϰ";
	//const char* className = "poi_beijing";

	RESULTCODE rc = AG_FAILURE;
	auge::DataEngine* pDataEngine = NULL;
	pDataEngine = auge::augeGetDataEngineInstance();

	auge::FeatureWorkspace* pWorkspace = NULL;
	pWorkspace = dynamic_cast<auge::FeatureWorkspace*>(pDataEngine->CreateWorkspace());
	pWorkspace->SetConnectionString(path);

	rc = pWorkspace->Open();
	CPPUNIT_ASSERT(rc==AG_SUCCESS);

	auge::FeatureClass* pFeatureClass = NULL;
	pFeatureClass = pWorkspace->OpenFeatureClass(className);
	CPPUNIT_ASSERT(pFeatureClass!=NULL);

	ShowFields(pFeatureClass->GetFields());
	
	auge::FeatureCursor* pCursor = NULL;
	pCursor = pFeatureClass->Query();
	CPPUNIT_ASSERT(pCursor!=NULL);

	g_uint fid = 0;
	g_uchar* wkb = NULL;
	auge::Geometry	*pGeometry = NULL;
	auge::Feature	*pFeature = NULL;
	while((pFeature=pCursor->NextFeature())!=NULL)
	{	
		fid = pFeature->GetFID();
		pGeometry = pFeature->GetGeometry();
		//wkb = pGeometry->AsBinary();

		//auge::WKBPoint* pWKBPoint = (auge::WKBPoint*)wkb;
		//printf("[%d]:%f,%f\n", pFeature->GetFID(),pWKBPoint->point.x, pWKBPoint->point.y);

		//printf("[��]:%f\n", pFeature->GetDouble("��"));
		//printf("[name]:%s\n", pFeature->GetString("��"));
		printf("[%d]:%s\n", fid,pGeometry->AsText());
		
		pFeature->Release();
	}
	printf("\n");

	AUGE_SAFE_RELEASE(pCursor);
	AUGE_SAFE_RELEASE(pFeatureClass);

	pWorkspace->Close();	
	AUGE_SAFE_RELEASE(pWorkspace);
}


void ShowFields(auge::GFields* pFields)
{
	int count = pFields->Count();
	for(int i=0; i<count; i++)
	{
		auge::GField* pField = pFields->GetField(i);
		printf("~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~\n");
		printf("[Name]:%s\n",pField->GetName());
		printf("[Type]:%d\n", pField->GetType());
		printf("[Width]:%d\n", pField->GetLength());
	}
	
}
