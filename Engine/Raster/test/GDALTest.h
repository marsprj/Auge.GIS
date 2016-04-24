#ifndef __AUGE_RASTER_TEST__H__
#define __AUGE_RASTER_TEST__H__

#include "cppunit/extensions/HelperMacros.h" 
#include "AugeTile.h"

class GDALTest : public CppUnit::TestFixture 
{
	CPPUNIT_TEST_SUITE(GDALTest);
	//CPPUNIT_TEST(Read_shp);
	//CPPUNIT_TEST(Read_tif);
	CPPUNIT_TEST(Show_Drivers);
	CPPUNIT_TEST_SUITE_END();
public:
	void setUp();
	void tearDown();

private:
	void	Read_shp();
	void	Read_tif();
	void	Show_Drivers();


private:
	auge::GConnection	*m_pConnection;
};

#endif //__AUGE_RASTER_TEST__H__
