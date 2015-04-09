#include "FilterTest.h"
#include "AugeFilter.h"
#include "AugeCore.h"
#include "AugeXML.h"

CPPUNIT_TEST_SUITE_REGISTRATION(FilterTest);

void FilterTest::setUp() 
{
	printf("setUp\n");
}

void FilterTest::tearDown()
{
	printf("tearDown\n");
}

void FilterTest::ReadArithmeticComparison()
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

	AUGE_SAFE_RELEASE(pFilter);
	pxDoc->Close();
	AUGE_SAFE_RELEASE(pxDoc);
}

void FilterTest::ReadBBox()
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

	AUGE_SAFE_RELEASE(pFilter);
	pxDoc->Close();
	AUGE_SAFE_RELEASE(pxDoc);
}

void FilterTest::ReadBetween()
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

	AUGE_SAFE_RELEASE(pFilter);
	pxDoc->Close();
	AUGE_SAFE_RELEASE(pxDoc);
}

void FilterTest::ReadBinaryComparison()
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

	AUGE_SAFE_RELEASE(pFilter);
	pxDoc->Close();
	AUGE_SAFE_RELEASE(pxDoc);
}

void FilterTest::ReadBinaryComparison_2()
{
	const char* path = "E:\\Research\\Auge.GIS\\Engine\\Filter\\ogc\\BinaryComparison_2.xml";
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

	AUGE_SAFE_RELEASE(pFilter);
	pxDoc->Close();
	AUGE_SAFE_RELEASE(pxDoc);
}

void FilterTest::ReadBinaryLogical()
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

	AUGE_SAFE_RELEASE(pFilter);
	pxDoc->Close();
	AUGE_SAFE_RELEASE(pxDoc);
}

void FilterTest::ReadBinarySpatial()
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

	AUGE_SAFE_RELEASE(pFilter);
	pxDoc->Close();
	AUGE_SAFE_RELEASE(pxDoc);
}

void FilterTest::ReadDistanceFilter()
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

	AUGE_SAFE_RELEASE(pFilter);
	pxDoc->Close();
	AUGE_SAFE_RELEASE(pxDoc);
}

void FilterTest::ReadIDFilter()
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

	AUGE_SAFE_RELEASE(pFilter);
	pxDoc->Close();
	AUGE_SAFE_RELEASE(pxDoc);
}

void FilterTest::ReadLike()
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

	AUGE_SAFE_RELEASE(pFilter);
	pxDoc->Close();
	AUGE_SAFE_RELEASE(pxDoc);
}

void FilterTest::ReadNull()
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

	AUGE_SAFE_RELEASE(pFilter);
	pxDoc->Close();
	AUGE_SAFE_RELEASE(pxDoc);
}

void FilterTest::ReadSpatialAttribute()
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

	AUGE_SAFE_RELEASE(pFilter);
	pxDoc->Close();
	AUGE_SAFE_RELEASE(pxDoc);
}

void FilterTest::ReadFilterText()
{
	const char* text = "<?xml version=\"1.0\" encoding=\"UTF-8\"?><ogc:Filter xmlns=\"http://www.opengis.net/sld\" xmlns:sld=\"http://www.opengis.net/sld\" xmlns:ogc=\"http://www.opengis.net/ogc\" xmlns:gml=\"http://www.opengis.net/gml\" version=\"1.0.0\"><ogc:PropertyIsEqualTo><ogc:PropertyName>continent</ogc:PropertyName><ogc:Literal>Antarctica</ogc:Literal></ogc:PropertyIsEqualTo></ogc:Filter>";
	auge::FilterFactory* pFactory = auge::augeGetFilterFactoryInstance();
	auge::FilterReader* pFilterReader = pFactory->CreateFilerReader(NULL);
	auge::GFilter* pFilter = NULL;

	auge::XParser parser;
	auge::XDocument	*pxDoc = NULL;
	auge::XElement	*pxFilter = NULL;

	pxDoc = parser.ParseMemory(text);
	pxFilter = pxDoc->GetRootNode();

	pFilter = pFilterReader->Read(pxFilter);
	CPPUNIT_ASSERT(pFilter!=NULL);

	AUGE_SAFE_RELEASE(pFilter);
	pxDoc->Close();
	AUGE_SAFE_RELEASE(pxDoc);

}