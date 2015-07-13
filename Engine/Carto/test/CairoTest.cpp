#include "CairoTest.h"

CPPUNIT_TEST_SUITE_REGISTRATION(CairoTest);

#ifndef PI
#define PI					3.14159265358979323846
#endif

void draw_line(cairo_t *m_cairo, int* points_x, int* points_y, int count);
void draw_railway(cairo_t *m_cairo, int* points_x, int* points_y, int count);
void draw_railway_black_block(cairo_t *m_cairo, int* pts_x, int* pts_y, int pts_count);
void draw_dash_line(cairo_t *m_cairo, int* points_x, int* points_y, int count);

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

void CairoTest::DrawRailway()
{
	int points_x[] = {100,400,700};
	int points_y[] = {100,400,100};

	cairo_surface_t	*m_cairo_surface;
	cairo_t			*m_cairo;
	m_cairo_surface = cairo_image_surface_create(CAIRO_FORMAT_ARGB32, 800, 800);
	m_cairo = cairo_create(m_cairo_surface);

	int count = sizeof(points_x) / sizeof(int);
	//draw_railway(m_cairo, points_x, points_y, count);
	draw_line(m_cairo, points_x, points_y, count);
	draw_dash_line(m_cairo, points_x, points_y, count);

	cairo_surface_write_to_png(m_cairo_surface, "g:\\temp\\map\\example.png");

	if(m_cairo!=NULL)
	{
		cairo_destroy(m_cairo);
		cairo_surface_destroy(m_cairo_surface);
		m_cairo = NULL;
		m_cairo_surface = NULL;
	}
}

void CairoTest::DrawRailway_2()
{
	int block_width = 5;
	int block_length= 10;

	double dashes[] = { 10,  /* ink */
		10,  /* skip */
	};
	int ndash = sizeof(dashes) / sizeof(double);
	double dash_offset = block_length;

	int points_x[] = {100,400,700};
	int points_y[] = {100,400,100};

	cairo_surface_t	*m_cairo_surface;
	cairo_t			*m_cairo;
	m_cairo_surface = cairo_image_surface_create(CAIRO_FORMAT_ARGB32, 800, 800);
	m_cairo = cairo_create(m_cairo_surface);

	cairo_save(m_cairo);
	cairo_new_path(m_cairo);

	int count = sizeof(points_x) / sizeof(int);
	cairo_move_to(m_cairo, points_x[0], points_y[0]);
	for(int i=1; i<count; i++)
	{
		cairo_line_to(m_cairo, points_x[i], points_y[i]);
	}

	// draw black
	cairo_set_line_cap(m_cairo, CAIRO_LINE_CAP_BUTT);
	cairo_set_source_rgba(m_cairo,	0.0f, 0.0f, 0.0f, 1.0f);
	cairo_set_line_width(m_cairo, block_width);
	cairo_stroke(m_cairo);

	// draw white block
	cairo_move_to(m_cairo, points_x[0], points_y[0]);
	for(int i=1; i<count; i++)
	{
		cairo_line_to(m_cairo, points_x[i], points_y[i]);
	}
	cairo_set_source_rgba(m_cairo,	1.0f, 1.0f, 1.0f, 1.0f);
	cairo_set_line_width(m_cairo, block_width-1);
	cairo_set_dash (m_cairo, dashes, ndash, dash_offset);
	cairo_stroke(m_cairo);
	
	// write image file
	cairo_surface_write_to_png(m_cairo_surface, "g:\\temp\\map\\example.png");

	if(m_cairo!=NULL)
	{
		cairo_destroy(m_cairo);
		cairo_surface_destroy(m_cairo_surface);
		m_cairo = NULL;
		m_cairo_surface = NULL;
	}
}

//void CairoTest::DrawRailway()
//{
//	int points_x[] = {100,400,700};
//	int points_y[] = {100,400,100};
//
//	cairo_surface_t	*m_cairo_surface;
//	cairo_t			*m_cairo;
//	m_cairo_surface = cairo_image_surface_create(CAIRO_FORMAT_ARGB32, 800, 800);
//	m_cairo = cairo_create(m_cairo_surface);
//
//	int count = sizeof(points_x) / sizeof(int);
//
//	draw_railway(m_cairo, points_x, points_y, count);
//	draw_line(m_cairo, points_x, points_y, count);
//	
//	cairo_surface_write_to_png(m_cairo_surface, "g:\\temp\\map\\example.png");
//
//	if(m_cairo!=NULL)
//	{
//		cairo_destroy(m_cairo);
//		cairo_surface_destroy(m_cairo_surface);
//		m_cairo = NULL;
//		m_cairo_surface = NULL;
//	}
//}

void draw_railway(cairo_t *m_cairo, int* points_x, int* points_y, int count)
{
	int block_width = 10;
	int block_length= 50;
	cairo_save(m_cairo);
	cairo_new_path(m_cairo);

	//cairo_move_to(m_cairo, points_x[0], points_y[0]);
	//for(int i=1; i<count; i++)
	//{
	//	cairo_line_to(m_cairo, points_x[i], points_y[i]);
	//}

	cairo_set_source_rgba(m_cairo,	0.0f, 0.0f, 0.0f, 1.0f);
	cairo_set_line_width(m_cairo, 30);
	//ÉèÖÃcap
	cairo_set_line_cap(m_cairo, CAIRO_LINE_CAP_BUTT);

	cairo_stroke(m_cairo);

	cairo_restore(m_cairo);
}

void draw_railway_black_block(cairo_t *m_cairo, int* pts_x, int* pts_y, int pts_count)
{

}

void draw_line(cairo_t *m_cairo, int* points_x, int* points_y, int count)
{
	cairo_save(m_cairo);
	cairo_new_path(m_cairo);

	cairo_move_to(m_cairo, points_x[0], points_y[0]);
	for(int i=1; i<count; i++)
	{
		cairo_line_to(m_cairo, points_x[i], points_y[i]);
	}
	//cairo_move_to(m_cairo, 100, 100);
	//cairo_line_to(m_cairo, 400, 400);
	//cairo_line_to(m_cairo, 700, 100);

	cairo_set_source_rgba(m_cairo,	0.0f, 0.0f, 0.0f, 1.0f);
	cairo_set_line_width(m_cairo, 20);
	//ÉèÖÃcap
	cairo_set_line_cap(m_cairo, CAIRO_LINE_CAP_BUTT);

	cairo_stroke(m_cairo);

	cairo_restore(m_cairo);
}

void draw_dash_line(cairo_t *m_cairo, int* points_x, int* points_y, int count)
{
	int block_width = 20-2;
	int block_length= 50;

	double dashes[] = { 50.0,  /* ink */
						50.0,  /* skip */
	};
	int ndash = sizeof(dashes) / sizeof(double);
	double offset = 50.0f;

	cairo_save(m_cairo);
	cairo_new_path(m_cairo);

	cairo_move_to(m_cairo, points_x[0], points_y[0]);
	for(int i=1; i<count; i++)
	{
		cairo_line_to(m_cairo, points_x[i], points_y[i]);
	}
	//cairo_move_to(m_cairo, 100, 100);
	//cairo_line_to(m_cairo, 400, 400);
	//cairo_line_to(m_cairo, 700, 100);

	cairo_set_source_rgba(m_cairo,	1.0f, 1.0f, 1.0f, 1.0f);
	cairo_set_line_width(m_cairo, block_width);
	cairo_set_dash (m_cairo, dashes, ndash, offset);
	//ÉèÖÃcap
	cairo_set_line_cap(m_cairo, CAIRO_LINE_CAP_BUTT);

	cairo_stroke(m_cairo);

	cairo_restore(m_cairo);
}