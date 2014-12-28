#ifndef __AUGE_CANVAS_TEST__H__
#define __AUGE_CANVAS_TEST__H__

#include "cppunit/extensions/HelperMacros.h" 
#include "AugeFeature.h"
#include "AugeField.h"
#include "AugeFilter.h"

class PgsTest : public CppUnit::TestFixture 
{
	CPPUNIT_TEST_SUITE(PgsTest);
	CPPUNIT_TEST(ReadTest);
	CPPUNIT_TEST(QueryExent);
	CPPUNIT_TEST(QueryBinaryComparision);
	CPPUNIT_TEST_SUITE_END();
public:
	void setUp();
	void tearDown();

private:
	void ReadTest();
	void QueryExent();
	void QueryBinaryComparision();

private:
	auge::FeatureWorksapce* m_pWorkspace;
};

#endif //__AUGE_CANVAS_TEST__H__