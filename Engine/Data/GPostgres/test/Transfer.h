#ifndef __AUGE_TRANSFER_TEST__H__
#define __AUGE_TRANSFER_TEST__H__

#include "cppunit/extensions/HelperMacros.h" 
#include "AugeFeature.h"

class TransferTest : public CppUnit::TestFixture 
{
	CPPUNIT_TEST_SUITE(TransferTest);
	CPPUNIT_TEST(Transfer);
	CPPUNIT_TEST_SUITE_END();
public:
	void setUp();
	void tearDown();

private:
	void Transfer();

private:
	auge::FeatureWorkspace* m_pwsSHP;
	auge::FeatureWorkspace* m_pwsPGS;
};

#endif //__AUGE_TRANSFER_TEST__H__