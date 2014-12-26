#ifndef __AUGE_SERVICE_TEST__H__
#define __AUGE_SERVICE_TEST__H__

#include "cppunit/extensions/HelperMacros.h" 

class ServiceTest : public CppUnit::TestFixture 
{
	CPPUNIT_TEST_SUITE(ServiceTest);
	//CPPUNIT_TEST(ManagerTest);
	CPPUNIT_TEST(RegisterService);
	//CPPUNIT_TEST(UnRegisterService);
	//CPPUNIT_TEST(RegisterMap);
	//CPPUNIT_TEST(DoGetMap);
	CPPUNIT_TEST(DoGetMapCapabilities);
	CPPUNIT_TEST(DoPost);
	CPPUNIT_TEST_SUITE_END();
public:
	void setUp();
	void tearDown();

private:
	void RegisterService();
	void UnRegisterService();
	void RegisterMap();
	void DoGetMap();
	void DoGetMapCapabilities();
	void DoGet();
	void DoPost();
	

	void ManagerTest();


};

#endif //__AUGE_SERVICE_TEST__H__