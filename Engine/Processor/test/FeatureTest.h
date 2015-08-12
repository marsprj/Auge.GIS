#ifndef __AUGE_FEATURE_TEST__H__
#define __AUGE_FEATURE_TEST__H__

#include "cppunit/extensions/HelperMacros.h" 
#include "AugeTile.h"

class FeatureTest : public CppUnit::TestFixture 
{
	CPPUNIT_TEST_SUITE(FeatureTest);
	//CPPUNIT_TEST(FeatureImportTest);
	CPPUNIT_TEST(FeatureExportTest);
	//CPPUNIT_TEST(CsvImportTest);

	//CPPUNIT_TEST(ProjectTest);

	CPPUNIT_TEST_SUITE_END();
public:
	void setUp();
	void tearDown();

private:
	void	FeatureImportTest();
	void	FeatureExportTest();

	void	CsvImportTest();

	void	ProjectTest();

private:
	auge::GConnection* m_pConnection;
};

#endif //__AUGE_FEATURE_TEST__H__
