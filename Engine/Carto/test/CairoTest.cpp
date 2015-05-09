#include "CairoTest.h"

//CPPUNIT_TEST_SUITE_REGISTRATION(CairoTest);

#ifndef PI
#define PI					3.14159265358979323846
#endif

void CairoTest::setUp() 
{
	m_width = 256;
	m_height = 256;

	printf("setUp\n");
	//m_cairo_surface = cairo_image_surface_create(CAIRO_FORMAT_ARGB32, m_width, m_height);
	const char* png_path = "C:\\Users\\renyc\\Pictures\\tile.png";
	m_cairo_surface = cairo_image_surface_create_from_png(png_path);
	m_cairo = cairo_create(m_cairo_surface);

	//cairo_set_source_rgba(m_cairo, 1, 0, 0, 0.5);
	//cairo_arc(m_cairo, m_width/2,m_height/2, 20, 0, 2*PI);
	//cairo_fill(m_cairo);
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

void CairoTest::ReadPNG()
{
	int              w, h, s;
	cairo_surface_t *image;

	const char* png_path = "C:\\Users\\renyc\\Pictures\\tile.png";

	image = cairo_image_surface_create_from_png (png_path);
	w = cairo_image_surface_get_width (image);
	h = cairo_image_surface_get_height (image);
	s = cairo_image_surface_get_stride(image);
	unsigned char* data = cairo_image_surface_get_data(image);
	cairo_surface_destroy (image);
}

/*
cairo_image_surface_create_for_data (	unsigned char	    *data,
										cairo_format_t		format,
										int			width,
										int			height,
										int			stride);
										*/
void CairoTest::DrawData_ARGB32()
{	
	unsigned char* data = NULL;
	data = new unsigned char[m_width*m_height*4];
	memset(data, 0, m_width*m_height*4);
	int stride = m_width*4;

	unsigned char* ptr = data;
	for(int i=0; i<m_width; i++)
	{
		int offset = i*stride + i*4;
		ptr = data + offset;
		*(ptr) = 0;			//b
		*(ptr+1) = 0;		//g
		*(ptr+2) = 255;		//r
		*(ptr+3) = 255;		//a
	}

	cairo_surface_t *surface = NULL;	
	surface = cairo_image_surface_create_for_data(data,
												  CAIRO_FORMAT_ARGB32,
												  m_width,
												  m_height,
												  stride);

	cairo_save(m_cairo);
	cairo_translate(m_cairo, 0, 0);
	cairo_scale  (m_cairo, 1, 1);
	cairo_set_source_surface(m_cairo, surface, 0,0);
	cairo_paint(m_cairo);
	cairo_surface_flush(m_cairo_surface);
	cairo_surface_destroy(surface);
	cairo_restore(m_cairo);
}

void CairoTest::DrawData_RGB24()
{	
	int nband = 4;
	unsigned char* data = NULL;
	data = new unsigned char[m_width*m_height*nband];
	memset(data, 255, m_width*m_height*nband);
	int stride = m_width*nband;
	
	unsigned char* ptr = data;
	for(int i=0; i<m_width; i++)
	{
		int offset = i*stride + i*nband;
		ptr = data + offset;
		*(ptr) = 128;		//b
		*(ptr+1) = 255;		//g
		*(ptr+2) = 255;		//r
		*(ptr+3) = 255;		//r
	}

	cairo_surface_t *surface = NULL;	
	surface = cairo_image_surface_create_for_data(data,
		CAIRO_FORMAT_RGB24,
		m_width,
		m_height,
		stride);

	cairo_save(m_cairo);
	cairo_translate(m_cairo, 0, 0);
	cairo_scale  (m_cairo, 1, 1);
	cairo_set_source_surface(m_cairo, surface, 0,0);
	cairo_paint(m_cairo);
	cairo_surface_flush(m_cairo_surface);
	cairo_surface_destroy(surface);
	cairo_restore(m_cairo);
}