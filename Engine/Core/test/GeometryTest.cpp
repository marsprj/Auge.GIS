#include "GeometryTest.h"
#include "AugeGeometry.h"
#include "AugeCore.h"

CPPUNIT_TEST_SUITE_REGISTRATION(GeometryTest);

void GeometryTest::setUp() 
{
	printf("setUp\n");
}

void GeometryTest::tearDown()
{
	printf("tearDown\n");
}

void GeometryTest::WKTTest()
{
	//const char* wkt = "POINT(123.456 678.890)";
	const char* wkt = "LINESTRING(0 0,1 1,3 7)";

	auge::Geometry* pGeometry = NULL;
	auge::GeometryFactory* pFactory = NULL;
	pFactory = auge::augeGetGeometryFactoryInstance();

	pGeometry = pFactory->CreateGeometryFromWKT(wkt);

	g_uchar* wkb = pGeometry->AsBinary();
	auge::WKBGeometryType type = (auge::WKBGeometryType)(WKBTYPE(wkb));
	switch(type)
	{
	default:
		break;
	}
}

