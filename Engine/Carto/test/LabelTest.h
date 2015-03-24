#ifndef __AUGE_LABEL_TEST__H__
#define __AUGE_LABEL_TEST__H__

#include "cppunit/extensions/HelperMacros.h" 

#include "AugeData.h"
#include "AugeStyle.h"

class LabelTest : public CppUnit::TestFixture 
{
	CPPUNIT_TEST_SUITE(LabelTest);
	CPPUNIT_TEST(DrawMap);
	CPPUNIT_TEST_SUITE_END();
public:
	void setUp();
	void tearDown();

private:
	void DrawMap();

private:
	auge::Style* LabelTest::LoadSLD(const char* path);
private:
	auge::GConnection	*m_pConnection;
};

#endif //__AUGE_LABEL_TEST__H__