#ifndef __AUGE_CANVAS_TEST__H__
#define __AUGE_CANVAS_TEST__H__

#include "cppunit/extensions/HelperMacros.h" 
#include "AugeFeature.h"
#include "AugeField.h"
#include "AugeFilter.h"

class PgsTest : public CppUnit::TestFixture 
{
	CPPUNIT_TEST_SUITE(PgsTest);
//	CPPUNIT_TEST(ReadTest);
//	CPPUNIT_TEST(QueryExent);
//	CPPUNIT_TEST(QueryBinaryComparision);
//	CPPUNIT_TEST(QueryTest);
//	CPPUNIT_TEST(QueryTest_2);
//	CPPUNIT_TEST(GetUniqueValue);
	CPPUNIT_TEST(GetNullValue);
	CPPUNIT_TEST_SUITE_END();
public:
	void setUp();
	void tearDown();

private:
	void ReadTest();
	void QueryExent();
	void QueryBinaryComparision();

	void QueryTest();
	void QueryTest_2();

	void GetUniqueValue();

	void GetNullValue();

private:
	auge::FeatureWorksapce* m_pWorkspace;
};

#endif //__AUGE_CANVAS_TEST__H__
