#ifndef __AUGE_CANVAS_TEST__H__
#define __AUGE_CANVAS_TEST__H__

#include "cppunit/extensions/HelperMacros.h" 

class EngineTest : public CppUnit::TestFixture 
{
	CPPUNIT_TEST_SUITE(EngineTest);
	//CPPUNIT_TEST(RegistrTest);
	CPPUNIT_TEST(ConnectionTest);
	CPPUNIT_TEST_SUITE_END();
public:
	void setUp();
	void tearDown();

private:
	void RegistrTest();
	void ConnectionTest();
};

#endif //__AUGE_CANVAS_TEST__H__