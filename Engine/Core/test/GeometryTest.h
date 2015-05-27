#ifndef __AUGE_SERVICE_TEST__H__
#define __AUGE_SERVICE_TEST__H__

#include "cppunit/extensions/HelperMacros.h" 

class GeometryTest : public CppUnit::TestFixture 
{
	CPPUNIT_TEST_SUITE(GeometryTest);
	//CPPUNIT_TEST(WKTTest);
	//CPPUNIT_TEST(GML_Point_Test);
	//CPPUNIT_TEST(GML_LineString_Test);
	//CPPUNIT_TEST(GML_Polygon_Test);
	//CPPUNIT_TEST(GML_MultiPoint_Test);
	//CPPUNIT_TEST(GML_MultiLineString_Test);
	//CPPUNIT_TEST(GML_MultiPolygon_Test);
	//CPPUNIT_TEST(Memory_Test);

	CPPUNIT_TEST(GML_Point_Read);
	CPPUNIT_TEST_SUITE_END();
public:
	void setUp();
	void tearDown();

private:
	void WKTTest();
	void GML_Point_Test();
	void GML_LineString_Test();
	void GML_Polygon_Test();
	void GML_MultiPoint_Test();
	void GML_MultiLineString_Test();
	void GML_MultiPolygon_Test();

	void GML_Point_Read();

	void XML_Create_Test();

	void Memory_Test();
};

#endif //__AUGE_SERVICE_TEST__H__