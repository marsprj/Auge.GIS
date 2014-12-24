#include "ShapeTest.h"
#include "AugeFeature.h"

CPPUNIT_TEST_SUITE_REGISTRATION(ShapeTest);

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

	RESULTCODE rc = AG_FAILURE;
	auge::DataEngine* pDataEngine = NULL;
	pDataEngine = auge::augeGetDataEngineInstance();

	auge::FeatureWorksapce* pWorkspace = NULL;
	pWorkspace = pDataEngine->CreateFeatureWorkspace();
	pWorkspace->SetConnectionString(path);

	rc = pWorkspace->Open();
	CPPUNIT_ASSERT(rc==AG_SUCCESS);

	auge::FeatureClass* pFeatureClass = NULL;
	pFeatureClass = pWorkspace->OpenFeatureClass("cities");
	CPPUNIT_ASSERT(pFeatureClass!=NULL);

	auge::FeatureCursor* pCursor = NULL;
	pCursor = pFeatureClass->Query();
	CPPUNIT_ASSERT(pCursor!=NULL);

	g_uchar* wkb = NULL;
	auge::Geometry	*pGeometry = NULL;
	auge::Feature	*pFeature = NULL;
	while((pFeature=pCursor->NextFeature())!=NULL)
	{	
		pGeometry = pFeature->GetGeometry();
		wkb = pGeometry->AsBinary();

		auge::WKBPoint* pWKBPoint = (auge::WKBPoint*)wkb;
		printf("[%d]:%f,%f\n", pFeature->GetFID(),pWKBPoint->point.x, pWKBPoint->point.y);

		pFeature->Release();
	}
	printf("\n");

	AUGE_SAFE_RELEASE(pCursor);
	AUGE_SAFE_RELEASE(pFeatureClass);

	pWorkspace->Close();	
	AUGE_SAFE_RELEASE(pWorkspace);
}
