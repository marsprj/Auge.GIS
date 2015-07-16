#include "StyleFactoryImpl.h"
#include "FontImpl.h"
#include "MarkImpl.h"
#include "FillImpl.h"
#include "StrokeImpl.h"
#include "RuleImpl.h"
#include "PointSymbolizerImpl.h"
#include "LineSymbolizerImpl.h"
#include "PolygonSymbolizerImpl.h"
#include "RasterSymbolizerImpl.h"
#include "TextSymbolizerImpl.h"
//#include "FeatureStyleImpl.h"
#include "FeatureStyleImpl.h"
#include "RasterStyleImpl.h"

#include "ColorMapImpl.h"

#include "StyleReaderImpl.h"
#include "StyleWriterImpl.h"


namespace auge
{
	FeatureStyle* CreatePointStyle();
	FeatureStyle* CreateLineStyle();
	FeatureStyle* CreatePolygonStyle();
	FeatureStyle* CreateTextStyle();
	FeatureStyle* CreateRasterStyle();

	PointSymbolizer*	CreatePointSymbolizer();	
	LineSymbolizer*		CreateLineSymbolizer();	
	PolygonSymbolizer*	CreatePolygonSymbolizer();	
	TextSymbolizer*		CreateTextSymbolizer();

	StyleFactoryImpl::StyleFactoryImpl()
	{

	}

	StyleFactoryImpl::~StyleFactoryImpl()
	{

	}

	//////////////////////////////////////////////////////////////////////////
	StyleFactory* augeGetStyleFactoryInstance()
	{
		static StyleFactoryImpl g_styleFactory;
		return &g_styleFactory;
	}
	//////////////////////////////////////////////////////////////////////////

	PointSymbolizer* StyleFactoryImpl::CreatePointSymbolizer()
	{
		//auge::GColor color(255,255,255,255);
		auge::GColor color(255,0,0,255);
		auge::PointSymbolizer* pPointSymbolizer = NULL;
		pPointSymbolizer = new PointSymbolizerImpl();

		//auge::Fill *pFill = new FillImpl();
		//pPointSymbolizer->SetFill(pFill);

		//auge::Stroke *pStroke = new StrokeImpl();
		//pPointSymbolizer->SetStroke(pStroke);

		return pPointSymbolizer;
	}

	LineSymbolizer* StyleFactoryImpl::CreateLineSymbolizer()
	{
		auge::LineSymbolizer* pLineSymbolizer = NULL;
		pLineSymbolizer = new LineSymbolizerImpl();

		//auge::GColor color(0,255,255,255);
		auge::GColor color(255,0,0,255);
		auge::Stroke *pStroke = new StrokeImpl();
		pStroke->SetWidth(0.2f);

		pLineSymbolizer->SetStroke(pStroke);
		return pLineSymbolizer;
	}

	PolygonSymbolizer* StyleFactoryImpl::CreatePolygonSymbolizer()
	{
		//auge::PolygonSymbolizer* pPolygonSymbolizer = NULL;
		//pPolygonSymbolizer = new PolygonSymbolizerImpl();

		//auge::GColor color(0,255,255,255);
		//auge::Stroke *pStroke = new StrokeImpl();

		//pPolygonSymbolizer->SetStroke(pStroke);
		//return pPolygonSymbolizer;

		Fill	*pFill = NULL;
		Stroke	*pStroke = NULL;
		PolygonSymbolizer *pPolygonSymbolizer = NULL;
		pPolygonSymbolizer = new PolygonSymbolizerImpl();
		pFill = new FillImpl();
		GColor fcolor;
		fcolor.SetAlpha(100);
		pFill->SetColor(fcolor);
		//pFill->GetColor().Set(234,0,32,100);
		//pFill->GetColor().Set(234,0,32,255);
		pPolygonSymbolizer->SetFill(pFill);
		pStroke = new StrokeImpl();;
		pStroke->GetColor().SetAlpha(255);
		pPolygonSymbolizer->SetStroke(pStroke);	
		return pPolygonSymbolizer;
	}

	TextSymbolizer* StyleFactoryImpl::CreateTextSymbolizer()
	{
		return (new TextSymbolizerImpl());
	}

	Font*	StyleFactoryImpl::CreateFont()
	{
		return (new FontImpl());
	}

	Fill*	StyleFactoryImpl::CreateFill()
	{
		return (new FillImpl());
	}

	Mark*	StyleFactoryImpl::CreateMark()
	{
		return (new MarkImpl());
	}

	Stroke* StyleFactoryImpl::CreateStroke()
	{
		return (new StrokeImpl());
	}

	//StyleReader* StyleFactoryImpl::CreateStyleReader()
	//{
	//	return (new StyleReaderImpl());
	//}
	//
	//StyleWriter* StyleFactoryImpl::CreateStyleWriter()
	//{
	//	return (new StyleWriterImpl());
	//}

	FeatureStyle* StyleFactoryImpl::CreateFeatureStyle()
	{
		return (new FeatureStyleImpl());
	}

	FeatureStyle* StyleFactoryImpl::CreateFeatureStyle(augeSymbolizerType type)
	{
		FeatureStyle* pFeatureStyle = NULL;
		switch(type)
		{
		case augeSymbolPoint:
			pFeatureStyle = CreatePointStyle();
			break;
		case augeSymbolLine:
			pFeatureStyle = CreateLineStyle();
			break;
		case augeSymbolPolygon:
			pFeatureStyle = CreatePolygonStyle();
			break;
		case augeSymbolText:
			pFeatureStyle = CreateTextStyle();
			break;
		//case augeSymbolRaster:
		//	pFeatureStyle = CreateRasterStyle();
		//	break;
		}
		return pFeatureStyle;
	}

	FeatureStyle* StyleFactoryImpl::CreateFeatureStyle(augeGeometryType type)
	{
		FeatureStyle* pFeatureStyle = NULL;
		switch(type)
		{
		case augeGTPoint:
		case augeGTMultiPoint:
			pFeatureStyle = CreatePointStyle();
			break;
		case augeGTLineString:
		case augeGTMultiLineString:
			pFeatureStyle = CreateLineStyle();
			break;
		case augeGTPolygon:
		case augeGTMultiPolygon:
			pFeatureStyle = CreatePolygonStyle();
			break;
		}
		return pFeatureStyle;
	}

	RasterStyle* StyleFactoryImpl::CreateRasterStyle()
	{
		return (new RasterStyleImpl());
	}

	Rule* StyleFactoryImpl::CreateRule()
	{
		return (new RuleImpl());
	}

	ColorMap* StyleFactoryImpl::CreateColorMap(g_uint count, GColor& start, GColor& end)
	{
		if(count==0)
		{
			return NULL;
		}
		ColorMapImpl* pColorMap = new ColorMapImpl();
		pColorMap->Create(-1, start,end);
		pColorMap->SetCount(count);
		return pColorMap;
	}

	StyleReader* StyleFactoryImpl::CreateStyleReader()
	{
		return (new StyleReaderImpl());
	}

	StyleWriter* StyleFactoryImpl::CreateStyleWriter()
	{
		return (new StyleWriterImpl());
	}


	FeatureStyle* CreatePointStyle()
	{
		// Point Symbol 
		Fill *pFill = new FillImpl();

		Stroke *pStroke = new StrokeImpl();

		PointSymbolizer *pPointSymbolizer = NULL;
		pPointSymbolizer = CreatePointSymbolizer();

		Rule *pRule = new RuleImpl();
		pRule->SetSymbolizer(pPointSymbolizer);

		GFilter *pFilter = NULL;
		pRule->SetFilter(pFilter);

		FeatureStyle *pFeatureStyle = NULL;
		pFeatureStyle = new FeatureStyleImpl();
		pFeatureStyle->AddRule(pRule);

		return pFeatureStyle;
	}

	FeatureStyle* CreateLineStyle()
	{
		// Polygon Symbol 
		Fill *pFill = new FillImpl();

		Stroke *pStroke = new StrokeImpl();

		LineSymbolizer *pSymbolizer = NULL;
		pSymbolizer = CreateLineSymbolizer();

		Rule *pRule = new RuleImpl();
		pRule->SetSymbolizer(pSymbolizer);

		GFilter *pFilter = NULL;
		pRule->SetFilter(pFilter);

		FeatureStyle *pFeatureStyle = NULL;
		pFeatureStyle = new FeatureStyleImpl();
		pFeatureStyle->AddRule(pRule);

		return pFeatureStyle;
	}

	FeatureStyle* CreatePolygonStyle()
	{
		// Polygon Symbol 
		Fill *pFill = new FillImpl();

		Stroke *pStroke = new StrokeImpl();

		PolygonSymbolizer *pSymbolizer = NULL;
		pSymbolizer = CreatePolygonSymbolizer();

		Rule *pRule = new RuleImpl();
		pRule->SetSymbolizer(pSymbolizer);

		GFilter *pFilter = NULL;
		pRule->SetFilter(pFilter);

		FeatureStyle *pFeatureStyle = NULL;
		pFeatureStyle = new FeatureStyleImpl();
		pFeatureStyle->AddRule(pRule);

		return pFeatureStyle;
	}

	FeatureStyle* CreateTextStyle()
	{
		return NULL;
	}

	FeatureStyle* CreateRasterStyle()
	{
		return NULL;
	}

	PointSymbolizer* CreatePointSymbolizer()
	{
		Fill *pFill = NULL;
		pFill = new FillImpl();

		Stroke *pStroke = NULL;
		pStroke = new StrokeImpl();

		PointSymbolizer *pPointSymbolizer = NULL;
		pPointSymbolizer = new PointSymbolizerImpl();
		//pPointSymbolizer->SetFill(pFill);
		//pPointSymbolizer->SetStroke(pStroke);
		//pPointSymbolizer->SetSize(3.0f);
		pPointSymbolizer->SetGeometry("SHAPE");

		return pPointSymbolizer;
	}

	LineSymbolizer* CreateLineSymbolizer()
	{
		Fill	*pFill = NULL;
		Stroke	*pStroke = NULL;
		LineSymbolizer *pLineSymbolizer = NULL;
		pLineSymbolizer = new LineSymbolizerImpl();
		pFill = new FillImpl();
		pStroke = new StrokeImpl();;
		//	pStroke->GetColor().Set(255,0,0,200);
		pStroke->SetWidth(0.5f);
		//	pStroke->SetWidth(5.0f);
		pLineSymbolizer->SetStroke(pStroke);

		return pLineSymbolizer;
	}

	PolygonSymbolizer* CreatePolygonSymbolizer()
	{
		Fill	*pFill = NULL;
		Stroke	*pStroke = NULL;
		PolygonSymbolizer *pPolygonSymbolizer = NULL;
		pPolygonSymbolizer = new PolygonSymbolizerImpl();
		pFill = new FillImpl();
		GColor fcolor;
		fcolor.SetAlpha(100);
		pFill->SetColor(fcolor);
		//pFill->GetColor().Set(234,0,32,100);
		//pFill->GetColor().Set(234,0,32,255);
		pPolygonSymbolizer->SetFill(pFill);
		pStroke = new StrokeImpl();;
		pStroke->GetColor().SetAlpha(255);
		pPolygonSymbolizer->SetStroke(pStroke);	
		return pPolygonSymbolizer;
	}

	TextSymbolizer* CreateTextSymbolizer()
	{
		TextSymbolizer *pTextSymbolizer = NULL;
		pTextSymbolizer =  new TextSymbolizerImpl();
		Font *pFont = NULL;
		pFont = new FontImpl();
		pFont->SetFamily("Arial");
		pFont->SetStyle(augeFontStyleItalic);
		pFont->SetSize(15.0f);
		pFont->SetWeight(augeFontWeightBold);
		pTextSymbolizer->SetFont(pFont);
		Fill *pFill = NULL;
		pFill = new FillImpl();
		//	pFill->GetColor().Set(0,255,0,255);
		pTextSymbolizer->SetFill(pFill);
		pTextSymbolizer->SetLabel("Name");
		pTextSymbolizer->SetGeometry("Shape");

		return pTextSymbolizer;
	}
//////////////////////////////////////////////////////////////////////////

}