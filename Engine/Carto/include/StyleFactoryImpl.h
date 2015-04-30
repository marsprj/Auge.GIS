#ifndef __AUGE_STYLE_FACTORY_IMPL_H__
#define __AUGE_STYLE_FACTORY_IMPL_H__

#include "AugeStyle.h"

namespace auge
{
	class StyleFactoryImpl : public StyleFactory
	{
	public:
		StyleFactoryImpl();
		virtual ~StyleFactoryImpl();
	public:
		virtual	FeatureStyle*		CreateFeatureStyle();
		virtual FeatureStyle*		CreateFeatureStyle(augeSymbolizerType type);
		virtual FeatureStyle*		CreateFeatureStyle(augeGeometryType type);
		virtual RasterStyle*		CreateRasterStyle();

		virtual Rule*				CreateRule();
		virtual PointSymbolizer*	CreatePointSymbolizer();
		virtual LineSymbolizer*		CreateLineSymbolizer();
		virtual PolygonSymbolizer*	CreatePolygonSymbolizer();
		virtual TextSymbolizer*		CreateTextSymbolizer();

		virtual Stroke*				CreateStroke();
		virtual Fill*				CreateFill();
		virtual Mark*				CreateMark();
		virtual Font*				CreateFont();
		virtual ColorMap*			CreateColorMap(g_uint count, GColor& start, GColor& end);

		virtual StyleReader*		CreateStyleReader();
		virtual StyleWriter*		CreateStyleWriter();
	};
}

#endif //__AUGE_STYLE_FACTORY_IMPL_H__
