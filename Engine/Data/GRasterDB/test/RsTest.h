#ifndef __AUGE_RASTER_TEST__H__
#define __AUGE_RASTER_TEST__H__

#include "cppunit/extensions/HelperMacros.h" 
#include "AugeRaster.h"

class RsTest : public CppUnit::TestFixture 
{
	CPPUNIT_TEST_SUITE(RsTest);
	//CPPUNIT_TEST(OpenRaster);
	//CPPUNIT_TEST(OpenRasterDataset);
	//CPPUNIT_TEST(AddRasterTest);
	CPPUNIT_TEST(GetDatasets);
	CPPUNIT_TEST_SUITE_END();
public:
	void setUp();
	void tearDown();

private:
	void OpenRaster();
	void OpenRasterDataset();

	void AddRasterTest();
	void GetDatasets();

private:
	auge::RasterWorkspace	*m_pWorkspace;
};

#endif //__AUGE_RASTER_TEST__H__
