#ifndef __AUGE_RASTER_TEST__H__
#define __AUGE_RASTER_TEST__H__

#include "cppunit/extensions/HelperMacros.h" 
#include "AugeTile.h"

class RasterTest : public CppUnit::TestFixture 
{
	CPPUNIT_TEST_SUITE(RasterTest);
	//CPPUNIT_TEST(ExtractByRectangle);
	//CPPUNIT_TEST(Stretch);
	//CPPUNIT_TEST(EdgeDetorSobol);
	//CPPUNIT_TEST(Graylize);
	CPPUNIT_TEST(ToJPG);
	CPPUNIT_TEST_SUITE_END();
public:
	void setUp();
	void tearDown();

private:
	void	ExtractByRectangle();
	void	Stretch();
	void	EdgeDetorSobol();
	void	Graylize();
	void	ToJPG();

private:
	auge::GConnection	*m_pConnection;
};

#endif //__AUGE_RASTER_TEST__H__
