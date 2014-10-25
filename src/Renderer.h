#ifndef __AUGE_RENDERER_H__
#define __AUGE_RENDERER_H__

#include "Base.h"
#include "Geometry.h"

#ifdef WIN32
#	include "cairo/cairo-win32.h"
#else
#	include "cairo/cairo.h"
#endif

namespace auge
{
	class Map;	
	class Layer;
	class FeatureLayer;
	class Transformation;
	class Style;
	class Rule;
	class Symbolizer;
	class PointSymbolizer;
	class LineSymbolizer;
	class PolygonSymbolizer;
	class TextSymbolizer;

	class Feature;
	class FeatureSet;
	
	class AUGE_API Renderer : public Base
	{
	public:
		Renderer();
		virtual ~Renderer();

	public:
		bool	Create(uint width, uint height);

		void	DrawMap(Map* pMap, Transformation* pTransformation);
		void	DrawLayer(FeatureLayer* pLayer, Transformation* pTransformation);
		
		bool	Save(const char* path);

	private:
		void	DrawLayer(FeatureLayer* pLayer, Rule *pRule, Transformation* pTransformation);
		void	DrawFeatures(FeatureSet* pFeatureSet, Symbolizer* pSymbolizer, Transformation* pTransformation);

		void	DrawGeomtry(uchar* wkb, Symbolizer* pSymbolizer, Transformation* pTransformation);
		void	DrawPoint(	WKBPoint	  *pWKBPoint,		PointSymbolizer   *pSymbolizer, Transformation* pTransformation);
		void	DrawLine(	WKBLineString *pWKBLineString,	LineSymbolizer    *pSymbolizer, Transformation* pTransformation);
		void	DrawPolygon(WKBPolygon	  *pWKBPolygon,		PolygonSymbolizer *pSymbolizer, Transformation* pTransformation);
		void	DrawMultiPoint(	 WKBMultiPoint		*pWKBMultiPoint,		PointSymbolizer   *pSymbolizer, Transformation* pTransformation);
		void	DrawMultiLine(	 WKBMultiLineString *pWKBMultiLineString,	LineSymbolizer    *pSymbolizer, Transformation* pTransformation);
		void	DrawMultiPolygon(WKBMultiPolygon	*pWKBMultiPolygon,		PolygonSymbolizer *pSymbolizer, Transformation* pTransformation);

		void	LabelFeatures(FeatureSet* pFeatureSet, TextSymbolizer* pSymbolizer, Transformation* pTransformation);
		void	LabelGeomtry(uchar* wkb, char* text, TextSymbolizer* pSymbolizer, Transformation* pTransformation);
		void	LabelPoint(	WKBPoint	  *pWKBPoint,		char* text, TextSymbolizer	*pSymbolizer, Transformation* pTransformation);
		void	LabelLine(	WKBLineString *pWKBLineString,	char* text, TextSymbolizer	*pSymbolizer, Transformation* pTransformation);
		void	LabelPolygon(WKBPolygon	  *pWKBPolygon,		char* text, TextSymbolizer	*pSymbolizer, Transformation* pTransformation);
		void	LabelMultiPoint( WKBMultiPoint		*pWKBMultiPoint,	char* text, TextSymbolizer	*pSymbolizer, Transformation* pTransformation);
		void	LabelMultiLine(	 WKBMultiLineString *pWKBMultiLineString,	char* text, TextSymbolizer	*pSymbolizer, Transformation* pTransformation);
		void	LabelMultiPolygon(WKBMultiPolygon	*pWKBMultiPolygon,		char* text, TextSymbolizer	*pSymbolizer, Transformation* pTransformation);
		void	LabelText(double x, double y, char* text, TextSymbolizer	*pSymbolizer);

	private:
		void	Save();
		void	Restore();
		void	SetSymbolizer(Symbolizer* pSymbolizer);

		void	Cleanup();

	private:
		cairo_surface_t	*m_surface;
		cairo_t			*m_cairo;
	};

}//namespace

#endif //__AUGE_RENDERER_H__