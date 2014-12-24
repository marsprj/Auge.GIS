#ifndef __AUGE_STYLE_WRITER_1_0_0_H__
#define __AUGE_STYLE_WRITER_1_0_0_H__

#include "AugeStyle.h"
#include "AugeXML.h"

namespace auge
{
	class StyleWriter_1_0_0
	{
	public:
		StyleWriter_1_0_0();
		virtual ~StyleWriter_1_0_0();
	public:
		XDocument*		Write(Style* pStyle, const char* version="1.0");
	private:
		XDocument*		WriteFeature(FeatureStyle* pStyle);
		XDocument*		WriteRaster(RasterStyle* pStyle);

		RESULTCODE		WriteFeatureStyle(XElement* pxFeature, FeatureStyle* pStyle);
		RESULTCODE		WriteRule(XElement* pxRule, Rule* pRule);
		RESULTCODE		WriteFilter(XElement* pxFilter, GFilter* pFilter);

		RESULTCODE		WriteSymbolizer(XElement* pxRule, Symbolizer* pSymbolizer);
		RESULTCODE		WriteSymbolizer(XElement* pxRule, PointSymbolizer* pSymbolizer);
		RESULTCODE		WriteSymbolizer(XElement* pxRule, LineSymbolizer* pSymbolizer);
		RESULTCODE		WriteSymbolizer(XElement* pxRule, PolygonSymbolizer* pSymbolizer);
		RESULTCODE		WriteSymbolizer(XElement* pxRule, TextSymbolizer* pSymbolizer);

		RESULTCODE		WriteFill(XElement* pxFill, Fill* pFill);
		RESULTCODE		WriteStroke(XElement* pxStroke, Stroke* pStroke);

	};

}

#endif //__AUGE_STYLE_WRITER_1_0_0_H__

