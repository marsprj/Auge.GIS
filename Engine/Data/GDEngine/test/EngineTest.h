#ifndef __AUGE_CANVAS_TEST__H__
#define __AUGE_CANVAS_TEST__H__

#include "cppunit/extensions/HelperMacros.h" 

#include "AugeData.h"

class EngineTest : public CppUnit::TestFixture 
{
	CPPUNIT_TEST_SUITE(EngineTest);
	//CPPUNIT_TEST(RegistrTest);
	//CPPUNIT_TEST(ConnectionTest);
	CPPUNIT_TEST(ConnectionTest_2);
	CPPUNIT_TEST_SUITE_END();
public:
	void setUp();
	void tearDown();

private:
	void RegistrTest();
	void ConnectionTest();
	void ConnectionTest_2();

private:
	auge::GConnection* m_pConnection;
};

#endif //__AUGE_CANVAS_TEST__H__