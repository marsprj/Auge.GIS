#include "CairoTest.h"

//CPPUNIT_TEST_SUITE_REGISTRATION(CairoTest);

#ifndef PI
#define PI					3.14159265358979323846
#endif

void CairoTest::setUp() 
{
	m_width = 800;
	m_height = 600;

	printf("setUp\n");
	m_cairo_surface = cairo_image_surface_create(CAIRO_FORMAT_ARGB32, m_width, m_height);
	m_cairo = cairo_create(m_cairo_surface);

	cairo_set_source_rgba(m_cairo, 1, 0, 0, 0.5);
	cairo_arc(m_cairo, m_width/2,m_height/2, 20, 0, 2*PI);
	cairo_fill(m_cairo);
}

void CairoTest::tearDown()
{
	printf("tearDown\n");

	cairo_surface_write_to_png(m_cairo_surface, "G:\\temp\\map\\cairo.png");
	if(m_cairo!=NULL)
	{
		cairo_destroy(m_cairo);
		cairo_surface_destroy(m_cairo_surface);

		m_cairo = NULL;
		m_cairo_surface = NULL;
	}
}

void CairoTest::DrawPNG()
{
	const char* png_path = "E:\\Research\\Auge.GIS\\Engine\\Carto\\sld\\graphic\\03.png";

	cairo_surface_t *image = cairo_image_surface_create_from_png(png_path);
	int w = cairo_image_surface_get_width(image);
	int h = cairo_image_surface_get_height(image);

	float size = 16.0f;

	//cairo_save(m_cairo);
	cairo_scale  (m_cairo, size/w, size/h);

	cairo_set_source_surface(m_cairo, image, 700, 500);
	
	cairo_paint(m_cairo);
	//cairo_surface_flush(m_cairo_surface);
	cairo_surface_destroy(image);

	//cairo_restore(m_cairo);
}

void CairoTest::TranslateImage()
{
	int              w, h;
	cairo_surface_t *image;

	const char* png_path = "C:\\Users\\renyc\\Pictures\\tile.png";

	image = cairo_image_surface_create_from_png (png_path);
	w = cairo_image_surface_get_width (image);
	h = cairo_image_surface_get_height (image);

	cairo_save(m_cairo);
	cairo_translate (m_cairo, m_width/2, m_height/2);
	//cairo_rotate (m_cairo, 45* PI/180);
	cairo_scale  (m_cairo, 128.0/w, 128.0/h);
	//cairo_translate (m_cairo, -0.5*w, -0.5*h);

	//cairo_set_source_surface (m_cairo, image, m_width/2, m_height/2);
	cairo_set_source_surface (m_cairo, image, 0,0);
	cairo_paint (m_cairo);
	cairo_surface_destroy (image);

	cairo_restore(m_cairo);
}