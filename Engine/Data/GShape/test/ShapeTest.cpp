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
	const char* path = "DATABASE=d:\\geobeans\\data\\world";
	const char* className = "rivers";

	RESULTCODE rc = AG_FAILURE;
	auge::DataEngine* pDataEngine = NULL;
	pDataEngine = auge::augeGetDataEngineInstance();

	auge::FeatureWorksapce* pWorkspace = NULL;
	pWorkspace = (auge::FeatureWorksapce*)pDataEngine->CreateWorkspace();
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

	g_uchar* wkb = NULL;
	auge::Geometry	*pGeometry = NULL;
	auge::Feature	*pFeature = NULL;
	while((pFeature=pCursor->NextFeature())!=NULL)
	{	
		pGeometry = pFeature->GetGeometry();
		//wkb = pGeometry->AsBinary();

		//auge::WKBPoint* pWKBPoint = (auge::WKBPoint*)wkb;
		//printf("[%d]:%f,%f\n", pFeature->GetFID(),pWKBPoint->point.x, pWKBPoint->point.y);

		printf("[name]:%s\n", pFeature->GetString("name"));
		printf("[%d]:%s\n", pFeature->GetFID(),pGeometry->AsText());
		

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