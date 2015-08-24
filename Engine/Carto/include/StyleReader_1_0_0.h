#ifndef __AUGE_STYLE_READER_1_0_0_H__
#define __AUGE_STYLE_READER_1_0_0_H__

#include "AugeStyle.h"
#include "AugeXML.h"

namespace auge
{
	class FeatureClass;

	class StyleReader_1_0_0
	{
	public:
		StyleReader_1_0_0();
		virtual ~StyleReader_1_0_0();
	public:
		Style*				Read(XElement* pxRootElement, FeatureClass* pFeatureClass);

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
		bool				ReadMarker(PointSymbolizer* pSymbolizer, XNode* pxMarkNode);
		bool				ReadExternalGraphic(PointSymbolizer* pSymbolizer, XNode* pxGraphicNode);

		//bool				ReadStroke(LineSymbolizer* pSymbolizer, XNode* pxStrokeNode);

		bool				ReadMarker(PolygonSymbolizer* pSymbolizer, XNode* pxMarkNode);
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
		ExternalGraphic*	ReadExternalGraphic(XNode*  pxGraphicNode);

		//RESULTCODE			ReadLabel(TextSymbolizer *pSymbolizer, XNode *pxNodeLabel);

	private:
		FeatureClass* m_pFeatureClass;
	};

}

#endif //__AUGE_STYLE_READER_1_0_0_H__

