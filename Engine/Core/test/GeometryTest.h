#ifndef __AUGE_SERVICE_TEST__H__
#define __AUGE_SERVICE_TEST__H__

#include "cppunit/extensions/HelperMacros.h" 

class GeometryTest : public CppUnit::TestFixture 
{
	CPPUNIT_TEST_SUITE(GeometryTest);
	CPPUNIT_TEST(WKTTest);
	CPPUNIT_TEST_SUITE_END();
public:
	void setUp();
	void tearDown();

private:
	void WKTTest();

};

#endif //__AUGE_SERVICE_TEST__H__