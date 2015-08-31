#ifndef __AUGE_RENDERER_CAIRO_H__
#define __AUGE_RENDERER_CAIRO_H__

#include "AugeCarto.h"

#ifdef WIN32
#	include "cairo/cairo-win32.h"
#else
#	include "cairo/cairo.h"
#endif

namespace auge
{
	class RendererCairo : public Renderer
	{
	public:
		RendererCairo(g_uint width, g_uint height);
		virtual ~RendererCairo();
	public:
		virtual void		Draw(g_uchar* wkb, Symbolizer* pSymbolizer, Transformation* pTransformation);
		virtual void		DrawPoint(WKBPoint			*pWKBPoint,				PointSymbolizer* pSymbolizer, Transformation* pTransformation);
		virtual void		DrawPoint(WKBMultiPoint		*pWKBMultiPoint,		PointSymbolizer* pSymbolizer, Transformation* pTransformation);
		virtual void		DrawLine(WKBLineString		*pWKBLineString,		Stroke* pStroke, Transformation* pTransformation);
		virtual void		DrawLine(WKBMultiLineString	*pWKBMultiLineString,	Stroke* pStroke, Transformation* pTransformation);
		virtual void		DrawLine(WKBLineString		*pWKBLineString,		LineSymbolizer* pSymbolizer, Transformation* pTransformation);
		virtual void		DrawLine(WKBMultiLineString	*pWKBMultiLineString,	LineSymbolizer* pSymbolizer, Transformation* pTransformation);
		virtual void		DrawPolygon(WKBPolygon		*pWKBPolygon,			PolygonSymbolizer* pPolygonSymbolizer, Transformation* pTransformation);
		virtual void		DrawPolygon(WKBMultiPolygon	*pWKBMultiPolygon,		PolygonSymbolizer* pPolygonSymbolizer, Transformation* pTransformation);

		virtual void		DrawText(const	  char* text, int x, int y, TextSymbolizer* pSymbolizer);
		virtual void		DrawText(const g_uchar* text, int x, int y, TextSymbolizer* pSymbolizer);
		virtual void		Label(const char* text, WKBPoint *pWKBPoint, TextSymbolizer* pSymbolizer, Transformation* pTransformation);

		virtual bool		DrawColorMap(ColorMap* pColorMap);

		virtual bool		DrawRaster(Raster* pRaster, Transformation* pTransformation);

		virtual void		Save();
		virtual void		Resotre();

	public:
		virtual void		FillRectangle(g_uint width, g_uint height, GColor& color);
		virtual	void		DrawRectangle(g_uint width, g_uint height, GColor& color, double border);
		virtual void		FillRectangle(g_uint x, g_uint y, g_uint width, g_uint height, GColor& color);
		virtual	void		DrawRectangle(g_uint x, g_uint y, g_uint width, g_uint height, GColor& color, double border);

	public:
		virtual void		SetFont(const char* family, 
									float size, 
									augeFontSlant slant=augeFontStyleNormal, 
									augeFontWeight weight=augeFontWeightNormal);
		virtual bool		GetTextExtent(const char* text, GEnvelope& extent);

	public:
		virtual RESULTCODE	SaveAsImage(const char* szPath);
		virtual RESULTCODE	SaveAsImage(g_uint x, g_uint y, g_uint width, g_uint height, const char* path);
		virtual void		Relaese();

	public:
		cairo_t*			GetCairo();
		cairo_surface_t*	GetCairoSurface();

	private:
		void	DrawCircle(int cx, int cy, float radius, Fill* pFill, Stroke* pStroke);
		void	DrawSquare(int cx, int cy, float size, float rotation, Fill* pFill, Stroke* pStroke);
		void	DrawStar(int cx, int cy, float size, float rotation, Fill* pFill, Stroke* pStroke);
		void	DrawsTriangle(int cx, int cy, float size, float rotation, Fill* pFill, Stroke* pStroke);
		void	DrawPentagon(int cx, int cy, float size, float rotation, Fill* pFill, Stroke* pStroke);
		void	DrawCapital(int cx, int cy, float size, Fill* pFill, Stroke* pStroke);

		void	DrawCircle(int cx, int cy, float radius, Stroke* pStroke);
		void	FillCircle(int cx, int cy, float radius, Fill* pFill);

		void	DrawSquare(int cx, int cy, float size, Stroke* pStroke);
		void	FillSquare(int cx, int cy, float size, Fill* pFill);

		void	DrawGeometryPoint(WKBPoint	*pWKBPoint,	PointSymbolizer* pSymbolizer, Transformation* pTransformation);
		void	DrawGraphicPoint(WKBPoint	*pWKBPoint,	PointSymbolizer* pSymbolizer, Transformation* pTransformation);

		bool	ReadRasterSubArea_Byte(Raster* pRaster, unsigned char* pdata, int x, int y, int width, int height);
		bool	ReadRasterSubAreaBand_Byte_1(Raster* pRaster, unsigned char* pdata, int x, int y, int width, int height);
		bool	ReadRasterSubAreaBand_Byte_3(Raster* pRaster, unsigned char* pdata, int x, int y, int width, int height);
		bool	ReadRasterSubAreaBand_Byte_4(Raster* pRaster, unsigned char* pdata, int x, int y, int width, int height);
		void	CopyMatrix_Byte(unsigned char* src, int src_width, int src_height, unsigned char* obj, int obj_width, int obj_height, int obj_step);
		void	SetMatrix(unsigned char* obj, int width, int height, int stride, unsigned char value);

		bool	ReadRasterSubArea_Short(Raster* pRaster, unsigned char* pdata, int x, int y, int width, int height);
		bool	ReadRasterSubAreaBand_Short_1(Raster* pRaster, g_byte* pdata, int x, int y, int width, int height);
		bool	ReadRasterSubAreaBand_Short_3(Raster* pRaster, g_byte* pdata, int x, int y, int width, int height);
		void	CopyMatrix_Short(g_int16* src, int src_width, int src_height, unsigned char* obj, int obj_width, int obj_height, int obj_step);
		

		// DrawLine
		void	DrawRailway(WKBLineString *pWKBLineString, Transformation* pTransformation);
		void	DrawRailway(WKBMultiLineString *pWKBMultiLineString, Transformation* pTransformation);

	private:
		cairo_surface_t	*m_cairo_surface;
		cairo_t			*m_cairo;
		bool			m_image_render;
		g_uint			m_width;
		g_uint			m_height;
	};

	void	set_stroke_style(cairo_t* cairo, Stroke* pStroke);
	void	build_path(cairo_t* cairo, WKBLineString		*pWKBLineString,		Transformation* pTransformation);
	void	build_path(cairo_t* cairo, WKBMultiLineString	*pWKBMultiLineString,	Transformation* pTransformation);
	void	build_path(cairo_t* cairo, WKBPolygon			*pWKBPolygon,			Transformation* pTransformation);
	void	build_path(cairo_t* cairo, WKBMultiPolygon		*pWKBPolygon,			Transformation* pTransformation);
}

#endif //__AUGE_RENDERER_CAIRO_H__
