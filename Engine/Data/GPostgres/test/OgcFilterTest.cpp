#include "OgcFilterTest.h"
#include "AugeFilter.h"
#include "AugeCore.h"
#include "AugeXML.h"

CPPUNIT_TEST_SUITE_REGISTRATION(OgcFilterTest);

void OgcFilterTest::setUp() 
{
	//const char* path = "SERVER=127.0.0.1;INSTANCE=5432;DATABASE=GISDB;USER=postgres;PASSWORD=qwer1234;ENCODING=GBK";
	const char* path = "SERVER=127.0.0.1;INSTANCE=5432;DATABASE=gisdb;USER=postgres;PASSWORD=qwer1234;ENCODING=GBK";

	RESULTCODE rc = AG_FAILURE;
	auge::DataEngine* pDataEngine = NULL;
	pDataEngine = auge::augeGetDataEngineInstance();

	m_pWorkspace = dynamic_cast<auge::FeatureWorksapce*>(pDataEngine->CreateWorkspace());
	m_pWorkspace->SetConnectionString(path);

	rc = m_pWorkspace->Open();
	CPPUNIT_ASSERT(rc==AG_SUCCESS);

	printf("setUp\n");
}

void OgcFilterTest::tearDown()
{
	m_pWorkspace->Close();	
	AUGE_SAFE_RELEASE(m_pWorkspace);
	printf("tearDown\n");
}

void OgcFilterTest::ReadArithmeticComparison()
{
	const char* path = "E:\\Research\\Auge.GIS\\Engine\\Filter\\ogc\\ArithmeticComparison.xml";
	auge::FilterFactory* pFactory = auge::augeGetFilterFactoryInstance();
	auge::FilterReader* pFilterReader = pFactory->CreateFilerReader(NULL);
	auge::GFilter* pFilter = NULL;

	auge::XParser parser;
	auge::XDocument	*pxDoc = NULL;
	auge::XElement	*pxFilter = NULL;

	pxDoc = parser.Parse(path);
	pxFilter = pxDoc->GetRootNode();

	pFilter = pFilterReader->Read(pxFilter);
	CPPUNIT_ASSERT(pFilter!=NULL);

	/**************************************************************/
	auge::FeatureClass* pFeatureClass = NULL;
	pFeatureClass = m_pWorkspace->OpenFeatureClass("cities");
	CPPUNIT_ASSERT(pFeatureClass!=NULL);

	auge::FeatureCursor* pCursor = NULL;
	pCursor = pFeatureClass->Query(pFilter);
	CPPUNIT_ASSERT(pCursor!=NULL);

	g_uint i=0;
	auge::Geometry	*pGeometry = NULL;
	auge::Feature	*pFeature = NULL;
	while((pFeature=pCursor->NextFeature())!=NULL)
	{	
		printf("\r%d",++i);
		pFeature->Release();
	}
	printf("\n");

	AUGE_SAFE_RELEASE(pCursor);
	AUGE_SAFE_RELEASE(pFeatureClass);

	/**************************************************************/

	AUGE_SAFE_RELEASE(pFilter);
	pxDoc->Close();
	AUGE_SAFE_RELEASE(pxDoc);
}

void OgcFilterTest::ReadBBox()
{
	const char* path = "E:\\Research\\Auge.GIS\\Engine\\Filter\\ogc\\BBox.xml";
	auge::FilterFactory* pFactory = auge::augeGetFilterFactoryInstance();
	auge::FilterReader* pFilterReader = pFactory->CreateFilerReader(NULL);
	auge::GFilter* pFilter = NULL;

	auge::XParser parser;
	auge::XDocument	*pxDoc = NULL;
	auge::XElement	*pxFilter = NULL;

	pxDoc = parser.Parse(path);
	pxFilter = pxDoc->GetRootNode();

	pFilter = pFilterReader->Read(pxFilter);
	CPPUNIT_ASSERT(pFilter!=NULL);

	/**************************************************************/
	auge::FeatureClass* pFeatureClass = NULL;
	pFeatureClass = m_pWorkspace->OpenFeatureClass("cities");
	CPPUNIT_ASSERT(pFeatureClass!=NULL);

	auge::FeatureCursor* pCursor = NULL;
	pCursor = pFeatureClass->Query(pFilter);
	CPPUNIT_ASSERT(pCursor!=NULL);

	g_uint i=0;
	auge::Geometry	*pGeometry = NULL;
	auge::Feature	*pFeature = NULL;
	while((pFeature=pCursor->NextFeature())!=NULL)
	{	
		printf("\r%d",++i);
		pFeature->Release();
	}
	printf("\n");

	AUGE_SAFE_RELEASE(pCursor);
	AUGE_SAFE_RELEASE(pFeatureClass);

	/**************************************************************/

	AUGE_SAFE_RELEASE(pFilter);
	pxDoc->Close();
	AUGE_SAFE_RELEASE(pxDoc);
}

void OgcFilterTest::ReadBetween()
{
	const char* path = "E:\\Research\\Auge.GIS\\Engine\\Filter\\ogc\\Between.xml";
	auge::FilterFactory* pFactory = auge::augeGetFilterFactoryInstance();
	auge::FilterReader* pFilterReader = pFactory->CreateFilerReader(NULL);
	auge::GFilter* pFilter = NULL;

	auge::XParser parser;
	auge::XDocument	*pxDoc = NULL;
	auge::XElement	*pxFilter = NULL;

	pxDoc = parser.Parse(path);
	pxFilter = pxDoc->GetRootNode();

	pFilter = pFilterReader->Read(pxFilter);
	CPPUNIT_ASSERT(pFilter!=NULL);

	/**************************************************************/
	auge::FeatureClass* pFeatureClass = NULL;
	pFeatureClass = m_pWorkspace->OpenFeatureClass("cities");
	CPPUNIT_ASSERT(pFeatureClass!=NULL);

	auge::FeatureCursor* pCursor = NULL;
	pCursor = pFeatureClass->Query(pFilter);
	CPPUNIT_ASSERT(pCursor!=NULL);

	g_uint i=0;
	auge::Geometry	*pGeometry = NULL;
	auge::Feature	*pFeature = NULL;
	while((pFeature=pCursor->NextFeature())!=NULL)
	{	
		printf("\r%d",++i);
		pFeature->Release();
	}
	printf("\n");

	AUGE_SAFE_RELEASE(pCursor);
	AUGE_SAFE_RELEASE(pFeatureClass);

	/**************************************************************/

	AUGE_SAFE_RELEASE(pFilter);
	pxDoc->Close();
	AUGE_SAFE_RELEASE(pxDoc);
}

void OgcFilterTest::ReadBinaryComparison()
{
	const char* path = "E:\\Research\\Auge.GIS\\Engine\\Filter\\ogc\\BinaryComparison.xml";
	auge::FilterFactory* pFactory = auge::augeGetFilterFactoryInstance();
	auge::FilterReader* pFilterReader = pFactory->CreateFilerReader(NULL);
	auge::GFilter* pFilter = NULL;

	auge::XParser parser;
	auge::XDocument	*pxDoc = NULL;
	auge::XElement	*pxFilter = NULL;

	pxDoc = parser.Parse(path);
	pxFilter = pxDoc->GetRootNode();

	pFilter = pFilterReader->Read(pxFilter);
	CPPUNIT_ASSERT(pFilter!=NULL);

	/**************************************************************/
	auge::FeatureClass* pFeatureClass = NULL;
	pFeatureClass = m_pWorkspace->OpenFeatureClass("cities");
	CPPUNIT_ASSERT(pFeatureClass!=NULL);

	auge::FeatureCursor* pCursor = NULL;
	pCursor = pFeatureClass->Query(pFilter);
	CPPUNIT_ASSERT(pCursor!=NULL);

	g_uint i=0;
	auge::Geometry	*pGeometry = NULL;
	auge::Feature	*pFeature = NULL;
	while((pFeature=pCursor->NextFeature())!=NULL)
	{	
		printf("\r%d",++i);
		pFeature->Release();
	}
	printf("\n");

	AUGE_SAFE_RELEASE(pCursor);
	AUGE_SAFE_RELEASE(pFeatureClass);

	/**************************************************************/

	AUGE_SAFE_RELEASE(pFilter);
	pxDoc->Close();
	AUGE_SAFE_RELEASE(pxDoc);
}

void OgcFilterTest::ReadBinaryComparison_2()
{
	const char* path = "E:\\Research\\Auge.GIS\\Engine\\Filter\\ogc\\BinaryComparison_2.xml";
	auge::FilterFactory* pFactory = auge::augeGetFilterFactoryInstance();
	
	auge::XParser parser;
	auge::XDocument	*pxDoc = NULL;
	auge::XElement	*pxFilter = NULL;

	pxDoc = parser.Parse(path);
	pxFilter = pxDoc->GetRootNode();

	/**************************************************************/
	auge::FeatureClass* pFeatureClass = NULL;
	pFeatureClass = m_pWorkspace->OpenFeatureClass("cities");
	CPPUNIT_ASSERT(pFeatureClass!=NULL);

	auge::FilterReader* pFilterReader = pFactory->CreateFilerReader(pFeatureClass->GetFields());
	auge::GFilter* pFilter = NULL;
	pFilter = pFilterReader->Read(pxFilter);
	CPPUNIT_ASSERT(pFilter!=NULL);

	auge::FeatureCursor* pCursor = NULL;
	pCursor = pFeatureClass->Query(pFilter);
	CPPUNIT_ASSERT(pCursor!=NULL);

	g_uint i=0;
	auge::Geometry	*pGeometry = NULL;
	auge::Feature	*pFeature = NULL;
	while((pFeature=pCursor->NextFeature())!=NULL)
	{	
		printf("\r%d",++i);
		pFeature->Release();
	}
	printf("\n");

	AUGE_SAFE_RELEASE(pCursor);
	AUGE_SAFE_RELEASE(pFeatureClass);

	/**************************************************************/

	AUGE_SAFE_RELEASE(pFilter);
	pxDoc->Close();
	AUGE_SAFE_RELEASE(pxDoc);
}

void OgcFilterTest::ReadBinaryLogical()
{
	const char* path = "E:\\Research\\Auge.GIS\\Engine\\Filter\\ogc\\BinaryLogical.xml";
	auge::FilterFactory* pFactory = auge::augeGetFilterFactoryInstance();
	auge::FilterReader* pFilterReader = pFactory->CreateFilerReader(NULL);
	auge::GFilter* pFilter = NULL;

	auge::XParser parser;
	auge::XDocument	*pxDoc = NULL;
	auge::XElement	*pxFilter = NULL;

	pxDoc = parser.Parse(path);
	pxFilter = pxDoc->GetRootNode();

	pFilter = pFilterReader->Read(pxFilter);
	CPPUNIT_ASSERT(pFilter!=NULL);

	/**************************************************************/
	auge::FeatureClass* pFeatureClass = NULL;
	pFeatureClass = m_pWorkspace->OpenFeatureClass("cities");
	CPPUNIT_ASSERT(pFeatureClass!=NULL);

	auge::FeatureCursor* pCursor = NULL;
	pCursor = pFeatureClass->Query(pFilter);
	CPPUNIT_ASSERT(pCursor!=NULL);

	g_uint i=0;
	auge::Geometry	*pGeometry = NULL;
	auge::Feature	*pFeature = NULL;
	while((pFeature=pCursor->NextFeature())!=NULL)
	{	
		printf("\r%d",++i);
		pFeature->Release();
	}
	printf("\n");

	AUGE_SAFE_RELEASE(pCursor);
	AUGE_SAFE_RELEASE(pFeatureClass);

	/**************************************************************/

	AUGE_SAFE_RELEASE(pFilter);
	pxDoc->Close();
	AUGE_SAFE_RELEASE(pxDoc);
}

void OgcFilterTest::ReadBinarySpatial()
{
	const char* path = "E:\\Research\\Auge.GIS\\Engine\\Filter\\ogc\\BinarySpatial.xml";
	auge::FilterFactory* pFactory = auge::augeGetFilterFactoryInstance();
	auge::FilterReader* pFilterReader = pFactory->CreateFilerReader(NULL);
	auge::GFilter* pFilter = NULL;

	auge::XParser parser;
	auge::XDocument	*pxDoc = NULL;
	auge::XElement	*pxFilter = NULL;

	pxDoc = parser.Parse(path);
	pxFilter = pxDoc->GetRootNode();

	pFilter = pFilterReader->Read(pxFilter);
	CPPUNIT_ASSERT(pFilter!=NULL);

	/**************************************************************/
	auge::FeatureClass* pFeatureClass = NULL;
	pFeatureClass = m_pWorkspace->OpenFeatureClass("cities");
	CPPUNIT_ASSERT(pFeatureClass!=NULL);

	auge::FeatureCursor* pCursor = NULL;
	pCursor = pFeatureClass->Query(pFilter);
	CPPUNIT_ASSERT(pCursor!=NULL);

	g_uint i=0;
	auge::Geometry	*pGeometry = NULL;
	auge::Feature	*pFeature = NULL;
	while((pFeature=pCursor->NextFeature())!=NULL)
	{	
		printf("\r%d",++i);
		pFeature->Release();
	}
	printf("\n");

	AUGE_SAFE_RELEASE(pCursor);
	AUGE_SAFE_RELEASE(pFeatureClass);

	/**************************************************************/

	AUGE_SAFE_RELEASE(pFilter);
	pxDoc->Close();
	AUGE_SAFE_RELEASE(pxDoc);
}

void OgcFilterTest::ReadDistanceFilter()
{
	const char* path = "E:\\Research\\Auge.GIS\\Engine\\Filter\\ogc\\DistanceFilter.xml";
	auge::FilterFactory* pFactory = auge::augeGetFilterFactoryInstance();
	auge::FilterReader* pFilterReader = pFactory->CreateFilerReader(NULL);
	auge::GFilter* pFilter = NULL;

	auge::XParser parser;
	auge::XDocument	*pxDoc = NULL;
	auge::XElement	*pxFilter = NULL;

	pxDoc = parser.Parse(path);
	pxFilter = pxDoc->GetRootNode();

	pFilter = pFilterReader->Read(pxFilter);
	CPPUNIT_ASSERT(pFilter!=NULL);

	/**************************************************************/
	auge::FeatureClass* pFeatureClass = NULL;
	pFeatureClass = m_pWorkspace->OpenFeatureClass("cities");
	CPPUNIT_ASSERT(pFeatureClass!=NULL);

	auge::FeatureCursor* pCursor = NULL;
	pCursor = pFeatureClass->Query(pFilter);
	CPPUNIT_ASSERT(pCursor!=NULL);

	g_uint i=0;
	auge::Geometry	*pGeometry = NULL;
	auge::Feature	*pFeature = NULL;
	while((pFeature=pCursor->NextFeature())!=NULL)
	{	
		printf("\r%d",++i);
		pFeature->Release();
	}
	printf("\n");

	AUGE_SAFE_RELEASE(pCursor);
	AUGE_SAFE_RELEASE(pFeatureClass);

	/**************************************************************/

	AUGE_SAFE_RELEASE(pFilter);
	pxDoc->Close();
	AUGE_SAFE_RELEASE(pxDoc);
}

void OgcFilterTest::ReadIDFilter()
{
	const char* path = "E:\\Research\\Auge.GIS\\Engine\\Filter\\ogc\\IDFilter.xml";
	auge::FilterFactory* pFactory = auge::augeGetFilterFactoryInstance();
	auge::FilterReader* pFilterReader = pFactory->CreateFilerReader(NULL);
	auge::GFilter* pFilter = NULL;

	auge::XParser parser;
	auge::XDocument	*pxDoc = NULL;
	auge::XElement	*pxFilter = NULL;

	pxDoc = parser.Parse(path);
	pxFilter = pxDoc->GetRootNode();

	pFilter = pFilterReader->Read(pxFilter);
	CPPUNIT_ASSERT(pFilter!=NULL);

	/**************************************************************/
	auge::FeatureClass* pFeatureClass = NULL;
	pFeatureClass = m_pWorkspace->OpenFeatureClass("cities");
	CPPUNIT_ASSERT(pFeatureClass!=NULL);

	auge::FeatureCursor* pCursor = NULL;
	pCursor = pFeatureClass->Query(pFilter);
	CPPUNIT_ASSERT(pCursor!=NULL);

	g_uint i=0;
	auge::Geometry	*pGeometry = NULL;
	auge::Feature	*pFeature = NULL;
	while((pFeature=pCursor->NextFeature())!=NULL)
	{	
		printf("\r%d",++i);
		pFeature->Release();
	}
	printf("\n");

	AUGE_SAFE_RELEASE(pCursor);
	AUGE_SAFE_RELEASE(pFeatureClass);

	/**************************************************************/

	AUGE_SAFE_RELEASE(pFilter);
	pxDoc->Close();
	AUGE_SAFE_RELEASE(pxDoc);
}

void OgcFilterTest::ReadLike()
{
	const char* path = "E:\\Research\\Auge.GIS\\Engine\\Filter\\ogc\\Like.xml";
	auge::FilterFactory* pFactory = auge::augeGetFilterFactoryInstance();
	auge::FilterReader* pFilterReader = pFactory->CreateFilerReader(NULL);
	auge::GFilter* pFilter = NULL;

	auge::XParser parser;
	auge::XDocument	*pxDoc = NULL;
	auge::XElement	*pxFilter = NULL;

	pxDoc = parser.Parse(path);
	pxFilter = pxDoc->GetRootNode();

	pFilter = pFilterReader->Read(pxFilter);
	CPPUNIT_ASSERT(pFilter!=NULL);

	/**************************************************************/
	auge::FeatureClass* pFeatureClass = NULL;
	pFeatureClass = m_pWorkspace->OpenFeatureClass("cities");
	CPPUNIT_ASSERT(pFeatureClass!=NULL);

	auge::FeatureCursor* pCursor = NULL;
	pCursor = pFeatureClass->Query(pFilter);
	CPPUNIT_ASSERT(pCursor!=NULL);

	g_uint i=0;
	auge::Geometry	*pGeometry = NULL;
	auge::Feature	*pFeature = NULL;
	while((pFeature=pCursor->NextFeature())!=NULL)
	{	
		printf("\r%d",++i);
		pFeature->Release();
	}
	printf("\n");

	AUGE_SAFE_RELEASE(pCursor);
	AUGE_SAFE_RELEASE(pFeatureClass);

	/**************************************************************/

	AUGE_SAFE_RELEASE(pFilter);
	pxDoc->Close();
	AUGE_SAFE_RELEASE(pxDoc);
}

void OgcFilterTest::ReadNull()
{
	const char* path = "E:\\Research\\Auge.GIS\\Engine\\Filter\\ogc\\Null.xml";
	auge::FilterFactory* pFactory = auge::augeGetFilterFactoryInstance();
	auge::FilterReader* pFilterReader = pFactory->CreateFilerReader(NULL);
	auge::GFilter* pFilter = NULL;

	auge::XParser parser;
	auge::XDocument	*pxDoc = NULL;
	auge::XElement	*pxFilter = NULL;

	pxDoc = parser.Parse(path);
	pxFilter = pxDoc->GetRootNode();

	pFilter = pFilterReader->Read(pxFilter);
	CPPUNIT_ASSERT(pFilter!=NULL);

	/**************************************************************/
	auge::FeatureClass* pFeatureClass = NULL;
	pFeatureClass = m_pWorkspace->OpenFeatureClass("cities");
	CPPUNIT_ASSERT(pFeatureClass!=NULL);

	auge::FeatureCursor* pCursor = NULL;
	pCursor = pFeatureClass->Query(pFilter);
	CPPUNIT_ASSERT(pCursor!=NULL);

	g_uint i=0;
	auge::Geometry	*pGeometry = NULL;
	auge::Feature	*pFeature = NULL;
	while((pFeature=pCursor->NextFeature())!=NULL)
	{	
		printf("\r%d",++i);
		pFeature->Release();
	}
	printf("\n");

	AUGE_SAFE_RELEASE(pCursor);
	AUGE_SAFE_RELEASE(pFeatureClass);

	/**************************************************************/

	AUGE_SAFE_RELEASE(pFilter);
	pxDoc->Close();
	AUGE_SAFE_RELEASE(pxDoc);
}

void OgcFilterTest::ReadSpatialAttribute()
{
	const char* path = "E:\\Research\\Auge.GIS\\Engine\\Filter\\ogc\\SpatialAttribute.xml";
	auge::FilterFactory* pFactory = auge::augeGetFilterFactoryInstance();
	auge::FilterReader* pFilterReader = pFactory->CreateFilerReader(NULL);
	auge::GFilter* pFilter = NULL;

	auge::XParser parser;
	auge::XDocument	*pxDoc = NULL;
	auge::XElement	*pxFilter = NULL;

	pxDoc = parser.Parse(path);
	pxFilter = pxDoc->GetRootNode();

	pFilter = pFilterReader->Read(pxFilter);
	CPPUNIT_ASSERT(pFilter!=NULL);

	/**************************************************************/
	auge::FeatureClass* pFeatureClass = NULL;
	pFeatureClass = m_pWorkspace->OpenFeatureClass("cities");
	CPPUNIT_ASSERT(pFeatureClass!=NULL);

	auge::FeatureCursor* pCursor = NULL;
	pCursor = pFeatureClass->Query(pFilter);
	CPPUNIT_ASSERT(pCursor!=NULL);

	g_uint i=0;
	auge::Geometry	*pGeometry = NULL;
	auge::Feature	*pFeature = NULL;
	while((pFeature=pCursor->NextFeature())!=NULL)
	{	
		printf("\r%d",++i);
		pFeature->Release();
	}
	printf("\n");

	AUGE_SAFE_RELEASE(pCursor);
	AUGE_SAFE_RELEASE(pFeatureClass);

	/**************************************************************/

	AUGE_SAFE_RELEASE(pFilter);
	pxDoc->Close();
	AUGE_SAFE_RELEASE(pxDoc);
}

void OgcFilterTest::WithinFilteTest()
{
	const char* path = "E:\\Research\\Auge.GIS\\Engine\\Filter\\ogc\\BinarySpatial_Within.xml";
	auge::FilterFactory* pFactory = auge::augeGetFilterFactoryInstance();
	auge::FilterReader* pFilterReader = pFactory->CreateFilerReader(NULL);
	auge::GFilter* pFilter = NULL;

	auge::XParser parser;
	auge::XDocument	*pxDoc = NULL;
	auge::XElement	*pxFilter = NULL;

	pxDoc = parser.Parse(path);
	pxFilter = pxDoc->GetRootNode();

	pFilter = pFilterReader->Read(pxFilter);
	CPPUNIT_ASSERT(pFilter!=NULL);

	/**************************************************************/
	auge::FeatureClass* pFeatureClass = NULL;
	pFeatureClass = m_pWorkspace->OpenFeatureClass("country");
	CPPUNIT_ASSERT(pFeatureClass!=NULL);

	auge::FeatureCursor* pCursor = NULL;
	pCursor = pFeatureClass->Query(pFilter);
	CPPUNIT_ASSERT(pCursor!=NULL);

	g_uint i=0;
	auge::Geometry	*pGeometry = NULL;
	auge::Feature	*pFeature = NULL;
	while((pFeature=pCursor->NextFeature())!=NULL)
	{	
		printf("\r%d",++i);
		pFeature->Release();
	}
	printf("\n");

	AUGE_SAFE_RELEASE(pCursor);
	AUGE_SAFE_RELEASE(pFeatureClass);

	/**************************************************************/

	AUGE_SAFE_RELEASE(pFilter);
	pxDoc->Close();
	AUGE_SAFE_RELEASE(pxDoc);
}

void OgcFilterTest::DWithinFilteTest()
{
	const char* path = "E:\\Research\\Auge.GIS\\Engine\\Filter\\ogc\\DWithinFilter.xml";
	auge::FilterFactory* pFactory = auge::augeGetFilterFactoryInstance();
	auge::FilterReader* pFilterReader = pFactory->CreateFilerReader(NULL);
	auge::GFilter* pFilter = NULL;

	auge::XParser parser;
	auge::XDocument	*pxDoc = NULL;
	auge::XElement	*pxFilter = NULL;

	pxDoc = parser.Parse(path);
	pxFilter = pxDoc->GetRootNode();

	pFilter = pFilterReader->Read(pxFilter);
	CPPUNIT_ASSERT(pFilter!=NULL);

	/**************************************************************/
	auge::FeatureClass* pFeatureClass = NULL;
	pFeatureClass = m_pWorkspace->OpenFeatureClass("cities");
	CPPUNIT_ASSERT(pFeatureClass!=NULL);

	auge::FeatureCursor* pCursor = NULL;
	pCursor = pFeatureClass->Query(pFilter);
	CPPUNIT_ASSERT(pCursor!=NULL);

	g_uint i=0;
	auge::Geometry	*pGeometry = NULL;
	auge::Feature	*pFeature = NULL;
	while((pFeature=pCursor->NextFeature())!=NULL)
	{	
		//printf("\r%d",++i);
		printf("%d\n",pFeature->GetFID());
		pFeature->Release();
	}
	printf("\n");

	AUGE_SAFE_RELEASE(pCursor);
	AUGE_SAFE_RELEASE(pFeatureClass);

	/**************************************************************/

	AUGE_SAFE_RELEASE(pFilter);
	pxDoc->Close();
	AUGE_SAFE_RELEASE(pxDoc);
}


void QueryTest()
{

}

