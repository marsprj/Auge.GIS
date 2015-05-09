#ifndef __AUGE_RASTER_TEST__H__
#define __AUGE_RASTER_TEST__H__

#include "cppunit/extensions/HelperMacros.h" 
#include "AugeRaster.h"

class RsTest : public CppUnit::TestFixture 
{
	CPPUNIT_TEST_SUITE(RsTest);
//	CPPUNIT_TEST(ReadTest);
//	CPPUNIT_TEST(GetDataSet);
//	CPPUNIT_TEST(DrawRaster);
	CPPUNIT_TEST(DrawRaster_jpg);
//	CPPUNIT_TEST(CairoReadTest);
	CPPUNIT_TEST_SUITE_END();
public:
	void setUp();
	void tearDown();

private:
	void ReadTest();
	void CairoReadTest();
	void GetDataSet();

	void DrawRaster();
	void DrawRaster_jpg();

private:
	void CopyMatrix(unsigned char* src, unsigned char* obj, int width, int height, int stride);
	void SetMatrix(unsigned char* obj, int width, int height, int stride, unsigned char value);

private:
	auge::RasterWorkspace* m_pWorkspace;
};

#endif //__AUGE_RASTER_TEST__H__
