#ifndef __AUGE_CANVAS_TEST__H__
#define __AUGE_CANVAS_TEST__H__

#include "cppunit/extensions/HelperMacros.h" 

class ShapeTest : public CppUnit::TestFixture 
{
	CPPUNIT_TEST_SUITE(ShapeTest);
	CPPUNIT_TEST(ReadTest);
	CPPUNIT_TEST_SUITE_END();
public:
	void setUp();
	void tearDown();

private:
	void ReadTest();
};

#endif //__AUGE_CANVAS_TEST__H__