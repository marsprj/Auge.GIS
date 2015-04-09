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
	CPPUNIT_TEST(TranslateImage);
	CPPUNIT_TEST_SUITE_END();
public:
	void setUp();
	void tearDown();

private:
	void DrawPNG();
	void TranslateImage();

private:
	cairo_surface_t	*m_cairo_surface;
	cairo_t			*m_cairo;
	int				m_width;
	int				m_height;
};

#endif //__AUGE_CAIRO_TEST__H__