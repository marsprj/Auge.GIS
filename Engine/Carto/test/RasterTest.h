#ifndef __AUGE_RASTER_TEST__H__
#define __AUGE_RASTER_TEST__H__

#include "cppunit/extensions/HelperMacros.h" 

#include "AugeData.h"
#include "AugeStyle.h"
#include "AugeCarto.h"
#include "AugeFeature.h"
#include "AugeRaster.h"

class RasterTest : public CppUnit::TestFixture 
{
	CPPUNIT_TEST_SUITE(RasterTest);
	CPPUNIT_TEST(DrawMap);
	CPPUNIT_TEST_SUITE_END();
public:
	void setUp();
	void tearDown();

private:
	void DrawMap();

private:
	auge::RasterLayer* CreateRasterLayer();
	auge::FeatureLayer* CreateFeatureLayer();

private:
	auge::GConnection	*m_pConnection;
};

#endif //__AUGE_RASTER_TEST__H__