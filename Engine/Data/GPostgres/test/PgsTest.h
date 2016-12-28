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
//	CPPUNIT_TEST(QueryBetween);
//	CPPUNIT_TEST(QueryBetween_2);
//	CPPUNIT_TEST(QueryLike);
//	CPPUNIT_TEST(QueryTest);
//	CPPUNIT_TEST(QueryTest_2);
//	CPPUNIT_TEST(GetUniqueValue);
//	CPPUNIT_TEST(GetNullValue);
//	CPPUNIT_TEST(GetAQI);
//	CPPUNIT_TEST(GetAQIQuery);
//	CPPUNIT_TEST(RefreshFeatureClass);
	CPPUNIT_TEST(UpdatePOI);
	CPPUNIT_TEST_SUITE_END();
public:
	void setUp();
	void tearDown();

private:
	void ReadTest();
	void QueryExent();
	void QueryBinaryComparision();
	void QueryBetween();
	void QueryBetween_2();
	void QueryLike();

	void QueryTest();
	void QueryTest_2();

	void GetUniqueValue();

	void GetNullValue();

	void GetAQI();
	void GetAQIQuery();

	void RefreshFeatureClass();

	void UpdatePOI();

private:
	auge::FeatureWorkspace* m_pWorkspace;
};

#endif //__AUGE_CANVAS_TEST__H__
