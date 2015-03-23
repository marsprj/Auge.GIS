#include "SLDConstant.h"
#include "StyleReader_1_0_0.h"

#include "FeatureStyleImpl.h"
#include "RuleImpl.h"
#include "PointSymbolizerImpl.h"
#include "LineSymbolizerImpl.h"
#include "PolygonSymbolizerImpl.h"
#include "TextSymbolizerImpl.h"
#include "FillImpl.h"
#include "StrokeImpl.h"

namespace auge
{

	StyleReader_1_0_0::StyleReader_1_0_0()
	{

	}

	StyleReader_1_0_0::~StyleReader_1_0_0()
	{

	}

	Style* StyleReader_1_0_0::Read(XElement* pxRoot)
	{
		const char	*nodeName = NULL;
		Style		*pStyle = NULL;
		XNode		*pxNode    = NULL;
		XNodeSet	*pxNodeSet = NULL;

		pxNodeSet = pxRoot->GetChildren();
		if(pxNodeSet==NULL)
		{
			return NULL;
		}

		pxNodeSet->Reset();
		while(!pxNodeSet->IsEOF())
		{
			pxNode = pxNodeSet->Next();
			if(pxNode!=NULL)
			{
				nodeName = pxNode->GetName();
				if(g_stricmp(nodeName, AUGE_SLD_USER_LAYER)==0)
				{
					pStyle = ReadUserLayer(pxNode);
				}
				else if(g_stricmp(nodeName, AUGE_SLD_NAMED_LAYER)==0)
				{
					pStyle = ReadNamedLayer(pxNode);
				}
			}
		}

		pxNodeSet->Release();

		return pStyle;
	}

	Style* StyleReader_1_0_0::ReadUserLayer(XNode* pxUserLayer)
	{
		XNode			*pxNode    = NULL;
		XNodeSet		*pxNodeSet = NULL;
		Style			*pStyle = NULL;
		const char		*nodeName = NULL;
		//UserStyle		*psldUserStyle = NULL;

		pxNodeSet = pxUserLayer->GetChildren(AUGE_SLD_USER_STYLE);
		if(pxNodeSet==NULL)
		{
			return NULL;
		}
		pxNodeSet->Reset();
		if(pxNodeSet->Count()==0)
		{
			return NULL;
		}
		pxNode = pxNodeSet->Item(0);
		nodeName = pxNode->GetName();
		if(g_stricmp(nodeName, AUGE_SLD_USER_STYLE))
		{
			return NULL;
		}

		pStyle = ReadUserStyle(pxNode);
		pxNodeSet->Release();

		return pStyle;
	}
	
	Style* StyleReader_1_0_0::ReadNamedLayer(XNode* pxNamedLayer)
	{
		Style* pStyle = NULL;
		return pStyle;
	}

	Style* StyleReader_1_0_0::ReadUserStyle(XNode* pxUserStyle)
	{
		XNode		*pxNode		= NULL;
		XNodeSet	*pxNodeSet	= NULL;
		const char	*nodeName	= NULL;

		Style		*pStyle = NULL;
		std::string	name, title;

		pxNodeSet = pxUserStyle->GetChildren();
		pxNodeSet->Reset();

		while(!pxNodeSet->IsEOF())
		{
			pxNode = pxNodeSet->Next();
			nodeName = pxNode->GetName();
			if(g_stricmp(nodeName, "Name")==0)
			{
				if(pxNode->GetChildren()!=NULL)
				{
					name = pxNode->GetContent();
				}
			}
			else if(g_stricmp(nodeName, "Title")==0)
			{
				if(pxNode->GetChildren()!=NULL)
				{
					title = pxNode->GetContent();
				}
			}
			else if(g_stricmp(nodeName, AUGE_SLD_FEATURETYPE_STYLE)==0)
			{
				pStyle = ReadFeatureStyle(pxNode);
			}
		}
		if(pStyle!=NULL)
		{
			pStyle->SetName(name.c_str());
		}

		pxNodeSet->Release();

		return pStyle;
	}

	FeatureStyle* StyleReader_1_0_0::ReadFeatureStyle(XNode* pxFeatureStyle)
	{
		XNode				*pxNode		= NULL;
		XNodeSet			*pxNodeSet	= NULL;
		const char			*nodeName	= NULL;
		FeatureStyleImpl	*pFStyle	= NULL;

		pFStyle = new FeatureStyleImpl();
		pxNodeSet = pxFeatureStyle->GetChildren();
		if(pxNodeSet==NULL)
		{
			return NULL;
		}

		pxNodeSet->Reset();
		while(!pxNodeSet->IsEOF())
		{
			pxNode = pxNodeSet->Next();
			if(pxNode!=NULL)
			{
				nodeName = pxNode->GetName();
				if(g_stricmp(nodeName, "Name")==0)
				{
					pFStyle->SetName(pxNode->GetContent());
				}
				else if(g_stricmp(nodeName, AUGE_SLD_FEATURETYPE_NAME)==0)
				{
					pFStyle->SetFeatureTypeName(pxNode->GetContent());
				}
				else if(g_stricmp(nodeName, AUGE_SLD_RULE)==0)
				{
					Rule* pRule = NULL;
					pRule = ReadRule(pxNode);
					if(pRule!=NULL)
					{
						pFStyle->AddRule(pRule);
					}
				}
			}
		}

		pxNodeSet->Release();
		return pFStyle;
	}

	Rule* StyleReader_1_0_0::ReadRule(XNode* pxRuleNode)
	{
		XNode		*pxNode		= NULL;
		XNodeSet	*pxNodeSet	= NULL;
		const char	*nodeName	= NULL;
		Rule		*pRule		= NULL;
		GFilter		*pFilter	= NULL;

		pRule = new RuleImpl();
		pxNodeSet = pxRuleNode->GetChildren();
		if(pxNodeSet==NULL)
		{
			return NULL;
		}

		pxNodeSet->Reset();
		while(!pxNodeSet->IsEOF())
		{
			pxNode = pxNodeSet->Next();
			if(pxNode!=NULL)
			{
				nodeName = pxNode->GetName();
				if(g_stricmp(nodeName, "Name")==0)
				{
					pRule->SetName(pxNode->GetContent());
				}
				else if(g_stricmp(nodeName, AUGE_SLD_POINT_SYMBOLIZER)==0)
				{
					PointSymbolizer* pSymbolizer = NULL;
					pSymbolizer = ReadPointSymbolizer(pxNode);
					if(pSymbolizer!=NULL)
					{
						pRule->SetSymbolizer(pSymbolizer);
					}
				}
				else if(g_stricmp(nodeName, AUGE_SLD_LINE_SYMBOLIZER)==0)
				{
					LineSymbolizer* pSymbolizer = NULL;
					pSymbolizer = ReadLineSymbolizer(pxNode);
					if(pSymbolizer!=NULL)
					{
						pRule->SetSymbolizer(pSymbolizer);
					}
				}
				else if(g_stricmp(nodeName, AUGE_SLD_POLYGON_SYMBOLIZER)==0)
				{
					PolygonSymbolizer* pSymbolizer = NULL;
					pSymbolizer = ReadPolygonSymbolizer(pxNode);
					if(pSymbolizer!=NULL)
					{
						pRule->SetSymbolizer(pSymbolizer);
					}
				}
				else if(g_stricmp(nodeName, AUGE_SLD_TEXT_SYMBOLIZER)==0)
				{
					TextSymbolizer* pSymbolizer = NULL;
					pSymbolizer = ReadTextSymbolizer(pxNode);
					if(pSymbolizer!=NULL)
					{
						pRule->SetTextSymbolizer(pSymbolizer);
					}
				}
				else if(g_stricmp(nodeName, AUGE_SLD_MIN_SCALE)==0)
				{
					const char* val = pxNode->GetContent();
					pRule->SetMinScale((val==NULL)?-1.0f:atof(val));
				}
				else if(g_stricmp(nodeName, AUGE_SLD_MAX_SCALE)==0)
				{
					const char* val = pxNode->GetContent();
					pRule->SetMaxScale((val==NULL)?-1.0f:atof(val));
				}

				
			}
		}
		pxNodeSet->Release();

		if(pRule->GetSymbolizer()==NULL)
		{
			pRule->Release();
			return NULL;
		}
		pRule->SetFilter(pFilter);

		return pRule;
	}

	PointSymbolizer* StyleReader_1_0_0::ReadPointSymbolizer(XNode* pxSymbolizerNode)
	{
		XNode		*pxNode    = NULL;
		XNodeSet	*pxNodeSet = NULL;
		Style		*pStyle = NULL;
		const char	*nodeName = NULL;
		PointSymbolizer *pSymbolizer = new PointSymbolizerImpl();

		pxNodeSet = pxSymbolizerNode->GetChildren();
		if(pxNodeSet==NULL)
		{
			return NULL;
		}

		pxNodeSet->Reset();
		while(!pxNodeSet->IsEOF())
		{
			pxNode = pxNodeSet->Next();
			if(pxNode!=NULL)
			{
				nodeName = pxNode->GetName();
				if(g_stricmp(nodeName, AUGE_SLD_GRAPHIC)==0)
				{
					ReadGraphic(pSymbolizer, pxNode);
				}
				else if(g_stricmp(nodeName, AUGE_SLD_GEOMETRY)==0)
				{
					ReadGeometry(pSymbolizer, pxNode);
				}	
			}
		}

		pxNodeSet->Release();

		return pSymbolizer;
	}

	LineSymbolizer* StyleReader_1_0_0::ReadLineSymbolizer(XNode* pxSymbolizerNode)
	{
		XNode		*pxNode    = NULL;
		XNodeSet	*pxNodeSet = NULL;
		Style		*pStyle = NULL;
		const char	*nodeName = NULL;
		LineSymbolizer *pSymbolizer = new LineSymbolizerImpl();

		pxNodeSet = pxSymbolizerNode->GetChildren();
		if(pxNodeSet==NULL)
		{
			return NULL;
		}

		pxNodeSet->Reset();
		while(!pxNodeSet->IsEOF())
		{
			pxNode = pxNodeSet->Next();
			if(pxNode!=NULL)
			{
				nodeName = pxNode->GetName();
				if(g_stricmp(nodeName, AUGE_SLD_STROKE)==0)
				{
					ReadStroke(pSymbolizer, pxNode);
				}
				else if(g_stricmp(nodeName, AUGE_SLD_GEOMETRY)==0)
				{
					ReadGeometry(pSymbolizer, pxNode);
				}	
			}
		}

		pxNodeSet->Release();
		return pSymbolizer;
	}

	PolygonSymbolizer* StyleReader_1_0_0::ReadPolygonSymbolizer(XNode* pxSymbolizerNode)
	{
		XNode		*pxNode    = NULL;
		XNodeSet	*pxNodeSet = NULL;
		Style		*pStyle = NULL;
		const char	*nodeName = NULL;
		PolygonSymbolizer *pSymbolizer = new PolygonSymbolizerImpl();

		pxNodeSet = pxSymbolizerNode->GetChildren();
		if(pxNodeSet==NULL)
		{
			return NULL;
		}

		pxNodeSet->Reset();
		while(!pxNodeSet->IsEOF())
		{
			pxNode = pxNodeSet->Next();
			if(pxNode!=NULL)
			{
				nodeName = pxNode->GetName();
				if(g_stricmp(nodeName, AUGE_SLD_FILL)==0)
				{
					ReadFill(pSymbolizer, pxNode);
				}
				else if(g_stricmp(nodeName, AUGE_SLD_STROKE)==0)
				{
					ReadStroke(pSymbolizer, pxNode);
				}
				else if(g_stricmp(nodeName, AUGE_SLD_GEOMETRY)==0)
				{
					ReadGeometry(pSymbolizer, pxNode);
				}	
			}
		}

		pxNodeSet->Release();

		return pSymbolizer;
	}

	TextSymbolizer* StyleReader_1_0_0::ReadTextSymbolizer(XNode* pxSymbolizerNode)
	{
		XNode		*pxNode    = NULL;
		XNodeSet	*pxNodeSet = NULL;
		Style		*pStyle = NULL;
		const char	*nodeName = NULL;
		TextSymbolizer* pSymbolizer = new TextSymbolizerImpl();

		pxNodeSet = pxSymbolizerNode->GetChildren();
		if(pxNodeSet==NULL)
		{
			return NULL;
		}

		pxNodeSet->Reset();
		while(!pxNodeSet->IsEOF())
		{
			pxNode = pxNodeSet->Next();
			if(pxNode!=NULL)
			{
				nodeName = pxNode->GetName();
				if(g_stricmp(nodeName, "Label")==0)
				{
					ReadLabel(pSymbolizer, pxNode);
				}
				else if(g_stricmp(nodeName, "Font")==0)
				{
					ReadFont(pSymbolizer, pxNode);
				}
				else if(g_stricmp(nodeName, "LabelPlacement")==0)
				{
					ReadPlacement(pSymbolizer, pxNode);
				}
				else if(g_stricmp(nodeName, "Fill")==0)
				{
					ReadFill(pSymbolizer, pxNode);
				}
				else if(g_stricmp(nodeName, "Stroke")==0)
				{
					ReadStroke(pSymbolizer, pxNode);
				}
			}
		}

		pxNodeSet->Release();

		return pSymbolizer;
	}

	//double StyleReader_1_0_0::ReadSize(XNode* pxSizeNode)
	//{
	//	XNode		*pxNode    = NULL;
	//	XNodeSet	*pxNodeSet = NULL;
	//	pxNodeSet = pxSizeNode->GetChildren();
	//	if(pxNodeSet->Count()==0)
	//	{
	//		return 1.0f;
	//	}

	//	pxNode = pxNodeSet->Item(0);
	//	double size = atof(pxNode->GetContent());
	//	pxNodeSet->Release();
	//	return size;
	//}

	bool  StyleReader_1_0_0::ReadGraphic(PointSymbolizer* pSymbolizer, XNode* pxGraphicNode)
	{
		XNode		*pxNode    = NULL;
		XNodeSet	*pxNodeSet = NULL;
		Style		*pStyle = NULL;
		const char	*nodeName = NULL;

		pxNodeSet = pxGraphicNode->GetChildren();
		if(pxNodeSet==NULL)
		{
			return false;
		}

		pxNodeSet->Reset();
		while(!pxNodeSet->IsEOF())
		{
			pxNode = pxNodeSet->Next();
			if(pxNode!=NULL)
			{
				nodeName = pxNode->GetName();
				if(g_stricmp(nodeName, AUGE_SLD_SIZE)==0)
				{
					double size = 1.0f;
					if(pxNode->GetContent()!=NULL)
					{
						size = atof(pxNode->GetContent());
						pSymbolizer->SetSize(size);
					}
				}
				else if(g_stricmp(nodeName, AUGE_SLD_MARK)==0)
				{
					ReadMark(pSymbolizer, pxNode);
				}
			}
		}

		pxNodeSet->Release();
		return true;
	}

	bool StyleReader_1_0_0::ReadGeometry(Symbolizer* pSymbolizer, XNode* pxGeometryNode)
	{
		return true;
	}

	bool StyleReader_1_0_0::ReadMark(PointSymbolizer* pSymbolizer, XNode* pxMarkNode)
	{
		XNode		*pxNode    = NULL;
		XNodeSet	*pxNodeSet = NULL;
		Style		*pStyle = NULL;
		const char	*nodeName = NULL;

		pxNodeSet = pxMarkNode->GetChildren();
		if(pxNodeSet==NULL)
		{
			return false;
		}

		pxNodeSet->Reset();
		while(!pxNodeSet->IsEOF())
		{
			pxNode = pxNodeSet->Next();
			if(pxNode!=NULL)
			{
				nodeName = pxNode->GetName();
				if(g_stricmp(nodeName, AUGE_SLD_STROKE)==0)
				{
					Stroke* pStroke = ReadStroke(pxNode);
					pSymbolizer->SetStroke(pStroke);
				}
				else if(g_stricmp(nodeName, AUGE_SLD_FILL)==0)
				{
					Fill* pFill = ReadFill(pxNode);
					pSymbolizer->SetFill(pFill);
					if(pFill!=NULL)
					{
						pSymbolizer->SetOpacity(pFill->GetColor().GetAlphaF());
					}
				}
			}
		}

		pxNodeSet->Release();
		return true;
	}

	bool StyleReader_1_0_0::ReadStroke(LineSymbolizer* pSymbolizer, XNode* pxStrokeNode)
	{
		Stroke* pStroke = ReadStroke(pxStrokeNode);
		pSymbolizer->SetStroke(pStroke);
		return true;
	}

	bool StyleReader_1_0_0::ReadFill(PolygonSymbolizer* pSymbolizer, XNode* pxFillNode)
	{
		Fill* pFill = ReadFill(pxFillNode);
		pSymbolizer->SetFill(pFill);
		return true;
	}

	bool  StyleReader_1_0_0::ReadStroke(PolygonSymbolizer* pSymbolizer, XNode* pxStrokeNode)
	{
		Stroke* pStroke = ReadStroke(pxStrokeNode);
		pSymbolizer->SetStroke(pStroke);
		return true;
	}

	bool StyleReader_1_0_0::ReadMark(PolygonSymbolizer* pSymbolizer, XNode* pxMarkNode)
	{
		XNode		*pxNode    = NULL;
		XNodeSet	*pxNodeSet = NULL;
		Style		*pStyle = NULL;
		const char	*nodeName = NULL;

		pxNodeSet = pxMarkNode->GetChildren();
		if(pxNodeSet==NULL)
		{
			return false;
		}

		pxNodeSet->Reset();
		while(!pxNodeSet->IsEOF())
		{
			pxNode = pxNodeSet->Next();
			if(pxNode!=NULL)
			{
				nodeName = pxNode->GetName();
				if(g_stricmp(nodeName, AUGE_SLD_STROKE)==0)
				{
					Stroke* pStroke = ReadStroke(pxNode);
					pSymbolizer->SetStroke(pStroke);
				}
				else if(g_stricmp(nodeName, AUGE_SLD_FILL)==0)
				{
					Fill* pFill = ReadFill(pxNode);
					pSymbolizer->SetFill(pFill);
					if(pFill!=NULL)
					{
						pSymbolizer->SetOpacity(pFill->GetColor().GetAlphaF());
					}
				}
			}
		}

		pxNodeSet->Release();
		return true;
	}

	bool StyleReader_1_0_0::ReadLabel(TextSymbolizer* pSymbolizer, XNode* pxLabelNode)
	{
		XNode* pxPropertyName = pxLabelNode->GetFirstChild("PropertyName");
		if(pxPropertyName!=NULL)
		{
			const char* prop_name = pxPropertyName->GetContent();
			pSymbolizer->SetLabel(prop_name);
		}
		return true;
	}

	bool StyleReader_1_0_0::ReadPlacement(TextSymbolizer* pSymbolizer, XNode* pxPlacementNode)
	{
		XNode* pxPointPlacement = pxPlacementNode->GetFirstChild("PointPlacement");
		if(pxPointPlacement==NULL)
		{
			return false;
		}

		//XNode* pxPOffset = pxLinePlacement->GetFirstChild("PerpendicularOffset");
		//if(pxPOffset==NULL)
		//{
		//	return false;
		//}
		return true;
	}

	bool StyleReader_1_0_0::ReadFont(TextSymbolizer* pSymbolizer, XNode* pxFontNode)
	{
		XNode		*pxNode    = NULL;
		XElement	*pxElement = NULL;
		XAttribute	*pxAttr	   = NULL;
		XNodeSet	*pxNodeSet = NULL;
		Style		*pStyle	   = NULL;
		const char	*attrName  = NULL;
		Font		*pFont	   = pSymbolizer->GetFont();
		
		pxNodeSet = pxFontNode->GetChildren();
		if(pxNodeSet==NULL)
		{
			return NULL;
		}

		pxNodeSet->Reset();
		while(!pxNodeSet->IsEOF())
		{
			pxElement = (XElement*)pxNodeSet->Next();
			pxAttr = pxElement->GetAttribute("name");
			if(pxAttr!=NULL)
			{
				attrName = pxAttr->GetValue();
				if(g_stricmp(attrName, "font-family")==0)
				{
					const char* value = pxElement->GetContent();
					pFont->SetFamily(value);
				}
				else if(g_stricmp(attrName, "font-size")==0)
				{
					const char* value = pxElement->GetContent();
					if(value!=NULL)
					{
						pFont->SetSize(atof(value));
					}
				}
				else if(g_stricmp(attrName, "font-style")==0)
				{
					const char* value = pxElement->GetContent();
					if(value!=NULL)
					{
						if(g_stricmp(value,"normal")==0)
						{
							pFont->SetStyle(augeFontStyleNormal);
						}
						else if(g_stricmp(value,"italic")==0)
						{
							pFont->SetStyle(augeFontStyleItalic);
						}
						else if(g_stricmp(value,"oblique")==0)
						{
							pFont->SetStyle(augeFontStyleOblique);
						}
					}
				}
				else if(g_stricmp(attrName, "font-weight")==0)
				{
					const char* value = pxElement->GetContent();
					if(value!=NULL)
					{
						if(g_stricmp(value,"normal")==0)
						{
							pFont->SetWeight(augeFontWeightNormal);
						}
						else if(g_stricmp(value,"bold")==0)
						{
							pFont->SetWeight(augeFontWeightBold);
						}
					}
				}
				
			}
		}

		pxNodeSet->Release();
		return true;
	}

	bool StyleReader_1_0_0::ReadFill(TextSymbolizer* pSymbolizer, XNode* pxFillNode)
	{
		Fill* pFill = ReadFill(pxFillNode);
		pSymbolizer->SetFill(pFill);
		return true;
	}

	bool StyleReader_1_0_0::ReadStroke(TextSymbolizer* pSymbolizer, XNode* pxStrokeNode)
	{
		//Stroke* pStroke = ReadStroke(pxStrokeNode);
		//pSymbolizer->SetStroke(pStroke);
		return true;
	}


	Stroke* StyleReader_1_0_0::ReadStroke(XNode* pxStrokeNode)
	{
		XNode		*pxNode    = NULL;
		XElement	*pxElement = NULL;
		XAttribute	*pxAttr	   = NULL;
		XNodeSet	*pxNodeSet = NULL;
		Style		*pStyle	   = NULL;
		const char	*attrName  = NULL;
		Stroke		*pStroke = new StrokeImpl();
		g_uint r, g, b, a;

		GColor& c = pStroke->GetColor();
		r = c.GetRed();
		g = c.GetGreen();
		b = c.GetBlue();
		a = c.GetAlpha();

		pxNodeSet = pxStrokeNode->GetChildren();
		if(pxNodeSet==NULL)
		{
			return NULL;
		}

		pxNodeSet->Reset();
		while(!pxNodeSet->IsEOF())
		{
			pxElement = (XElement*)pxNodeSet->Next();
			pxAttr = pxElement->GetAttribute("name");
			if(pxAttr!=NULL)
			{
				attrName = pxAttr->GetValue();
				if(g_stricmp(attrName, "stroke")==0)
				{
					const char* value = pxElement->GetContent();
					if(value!=NULL)
					{
						sscanf(value, "#%2x%2x%2x",&r,&g,&b);
					}
				}
				else if(g_stricmp(attrName, "stroke-opacity")==0)
				{
					const char* value = pxElement->GetContent();
					if(value!=NULL)
					{
						double v = atof(value);
						a = (int)((v * 255) + 0.5f);
					}
				}
				else if(g_stricmp(attrName, "stroke-width")==0)
				{
					const char* value = pxElement->GetContent();
					if(value!=NULL)
					{
						double w = atof(value);
						pStroke->SetWidth(w);
					}
				}
			}
		}
		c.Set(r,g,b,a);

		pxNodeSet->Release();
		return pStroke;
	}

	Fill* StyleReader_1_0_0::ReadFill(XNode* pxFillNode)
	{
		XNode		*pxNode    = NULL;
		XElement	*pxElement = NULL;
		XAttribute	*pxAttr	   = NULL;
		XNodeSet	*pxNodeSet = NULL;
		Style		*pStyle	   = NULL;
		const char	*attrName  = NULL;
		Fill		*pFill = new FillImpl();
		g_uint r, g, b, a;

		GColor& c = pFill->GetColor();
		r = c.GetRed();
		g = c.GetGreen();
		b = c.GetBlue();
		a = c.GetAlpha();

		pxNodeSet = pxFillNode->GetChildren();
		if(pxNodeSet==NULL)
		{
			return NULL;
		}

		pxNodeSet->Reset();
		while(!pxNodeSet->IsEOF())
		{
			pxElement = (XElement*)pxNodeSet->Next();
			pxAttr = pxElement->GetAttribute("name");
			if(pxAttr!=NULL)
			{
				attrName = pxAttr->GetValue();
				if(g_stricmp(attrName, "fill")==0)
				{
					const char* value = pxElement->GetContent();
					if(value!=NULL)
					{
						sscanf(value, "#%2x%2x%2x",&r,&g,&b);
					}
				}
				else if(g_stricmp(attrName, "fill-opacity")==0)
				{
					const char* value = pxElement->GetContent();
					if(value!=NULL)
					{
						double v = atof(value);
						a = (int)((v * 255) + 0.5f);
					}
				}
			}
		}
		c.Set(r,g,b,a);

		pxNodeSet->Release();
		return pFill;
	}

	//RESULTCODE StyleReader_1_0_0::ReadLabel(TextSymbolizer *pSymbolizer, XNode *pxNodeLabel)
	//{	
	//	XNode* pxNode = NULL;
	//	pxNode = pxNodeLabel->GetFirstChild();

	//	const char* nodeName = NULL;

	//	//pxNodeSet->Reset();
	//	//while(!pxNodeSet->IsEOF())
	//	//{
	//	//	pxNode = pxNodeSet->Next();
	//	//	nodeName = pxNode->GetName();
	//	//	if(g_stricmp(nodeName, "PropertyName")==0)
	//	//	{
	//	//		pSymbolizer->SetLabel(pxNode->GetContent());
	//	//	}
	//	//	else if(g_stricmp(nodeName, "PropertyName")==0)
	//	//	{

	//	//	}

	//	//}

	//	return AG_FAILURE;
	//}

}

