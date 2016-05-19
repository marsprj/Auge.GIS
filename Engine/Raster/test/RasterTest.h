#ifndef __AUGE_RASTER_TEST__H__
#define __AUGE_RASTER_TEST__H__

#include "cppunit/extensions/HelperMacros.h" 
#include "AugeTile.h"

class RasterTest : public CppUnit::TestFixture 
{
	CPPUNIT_TEST_SUITE(RasterTest);
	//CPPUNIT_TEST(Read_png);
	//CPPUNIT_TEST(Read_tif);
	//CPPUNIT_TEST(Create_Raster);
	CPPUNIT_TEST(Read_jpg);
	//CPPUNIT_TEST(ExtractByRectangle);
	CPPUNIT_TEST_SUITE_END();
public:
	void setUp();
	void tearDown();

private:
	void	ExtractByRectangle();

	void	Read_png();
	void	Read_tif();
	void	Read_jpg();

	void	Create_Raster();

private:
	auge::GConnection	*m_pConnection;
};

#endif //__AUGE_RASTER_TEST__H__
