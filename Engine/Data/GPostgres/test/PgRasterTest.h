#ifndef __AUGE_CANVAS_TEST__H__
#define __AUGE_CANVAS_TEST__H__

#include "cppunit/extensions/HelperMacros.h" 
#include "AugeRaster.h"

class PgRasterTest : public CppUnit::TestFixture 
{
	CPPUNIT_TEST_SUITE(PgRasterTest);
	CPPUNIT_TEST(CreateRasterFolder);
	//CPPUNIT_TEST(EnumRasterFolder);
	CPPUNIT_TEST(AddRaster);
	//CPPUNIT_TEST(GetRasterByPath);
	//CPPUNIT_TEST(ReadRaster);

	CPPUNIT_TEST_SUITE_END();
public:
	void setUp();
	void tearDown();

private:

	void CreateRasterFolder();
	void RemoveRasterFolder();
	void RenameRasterFolder();
	void EnumRasterFolder();

	void AddRaster();
	void GetRasterByPath();

	void ReadRaster();

private:
	auge::RasterWorkspace* m_pWorkspace;
};

#endif //__AUGE_CANVAS_TEST__H__
