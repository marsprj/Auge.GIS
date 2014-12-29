#ifndef __AUGE_SERVICE_TEST__H__
#define __AUGE_SERVICE_TEST__H__

#include "cppunit/extensions/HelperMacros.h" 

class FilterTest : public CppUnit::TestFixture 
{
	CPPUNIT_TEST_SUITE(FilterTest);
	//CPPUNIT_TEST(ReadArithmeticComparison);
	CPPUNIT_TEST(ReadBBox);
	//CPPUNIT_TEST(ReadBetween);
	//CPPUNIT_TEST(ReadBinaryComparison);
	CPPUNIT_TEST(ReadBinaryComparison_2);
	CPPUNIT_TEST(ReadBinaryLogical);
	//CPPUNIT_TEST(ReadBinarySpatial);
	//CPPUNIT_TEST(ReadDistanceFilter);
	CPPUNIT_TEST(ReadIDFilter);
	//CPPUNIT_TEST(ReadLike);
	//CPPUNIT_TEST(ReadNull);
	CPPUNIT_TEST(ReadSpatialAttribute);
	CPPUNIT_TEST_SUITE_END();
public:
	void setUp();
	void tearDown();

private:
	void ReadArithmeticComparison();
	void ReadBBox();
	void ReadBetween();
	void ReadBinaryComparison();
	void ReadBinaryComparison_2();
	void ReadBinaryLogical();
	void ReadBinarySpatial();
	void ReadDistanceFilter();
	void ReadIDFilter();
	void ReadLike();
	void ReadNull();
	void ReadSpatialAttribute();

};

#endif //__AUGE_SERVICE_TEST__H__