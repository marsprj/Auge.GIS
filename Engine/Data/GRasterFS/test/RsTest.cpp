#include "RsTest.h"
#include "AugeCore.h"
#include "AugeRaster.h"
#include "AugeData.h"
#include <iostream>

#ifdef _WIN32
#	include "cairo/cairo-win32.h"
#else
#	include "cairo/cairo.h"
#endif

CPPUNIT_TEST_SUITE_REGISTRATION(RsTest);

void RsTest::setUp() 
{
	//const char* path = "DATABASE=G:\\Data\\GF-1\\tianjin";
	//const char* path = "DATABASE=E:\\Project\\Project\\GA\\Data\\nodata";
	const char* path = "DATABASE=G:\\temp\\td";

	auge::GLogger	*pLogger = auge::augeGetLoggerInstance();
	pLogger->Initialize();

	RESULTCODE rc = AG_FAILURE;
	auge::DataEngine* pDataEngine = NULL;
	pDataEngine = auge::augeGetDataEngineInstance();
	
	m_pWorkspace = dynamic_cast<auge::RasterWorkspace*>(pDataEngine->CreateWorkspace());
	m_pWorkspace->SetConnectionString(path);

	rc = m_pWorkspace->Open();
	CPPUNIT_ASSERT(rc==AG_SUCCESS);

	printf("setUp\n");
}

void RsTest::tearDown()
{
	m_pWorkspace->Close();	
	AUGE_SAFE_RELEASE(m_pWorkspace);
	printf("tearDown\n");
}

void RsTest::ReadTest()
{
	
}

void RsTest::GetDataSet()
{
	//const char* name = "tj.jpg";
	//const char* name = "9634_39954_16.png";
	const char* name = "16km.png";
	
	//auge::Raster* pRaster = m_pWorkspace->OpenRaster(name);
	//auge::GEnvelope& extent = pRaster->GetExtent();

	//std::cout<<"[Name]:"<<pRaster->GetName()<<std::endl;
	//std::cout<<"[Width]:"<<pRaster->GetWidth()<<std::endl;
	//std::cout<<"[Height]:"<<pRaster->GetHeight()<<std::endl;
	//std::cout<<"[Bands]:"<<pRaster->GetBandCount()<<std::endl;
	//std::cout<<"[Extent]:"<<extent.m_xmin<<","<<extent.m_ymin<<","<<extent.m_xmax<<","<<extent.m_ymax<<std::endl;
}

/*
 * CAIRO_FORMAT_ARGB32:	b->g->r->a
 * CAIRO_FORMAT_RGB24:	b->g->r
 */

void RsTest::DrawRaster()
{
	////int m_width = 256;
	////int m_height = 256;
	//int img_width = 800;
	//int img_height = 600;
	//cairo_surface_t	*m_cairo_surface;
	//cairo_t			*m_cairo;
	// 
	//printf("setUp\n");
	//
	////////////////////////////////////////////////////////////////////////////

	////const char* name = "tj.jpg";
	////const char* name = "9634_39954.png";
	////const char* name = "16km.png";
	//const char* name = "Desert.png";

	//auge::Raster* pRaster = m_pWorkspace->OpenRaster(name);
	//auge::GEnvelope& extent = pRaster->GetExtent();

	//int d = pRaster->GetPixelSize();
	//int w = pRaster->GetWidth();
	//int h = pRaster->GetHeight();
	//int nband = pRaster->GetBandCount();

	//size_t size = d*w*h*4;
	//unsigned char* data = (unsigned char*)calloc(size, sizeof(unsigned char));
	//
	//auge::RasterBand* pBand = NULL;
	//// red
	//pBand = pRaster->GetBand(0);
	//unsigned char* pb = (unsigned char*)pBand->GetData();
	//CopyMatrix(pb, data+2, w, h, 4);
	//// green
	//pBand = pRaster->GetBand(1);
	//pb = (unsigned char*)pBand->GetData();
	//CopyMatrix(pb, data+1, w, h, 4);
	//// blue
	//pBand = pRaster->GetBand(2);
	//pb = (unsigned char*)pBand->GetData();
	//CopyMatrix(pb, data+0, w, h, 4);
	//if(nband>3)
	//{
	//	// alpha
	//	pBand = pRaster->GetBand(3);
	//	pb = (unsigned char*)pBand->GetData();
	//	CopyMatrix(pb, data+3, w, h, 4);
	//}
	//else
	//{
	//	CopyMatrix(pb, data+3, w, h, 4);
	//}

	//cairo_format_t cairo_format = CAIRO_FORMAT_ARGB32;
	//if(nband>3)
	//{
	//	cairo_format = CAIRO_FORMAT_ARGB32;
	//}
	//else
	//{
	//	cairo_format = CAIRO_FORMAT_RGB24; 
	//}

	//m_cairo_surface = cairo_image_surface_create(cairo_format, img_width, img_height);
	////const char* png_path = "C:\\Users\\renyc\\Pictures\\tile.png";
	////m_cairo_surface = cairo_image_surface_create_from_png(png_path);
	//m_cairo = cairo_create(m_cairo_surface);

	//cairo_surface_t *surface = NULL;	
	//surface = cairo_image_surface_create_for_data(data,
	//	cairo_format,
	//	w,
	//	h,
	//	d*4*w);

	//double scale_x = (double)img_width/w;
	//double scale_y = (double)img_height/h;

	//unsigned char* pd = cairo_image_surface_get_data(surface);
	//unsigned char r = pd[0];
	//unsigned char g = pd[1];
	//unsigned char b = pd[2];

	//cairo_save(m_cairo);
	//cairo_translate(m_cairo, 0, 0);
	//cairo_scale  (m_cairo, scale_x, scale_y);
	//cairo_set_source_surface(m_cairo, surface, 0,0);
	//cairo_paint(m_cairo);
	//cairo_surface_flush(m_cairo_surface);
	//cairo_surface_destroy(surface);
	//cairo_restore(m_cairo);

	////unsigned char* pd = cairo_image_surface_get_data(m_cairo_surface);
	////unsigned char r = pd[0];
	////unsigned char g = pd[1];
	////unsigned char b = pd[2];


	//pRaster->Release();

	////////////////////////////////////////////////////////////////////////////

	//cairo_surface_write_to_png(m_cairo_surface, "G:\\temp\\map\\cairo.png");
	//if(m_cairo!=NULL)
	//{
	//	cairo_destroy(m_cairo);
	//	cairo_surface_destroy(m_cairo_surface);

	//	m_cairo = NULL;
	//	m_cairo_surface = NULL;
	//}

	//free(data);
}

void RsTest::DrawRaster_jpg()
{
	////int m_width = 256;
	////int m_height = 256;
	//int img_width = 800;
	//int img_height = 600;
	//cairo_surface_t	*m_cairo_surface;
	//cairo_t			*m_cairo;

	//printf("setUp\n");

	////////////////////////////////////////////////////////////////////////////

	////const char* name = "tj.jpg";
	////const char* name = "9634_39954.png";
	////const char* name = "16km.png";
	//const char* name = "Desert.jpg";

	//auge::Raster* pRaster = m_pWorkspace->OpenRaster(name);
	//auge::GEnvelope& extent = pRaster->GetExtent();

	//int d = pRaster->GetPixelSize();
	//int w = pRaster->GetWidth();
	//int h = pRaster->GetHeight();
	//int nband = pRaster->GetBandCount();

	//size_t size = d*w*h*4;
	//unsigned char* data = (unsigned char*)calloc(size, sizeof(unsigned char));

	//auge::RasterBand* pBand = NULL;
	//// red
	//pBand = pRaster->GetBand(0);
	//unsigned char* pb = (unsigned char*)pBand->GetData();
	//CopyMatrix(pb, data+2, w, h, 4);
	//// green
	//pBand = pRaster->GetBand(1);
	//pb = (unsigned char*)pBand->GetData();
	//CopyMatrix(pb, data+1, w, h, 4);
	//// blue
	//pBand = pRaster->GetBand(2);
	//pb = (unsigned char*)pBand->GetData();
	//CopyMatrix(pb, data+0, w, h, 4);
	//if(nband>3)
	//{
	//	// alpha
	//	pBand = pRaster->GetBand(3);
	//	pb = (unsigned char*)pBand->GetData();
	//	CopyMatrix(pb, data+3, w, h, 4);
	//}
	//else
	//{
	//	CopyMatrix(pb, data+3, w, h, 4);
	//}

	//cairo_format_t cairo_format = CAIRO_FORMAT_ARGB32;
	//if(nband>3)
	//{
	//	cairo_format = CAIRO_FORMAT_ARGB32;
	//}
	//else
	//{
	//	cairo_format = CAIRO_FORMAT_RGB24; 
	//}

	//m_cairo_surface = cairo_image_surface_create(cairo_format, img_width, img_height);
	////const char* png_path = "C:\\Users\\renyc\\Pictures\\tile.png";
	////m_cairo_surface = cairo_image_surface_create_from_png(png_path);
	//m_cairo = cairo_create(m_cairo_surface);

	//cairo_surface_t *surface = NULL;	
	//surface = cairo_image_surface_create_for_data(data,
	//	cairo_format,
	//	w,
	//	h,
	//	d*4*w);

	//double scale_x = (double)img_width/w;
	//double scale_y = (double)img_height/h;

	//unsigned char* pd = cairo_image_surface_get_data(surface);
	//unsigned char r = pd[0];
	//unsigned char g = pd[1];
	//unsigned char b = pd[2];

	//cairo_save(m_cairo);
	//cairo_translate(m_cairo, 0, 0);
	//cairo_scale  (m_cairo, scale_x, scale_y);
	//cairo_set_source_surface(m_cairo, surface, 0,0);
	//cairo_paint(m_cairo);
	//cairo_surface_flush(m_cairo_surface);
	//cairo_surface_destroy(surface);
	//cairo_restore(m_cairo);

	////unsigned char* pd = cairo_image_surface_get_data(m_cairo_surface);
	////unsigned char r = pd[0];
	////unsigned char g = pd[1];
	////unsigned char b = pd[2];


	//pRaster->Release();

	////////////////////////////////////////////////////////////////////////////

	//cairo_surface_write_to_png(m_cairo_surface, "G:\\temp\\map\\cairo.png");
	//if(m_cairo!=NULL)
	//{
	//	cairo_destroy(m_cairo);
	//	cairo_surface_destroy(m_cairo_surface);

	//	m_cairo = NULL;
	//	m_cairo_surface = NULL;
	//}

	//free(data);
}

void RsTest::CopyMatrix(unsigned char* src, unsigned char* obj, int width, int height, int stride)
{
	unsigned char* sp = src;
	unsigned char* op = obj;
	for(int i=0; i<height; i++)
	{
		for(int j=0; j<width; j++,sp++,op+=stride)
		{
			*op = *sp;
		}
	}
}

void RsTest::SetMatrix(unsigned char* obj, int width, int height, int stride, unsigned char value)
{
	unsigned char* op = obj;
	for(int i=0; i<height; i++)
	{
		for(int j=0; j<width; j++,op+=stride)
		{
			*op = value;
		}
	}
}

void RsTest::CairoReadTest()
{
	const char* png_path = "G:\\temp\\td\\Desert.png";
	cairo_surface_t* cairo_surface = cairo_image_surface_create_from_png(png_path);

	cairo_format_t format = cairo_image_surface_get_format(cairo_surface);
	unsigned char* data = cairo_image_surface_get_data(cairo_surface);
	unsigned char r = data[0];
	unsigned char g = data[1];
	unsigned char b = data[2];
	r = data[3];
	g = data[4];
	b = data[5];


	cairo_surface_destroy(cairo_surface);
}

void RsTest::Read_png()
{
	const char* png_path = "G:\\temp\\td\\Desert.png";
	auge::RasterIO* pRasterIO = auge::augeGetRasterIOInstance();
	auge::Raster* pRaster = pRasterIO->Read(png_path);

	pRaster->Release();
}