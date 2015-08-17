#ifndef __AUGE_CAIRO_TEST__H__
#define __AUGE_CAIRO_TEST__H__

#include "cppunit/extensions/HelperMacros.h" 

#ifdef _WIN32
#	include "cairo/cairo-win32.h"
#else
#	include "cairo/cairo.h"
#endif

class CairoTest : public CppUnit::TestFixture 
{
	CPPUNIT_TEST_SUITE(CairoTest);
	//CPPUNIT_TEST(DrawPNG);
	//CPPUNIT_TEST(ReadPNG);
	//CPPUNIT_TEST(TranslateImage);
	//CPPUNIT_TEST(DrawData_ARGB32);
	//CPPUNIT_TEST(DrawData_RGB24);
	//CPPUNIT_TEST(DrawRailway);
	//CPPUNIT_TEST(DrawRailway_2);

	//CPPUNIT_TEST(DrawMatrix);
	CPPUNIT_TEST(ClipImage);
	CPPUNIT_TEST_SUITE_END();
public:
	void setUp();
	void tearDown();

private:
	void DrawPNG();
	void ReadPNG();
	void TranslateImage();

	void DrawData_ARGB32();
	void DrawData_RGB24();

	void DrawRailway();
	void DrawRailway_2();

	void DrawMatrix();
	void ClipImage();
private:
	cairo_surface_t	*m_cairo_surface;
	cairo_t			*m_cairo;
	int				m_width;
	int				m_height;
};

#endif //__AUGE_CAIRO_TEST__H__