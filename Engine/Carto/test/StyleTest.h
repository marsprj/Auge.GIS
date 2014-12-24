#ifndef __AUGE_MAP_TEST__H__
#define __AUGE_MAP_TEST__H__

#include "cppunit/extensions/HelperMacros.h" 

#include "AugeData.h"

class StyleTest : public CppUnit::TestFixture 
{
	CPPUNIT_TEST_SUITE(StyleTest);
	//CPPUNIT_TEST(ReadSLD);
	CPPUNIT_TEST(CreateSLD);
	//CPPUNIT_TEST(UpdateSLD);
	//CPPUNIT_TEST(RemoveSLD);
	CPPUNIT_TEST_SUITE_END();
public:
	void setUp();
	void tearDown();

private:
	void		CreateSLD();
	void		UpdateSLD();
	void		RemoveSLD();
	void		ReadSLD();

private:
	auge::GConnection* m_pConnection;
};

#endif //__AUGE_MAP_TEST__H__