#include "GeometryTest.h"
#include "AugeGeometry.h"
#include "AugeCore.h"
#include "AugeXML.h"

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

void GeometryTest::GML_Point_Test()
{
	const char* wkt = "POINT(123.456 678.890)";
	
	auge::Geometry* pGeometry = NULL;
	auge::GeometryFactory* pFactory = NULL;
	pFactory = auge::augeGetGeometryFactoryInstance();

	pGeometry = pFactory->CreateGeometryFromWKT(wkt);

	auge::GMLWriter* pWriter = pFactory->CreateGMLWriter();

	auge::XDocument xDoc;
	auge::XElement* pxRoot = xDoc.CreateRootNode("root",NULL, NULL);
	pWriter->Write(pxRoot, pGeometry, 4326, "GML2");

	int len = 0;
	g_char* buffer = NULL;
	xDoc.WriteToString((g_uchar**)(&buffer), len,"GBK",1);
	printf(buffer);

	pWriter->Release();
}

void GeometryTest::GML_LineString_Test()
{	
	const char* wkt = "LINESTRING(0 0,1 1,3 7)";

	auge::Geometry* pGeometry = NULL;
	auge::GeometryFactory* pFactory = NULL;
	pFactory = auge::augeGetGeometryFactoryInstance();

	pGeometry = pFactory->CreateGeometryFromWKT(wkt);

	auge::GMLWriter* pWriter = pFactory->CreateGMLWriter();

	auge::XDocument xDoc;
	auge::XElement* pxRoot = xDoc.CreateRootNode("root",NULL, NULL);
	pWriter->Write(pxRoot, pGeometry, 4326, "GML2");

	int len = 0;
	g_char* buffer = NULL;
	xDoc.WriteToString((g_uchar**)(&buffer), len,"GBK",1);
	printf(buffer);

	pWriter->Release();
}


void GeometryTest::GML_Polygon_Test()
{
	const char* wkt = "POLYGON((0 0,0 10,10 10,10 0,0 0),(2 2,2 8,8 8,8 2,2 2))";

	auge::Geometry* pGeometry = NULL;
	auge::GeometryFactory* pFactory = NULL;
	pFactory = auge::augeGetGeometryFactoryInstance();

	pGeometry = pFactory->CreateGeometryFromWKT(wkt);

	auge::GMLWriter* pWriter = pFactory->CreateGMLWriter();

	auge::XDocument xDoc;
	auge::XElement* pxRoot = xDoc.CreateRootNode("root",NULL, NULL);
	pWriter->Write(pxRoot, pGeometry, 4326, "GML2");

	int len = 0;
	g_char* buffer = NULL;
	xDoc.WriteToString((g_uchar**)(&buffer), len,"GBK",1);
	printf(buffer);

	pWriter->Release();
}

void GeometryTest::GML_MultiPoint_Test()
{
	const char* wkt = "MULTIPOINT(0 0,1 1,3 7)";

	auge::Geometry* pGeometry = NULL;
	auge::GeometryFactory* pFactory = NULL;
	pFactory = auge::augeGetGeometryFactoryInstance();

	pGeometry = pFactory->CreateGeometryFromWKT(wkt);

	auge::GMLWriter* pWriter = pFactory->CreateGMLWriter();

	auge::XDocument xDoc;
	auge::XElement* pxRoot = xDoc.CreateRootNode("root",NULL, NULL);
	pWriter->Write(pxRoot, pGeometry, 4326, "GML2");

	int len = 0;
	g_char* buffer = NULL;
	xDoc.WriteToString((g_uchar**)(&buffer), len,"GBK",1);
	printf(buffer);

	pWriter->Release();
}

void GeometryTest::GML_MultiLineString_Test()
{
	const char* wkt = "MULTILINESTRING((0 0,0 10,10 10,10 0,0 0),(2 2,2 8,8 8,8 2,2 2))";

	auge::Geometry* pGeometry = NULL;
	auge::GeometryFactory* pFactory = NULL;
	pFactory = auge::augeGetGeometryFactoryInstance();

	pGeometry = pFactory->CreateGeometryFromWKT(wkt);

	auge::GMLWriter* pWriter = pFactory->CreateGMLWriter();

	auge::XDocument xDoc;
	auge::XElement* pxRoot = xDoc.CreateRootNode("root",NULL, NULL);
	pWriter->Write(pxRoot, pGeometry, 4326, "GML2");

	int len = 0;
	g_char* buffer = NULL;
	xDoc.WriteToString((g_uchar**)(&buffer), len,"GBK",1);
	printf(buffer);

	pWriter->Release();
}

void GeometryTest::GML_MultiPolygon_Test()
{
	const char* wkt = "MULTIPOLYGON(((0 0,0 10,10 10,10 0,0 0),(2 2,2 8,8 8,8 2,2 2)),((3 -3,6 -3,6 -6,3 -6,3 -3)))";

	auge::Geometry* pGeometry = NULL;
	auge::GeometryFactory* pFactory = NULL;
	pFactory = auge::augeGetGeometryFactoryInstance();

	pGeometry = pFactory->CreateGeometryFromWKT(wkt);

	auge::GMLWriter* pWriter = pFactory->CreateGMLWriter();

	auge::XDocument xDoc;
	auge::XElement* pxRoot = xDoc.CreateRootNode("root",NULL, NULL);
	pWriter->Write(pxRoot, pGeometry, 4326, "GML2");

	int len = 0;
	g_char* buffer = NULL;
	xDoc.WriteToString((g_uchar**)(&buffer), len,"GBK",1);
	printf(buffer);

	pWriter->Release();
}