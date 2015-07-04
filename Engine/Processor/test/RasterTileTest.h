#ifndef __AUGE_RASTER_TILE_TEST__H__
#define __AUGE_RASTER_TILE_TEST__H__

#include "cppunit/extensions/HelperMacros.h" 
#include "AugeTile.h"

class RasterTileTest : public CppUnit::TestFixture 
{
	CPPUNIT_TEST_SUITE(RasterTileTest);
	CPPUNIT_TEST(BuildRasterPyramid);
	CPPUNIT_TEST_SUITE_END();
public:
	void setUp();
	void tearDown();

private:

	void	BuildRasterPyramid();

private:
	auge::TileWorkspace* m_pWorkspace;
	auge::GConnection	*m_pConnection;
};

#endif //__AUGE_RASTER_TILE_TEST__H__
