#ifndef __AUGE_STYLE_READER_1_0_0_H__
#define __AUGE_STYLE_READER_1_0_0_H__

#include "AugeStyle.h"
#include "AugeXML.h"

namespace auge
{
	class StyleReader_1_0_0
	{
	public:
		StyleReader_1_0_0();
		virtual ~StyleReader_1_0_0();
	public:
		Style*				Read(XElement* pxRootElement);

	private:
		Style*				ReadUserLayer(XNode* pxUserLayer);
		Style*				ReadNamedLayer(XNode* pxNamedLayer);

		Style*				ReadUserStyle(XNode* pxUserStyle);
		FeatureStyle*		ReadFeatureStyle(XNode* pxFeatureStyle);
		Rule*				ReadRule(XNode* pxRuleNode);

		PointSymbolizer*	ReadPointSymbolizer(XNode* pxSymbolizerNode);
		LineSymbolizer*		ReadLineSymbolizer(XNode* pxSymbolizerNode);
		PolygonSymbolizer*	ReadPolygonSymbolizer(XNode* pxSymbolizerNode);
		TextSymbolizer*		ReadTextSymbolizer(XNode* pxSymbolizerNode);

		//double				ReadSize(XNode* pxSizeNode);
		bool				ReadGraphic(PointSymbolizer* pSymbolizer, XNode* pxGraphicNode);		
		bool				ReadMark(PointSymbolizer* pSymbolizer, XNode* pxMarkNode);

		bool				ReadStroke(LineSymbolizer* pSymbolizer, XNode* pxStrokeNode);

		bool				ReadMark(PolygonSymbolizer* pSymbolizer, XNode* pxMarkNode);
		bool				ReadFill(PolygonSymbolizer* pSymbolizer, XNode* pxFillNode);
		bool				ReadStroke(PolygonSymbolizer* pSymbolizer, XNode* pxStrokeNode);

		bool				ReadLabel(TextSymbolizer* pSymbolizer, XNode* pxLabelNode);
		bool				ReadPlacement(TextSymbolizer* pSymbolizer, XNode* pxPlacementNode);
		bool				ReadFont(TextSymbolizer* pSymbolizer, XNode* pxFontNode);
		bool				ReadFill(TextSymbolizer* pSymbolizer, XNode* pxFillNode);
		bool				ReadStroke(TextSymbolizer* pSymbolizer, XNode* pxStrokeNode);

		bool				ReadGeometry(Symbolizer* pSymbolizer, XNode* pxGeometryNode);

		Stroke*				ReadStroke(XNode* pxStrokeNode);
		Fill*				ReadFill(XNode* pxFillNode);

		//RESULTCODE			ReadLabel(TextSymbolizer *pSymbolizer, XNode *pxNodeLabel);
	};

}

#endif //__AUGE_STYLE_READER_1_0_0_H__

