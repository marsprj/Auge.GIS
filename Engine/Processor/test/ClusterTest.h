#ifndef __AUGE_CLUSTER_TEST__H__
#define __AUGE_CLUSTER_TEST__H__

#include "cppunit/extensions/HelperMacros.h" 
#include "AugeTile.h"

class ClusterTest : public CppUnit::TestFixture 
{
	CPPUNIT_TEST_SUITE(ClusterTest);
	CPPUNIT_TEST(KMeanTest);
	CPPUNIT_TEST_SUITE_END();
public:
	void setUp();
	void tearDown();

private:
	void	KMeanTest();

private:
	auge::GConnection	*m_pConnection;
};

#endif //__AUGE_CLUSTER_TEST__H__
