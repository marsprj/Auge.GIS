#include "StyleWriter_1_0_0.h"

namespace auge
{
	StyleWriter_1_0_0::StyleWriter_1_0_0()
	{

	}

	StyleWriter_1_0_0::~StyleWriter_1_0_0()
	{

	}

	XDocument* StyleWriter_1_0_0::Write(Style* pStyle, const char* version/*="1.0"*/)
	{
		XDocument* pxDoc = NULL;
		augeStyleType type = pStyle->GetType();

		switch(type)
		{
		case augeStyleFeature:
			pxDoc = WriteFeature(static_cast<FeatureStyle*>(pStyle));
			break;
		case augeStyleRaster:
			pxDoc = WriteRaster(static_cast<RasterStyle*>(pStyle));
			break;
		}

		return pxDoc;
	}

	XDocument* StyleWriter_1_0_0::WriteFeature(FeatureStyle* pStyle)
	{
		XDocument	*pxDoc = new XDocument();
		
		XElement *pxNode = NULL;
		XElement *pxRoot = NULL;
		pxRoot = pxDoc->CreateRootNode("StyledLayerDescriptor", "http://www.opengis.net/sld", "sld");
		pxRoot->SetAttribute("version","1.0.0", NULL);
		pxRoot->SetNamespaceDeclaration("http://www.opengis.net/sld",NULL);
		pxRoot->SetNamespaceDeclaration("http://www.opengis.net/ogc","ogc");
		pxRoot->SetNamespaceDeclaration("http://www.opengis.net/gml","gml");

		XElement *pxUserLayer = NULL;
		pxUserLayer = pxRoot->AddChild("UserLayer", "sld");

		XElement *pxFeatureConstrains = NULL;
		pxFeatureConstrains = pxUserLayer->AddChild("LayerFeatureConstraints", "sld");
		
		// UserStyle
		XElement *pxUserStyle = NULL;
		pxUserStyle = pxUserLayer->AddChild("UserStyle", "sld");
		pxNode = pxUserStyle->AddChild("Name", "sld");
		pxNode->AddChildText(pStyle->GetName());
		pxNode = pxUserStyle->AddChild("Title", "sld");
		pxNode->AddChildText(pStyle->GetName());

		XElement *pxFeatureStyle = NULL;
		pxFeatureStyle = pxUserStyle->AddChild("FeatureTypeStyle", "sld");
		WriteFeatureStyle(pxFeatureStyle, pStyle);

		return pxDoc;
	}

	XDocument* StyleWriter_1_0_0::WriteRaster(RasterStyle* pStyle)
	{
		return NULL;
	}

	RESULTCODE StyleWriter_1_0_0::WriteFeatureStyle(XElement* pxFeature, FeatureStyle* pStyle)
	{
		XElement *pxNode = NULL;

		pxNode = pxFeature->AddChild("Name", "sld");
		pxNode->AddChildText(pStyle->GetName());
		pxNode = pxFeature->AddChild("FeatureTypeName", "sld");
		pxNode->AddChildText(pStyle->GetName());
		pxNode = pxFeature->AddChild("SemanticTypeIdentifier", "sld");
		pxNode->AddChildText("generic:geometry");

		Rule* pRule = NULL;
		g_uint count = pStyle->GetRuleCount();
		for(g_uint i=0; i<count; i++)
		{
			pRule = pStyle->GetRule(i);
			if(pRule!=NULL)
			{
				XElement *pxRule = NULL;
				pxRule = pxFeature->AddChild("Rule","sld");
				WriteRule(pxRule, pRule);
			}
		}

		return AG_SUCCESS;
	}

	RESULTCODE StyleWriter_1_0_0::WriteRule(XElement* pxRule, Rule* pRule)
	{
		XElement *pxNode = NULL;

		pxNode = pxRule->AddChild("Name", "sld");
		pxNode->AddChildText(pRule->GetName());
		pxNode = pxRule->AddChild("Title", "sld");
		pxNode->AddChildText(pRule->GetName());

		WriteFilter(pxRule, pRule->GetFilter());

		Symbolizer* pSymbolizer = NULL;
		pSymbolizer = pRule->GetSymbolizer();
		if(pSymbolizer!=NULL)
		{
			WriteSymbolizer(pxRule, pSymbolizer);
		}
		
		TextSymbolizer* pTextSymbolizer = pRule->GetTextSymbolizer();
		if(pTextSymbolizer!=NULL)
		{
			WriteSymbolizer(pxRule, pTextSymbolizer);
		}
		
		return AG_SUCCESS;
	}

	RESULTCODE StyleWriter_1_0_0::WriteFilter(XElement* pxRule, GFilter* pFilter)
	{
		XElement *pxFilter = NULL;
		pxFilter = pxRule->AddChild("Filter", "ogc");

		if(pFilter==NULL)
		{
			return AG_SUCCESS;
		}

		return AG_SUCCESS;
	}

	RESULTCODE StyleWriter_1_0_0::WriteSymbolizer(XElement* pxRule, Symbolizer* pSymbolizer)
	{
		RESULTCODE rc = AG_SUCCESS;
		switch(pSymbolizer->GetType())
		{
		case augeSymbolPoint:
			rc = WriteSymbolizer(pxRule, static_cast<PointSymbolizer*>(pSymbolizer));
			break;
		case augeSymbolLine:
			rc = WriteSymbolizer(pxRule, static_cast<LineSymbolizer*>(pSymbolizer));
			break;
		case augeSymbolPolygon:
			rc = WriteSymbolizer(pxRule, static_cast<PolygonSymbolizer*>(pSymbolizer));
			break;
		case augeSymbolText:
			rc = WriteSymbolizer(pxRule, static_cast<TextSymbolizer*>(pSymbolizer));
			break;
		}
		return rc;
	}

	RESULTCODE StyleWriter_1_0_0::WriteSymbolizer(XElement* pxRule, PointSymbolizer* pSymbolizer)
	{
		XElement *pxNode = NULL;
		XElement *pxSymbolizer = NULL;
		pxSymbolizer = pxRule->AddChild("PointSymbolizer", "sld");
		
		XElement *pxGraphic = NULL;
		pxGraphic = pxSymbolizer->AddChild("Graphic", "sld");
		XElement *pxMark = NULL;
		pxMark = pxGraphic->AddChild("Mark", "sld");
		Fill* pFill = pSymbolizer->GetFill();
		if(pFill!=NULL)
		{
			XElement *pxFill = pxMark->AddChild("Fill", "sld");
			WriteFill(pxFill, pFill);
		}
		Stroke* pStroke = pSymbolizer->GetStroke();
		if(pStroke!=NULL)
		{
			XElement *pxStroke = pxMark->AddChild("Stroke", "sld");
			WriteStroke(pxStroke, pStroke);
		}
		char buffer[AUGE_NAME_MAX] = {0};
		g_sprintf(buffer, "%f", pSymbolizer->GetSize());
		pxNode = pxGraphic->AddChild("Size", NULL);
		pxNode->AddChildText(buffer);

		return AG_SUCCESS;
	}

	RESULTCODE StyleWriter_1_0_0::WriteSymbolizer(XElement* pxRule, LineSymbolizer* pSymbolizer)
	{
		XElement *pxSymbolizer = NULL;
		pxSymbolizer = pxRule->AddChild("LineSymbolizer", "sld");

		Stroke* pStroke = pSymbolizer->GetStroke();
		if(pStroke!=NULL)
		{
			XElement *pxStroke = pxSymbolizer->AddChild("Stroke", "sld");
			WriteStroke(pxStroke, pStroke);
		}

		return AG_SUCCESS;
	}

	RESULTCODE StyleWriter_1_0_0::WriteSymbolizer(XElement* pxRule, PolygonSymbolizer* pSymbolizer)
	{
		XElement *pxSymbolizer = NULL;
		pxSymbolizer = pxRule->AddChild("PolygonSymbolizer", "sld");

		XElement *pxGeometry = NULL;
		pxGeometry = pxSymbolizer->AddChild("Geometry", "sld");

		Fill* pFill = pSymbolizer->GetFill();
		if(pFill!=NULL)
		{
			XElement *pxFill = pxSymbolizer->AddChild("Fill", "sld");
			WriteFill(pxFill, pFill);
		}
		Stroke* pStroke = pSymbolizer->GetStroke();
		if(pStroke!=NULL)
		{
			XElement *pxStroke = pxSymbolizer->AddChild("Stroke", "sld");
			WriteStroke(pxStroke, pStroke);
		}

		return AG_SUCCESS;
	}

	RESULTCODE StyleWriter_1_0_0::WriteSymbolizer(XElement* pxRule, TextSymbolizer* pSymbolizer)
	{
		XElement *pxSymbolizer = NULL;
		pxSymbolizer = pxRule->AddChild("TextSymbolizer", "sld");

		//sld:Label
		XElement *pxNode = NULL;
		XElement *pxLabel = pxSymbolizer->AddChild("Label", "sld");
		if(pSymbolizer->GetLabel()!=NULL)
		{
			pxNode = pxLabel->AddChild("PropertyName", "ogc");
			pxNode->SetChildText(pSymbolizer->GetLabel());
		}

		// sld:LabelPlacement
		XElement *pxPlacement = pxSymbolizer->AddChild("LabelPlacement", "sld");

		// sld:Font
		char str[AUGE_NAME_MAX];
		Font* pFont = pSymbolizer->GetFont();
		XElement *pxFont = pxSymbolizer->AddChild("Font", "sld");
		pxNode = pxFont->AddChild("CssParameter", "sld");
		pxNode->SetAttribute("name","font-family",NULL);
		pxNode->SetChildText(pFont->GetFamily());

		g_sprintf(str,"%f",pFont->GetSize());
		pxNode = pxFont->AddChild("CssParameter", "sld");
		pxNode->SetAttribute("name","font-size",NULL);
		pxNode->SetChildText(str);

		const char* style = NULL;
		switch(pFont->GetStyle())
		{
		case augeFontStyleNormal:
			style = "normal";
			break;
		case augeFontStyleItalic:
			style = "italic";
			break;
		case augeFontStyleOblique:
			style = "oblique";
			break;
		}
		pxNode = pxFont->AddChild("CssParameter", "sld");
		pxNode->SetAttribute("name","font-style",NULL);
		pxNode->SetChildText(style);

		const char* weigt = NULL;
		switch(pFont->GetWeight())
		{
		case augeFontWeightNormal:
			weigt = "normal";
			break;
		case augeFontWeightBold:
			weigt = "bold";
			break;
		}
		pxNode = pxFont->AddChild("CssParameter", "sld");
		pxNode->SetAttribute("name","font-weight",NULL);
		pxNode->SetChildText(weigt);

		// sld:Fill
		XElement *pxFill = pxSymbolizer->AddChild("Fill","sld");
		Fill* pFill = pSymbolizer->GetFill();
		WriteFill(pxFill, pFill);

		return AG_SUCCESS;
	}

	RESULTCODE StyleWriter_1_0_0::WriteFill(XElement* pxFill, Fill* pFill)
	{
		if(pFill==NULL)
		{
			return AG_FAILURE;
		}

		GColor& c = pFill->GetColor();
		g_uint r = c.GetRed();
		g_uint g = c.GetGreen();
		g_uint b = c.GetBlue();
		float  a = c.GetAlphaF();

		XElement *pxNode = NULL;
		char buffer[AUGE_NAME_MAX] = {0};

		g_sprintf(buffer, "#%02x%02x%02x",r,g,b);
		pxNode = pxFill->AddChild("CssParameter","sld");
		pxNode->SetAttribute("name","fill",NULL);
		pxNode->SetChildText(buffer);

		g_sprintf(buffer, "%f",a);
		pxNode = pxFill->AddChild("CssParameter","sld");
		pxNode->SetAttribute("name","fill-opacity",NULL);
		pxNode->SetChildText(buffer);

		return AG_SUCCESS;
	}

	RESULTCODE StyleWriter_1_0_0::WriteStroke(XElement* pxStroke, Stroke* pStroke)
	{
		if(pStroke==NULL)
		{
			return AG_FAILURE;
		}

		GColor& c = pStroke->GetColor();
		g_uint r = c.GetRed();
		g_uint g = c.GetGreen();
		g_uint b = c.GetBlue();
		float  a = c.GetAlphaF();

		XElement *pxNode = NULL;
		char buffer[AUGE_NAME_MAX] = {0};

		g_sprintf(buffer, "#%02x%02x%02x",r,g,b);
		pxNode = pxStroke->AddChild("CssParameter","sld");
		pxNode->SetAttribute("name","stroke",NULL);
		pxNode->SetChildText(buffer);

		g_sprintf(buffer, "%f",a);
		pxNode = pxStroke->AddChild("CssParameter","sld");
		pxNode->SetAttribute("name","stroke-opacity",NULL);
		pxNode->SetChildText(buffer);

		g_sprintf(buffer, "%f", pStroke->GetWidth());
		pxNode = pxStroke->AddChild("CssParameter","sld");
		pxNode->SetAttribute("name","stroke-width",NULL);
		pxNode->SetChildText(buffer);
		
		return AG_SUCCESS;
	}
}