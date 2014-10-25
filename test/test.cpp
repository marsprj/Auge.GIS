#include "MapBrush.h"

using namespace auge;

void showFeatures(FeatureType* pFeatureType);

Map* CreateMap(Workspace *pWorkspace);
Style* CreateStylePoint();
Style* CreateStyleLine();
Style* CreateStylePolygon();
PointSymbolizer* CreatePointSymbolizer();
LineSymbolizer* CreateLineSymbolizer();
PolygonSymbolizer* CreatePolygonSymbolizer();
TextSymbolizer* CreateTextSymbolizer();

int main()
{	
	Map	*pMap = NULL;
	Workspace *pWorkspace = NULL;
	
	const char* path = "g:\\temp\\map.png";

	int ret = AG_SUCCESS;
	//const char* constr = "hostaddr=192.168.111.159 port=5432 dbname=gisdb user=postgres password=qwer1234";
	const char* constr = "hostaddr=127.0.0.1 port=5432 dbname=gisdb user=postgres password=qwer1234";

	pWorkspace = new Workspace();
	ret = pWorkspace->Open(constr);
	if(ret!=AG_SUCCESS)
	{
		pWorkspace->Release();
		return -1;
	}

	pMap = CreateMap(pWorkspace);

	Canvas canvas(pMap, 1600,1200);
	canvas.Draw(pMap->GetExtent());
	canvas.Save(path);

	pMap->Release();

	pWorkspace->Close();
	pWorkspace->Release();

	return 0;
}

Map* CreateMap(Workspace *pWorkspace)
{
	Map				*pMap = NULL;
	FeatureLayer	*pLayer = NULL;
	FeatureType		*pType = NULL;
	Style			*pStyle = NULL;
	const char		*typeName = NULL;

	pMap = new Map();
	pMap->SetName("world");

	typeName = "cities";
	pType = pWorkspace->OpenFeatureType(typeName);
	//showFeatures(pType);
	//pType->GetFields();
	pLayer = new FeatureLayer();
	pLayer->SetName(typeName);
	pLayer->SetFeatureType(pType);
	pStyle = CreateStylePoint();
	pLayer->SetStyle(pStyle);
	pMap->AddLayer(pLayer);

	typeName = "rivers";
	pType = pWorkspace->OpenFeatureType(typeName);
	//showFeatures(pType);
	//pType->GetFields();
	pLayer = new FeatureLayer();
	pLayer->SetName(typeName);
	pLayer->SetFeatureType(pType);
	pStyle = CreateStyleLine();
	pLayer->SetStyle(pStyle);
	pMap->AddLayer(pLayer);

	typeName = "country";
	pType = pWorkspace->OpenFeatureType(typeName);
	//showFeatures(pType);
	//pType->GetFields();
	pLayer = new FeatureLayer();
	pLayer->SetName(typeName);
	pLayer->SetFeatureType(pType);
	pStyle = CreateStylePolygon();
	pLayer->SetStyle(pStyle);
	pMap->AddLayer(pLayer);

	return pMap;
}

Style* CreateStylePoint()
{
	Style	*pStyle = NULL;
	Rule	*pRule = NULL;
	Symbolizer* pSymbolzier = NULL;

	pStyle	= new Style();
	pRule	= new Rule();
	pSymbolzier = CreatePointSymbolizer();
	pRule->SetSymbolizer(pSymbolzier);
	pStyle->AddRule(pRule);

	pRule	= new Rule();
	pSymbolzier = CreateTextSymbolizer();
	pRule->SetSymbolizer(pSymbolzier);
	pStyle->AddRule(pRule);

	return pStyle;
}

Style* CreateStyleLine()
{
	Style	*pStyle = NULL;
	Rule	*pRule = NULL;
	Symbolizer* pSymbolzier = NULL;

	pStyle	= new Style();
	pRule	= new Rule();
	pSymbolzier = CreateLineSymbolizer();
	pRule->SetSymbolizer(pSymbolzier);
	pStyle->AddRule(pRule);

	return pStyle;
}

Style* CreateStylePolygon()
{
	Style	*pStyle = NULL;
	Rule	*pRule = NULL;
	Symbolizer* pSymbolzier = NULL;

	pStyle	= new Style();
	pRule	= new Rule();
	pSymbolzier = CreatePolygonSymbolizer();
	pRule->SetSymbolizer(pSymbolzier);
	pStyle->AddRule(pRule);

	return pStyle;
}

PointSymbolizer* CreatePointSymbolizer()
{
	PointSymbolizer* pSymbol = NULL;
	pSymbol = new PointSymbolizer();
	pSymbol->radius = 3.f;

	pSymbol->fill = true;
	pSymbol->fill_color = Color(0,255,0,255);

	pSymbol->outline = true;
	pSymbol->outline_color = Color(0,0,0,255);
	pSymbol->outline_width = 2.0f;
	pSymbol->outline_cap = augeRoundCap;
	pSymbol->outline_join = augeRoundJoin;

	return pSymbol;
}

LineSymbolizer* CreateLineSymbolizer()
{
	LineSymbolizer* pSymbol = NULL;
	pSymbol = new LineSymbolizer();
	
	pSymbol->outline_color = Color(0,255,0,255);
	pSymbol->outline_width = 1.0f;
	pSymbol->outline_cap = augeRoundCap;
	pSymbol->outline_join = augeRoundJoin;

	return pSymbol;
}

PolygonSymbolizer* CreatePolygonSymbolizer()
{
	PolygonSymbolizer* pSymbol = NULL;
	pSymbol = new PolygonSymbolizer();

	pSymbol->fill = true;
	pSymbol->fill_color = Color(255,0,0,255);

	pSymbol->outline = true;
	pSymbol->outline_color = Color(0,0,0,255);
	pSymbol->outline_width = 1.0f;
	pSymbol->outline_cap = augeRoundCap;
	pSymbol->outline_join = augeRoundJoin;

	return pSymbol;
}

TextSymbolizer* CreateTextSymbolizer()
{
	TextSymbolizer* pSymbol = NULL;
	pSymbol = new TextSymbolizer();

	pSymbol->fill = true;
	pSymbol->field = "name";
	pSymbol->size = 24;
	pSymbol->color = Color(0,0,255,255);
	pSymbol->slant = augeFontStyleItalic;
	pSymbol->weight = augeFontWeightNormal;

	pSymbol->outline = true;
	pSymbol->outline_color = Color(0,255,0,255);
	pSymbol->outline_width = 0.5f;

	pSymbol->offset_x = 0.0f;
	pSymbol->offset_y = -10.0f;

	return pSymbol;
}

void showFeatures(FeatureType* pFeatureType)
{
	byte* wkb = NULL;
	FeatureSet* pSet = NULL;
	pSet = pFeatureType->Query();

	Envelope& extent = pFeatureType->GetExtent();

	pSet->Reset();
	while(!pSet->IsEOF())
	{
		Feature& f = pSet->Next();
		std::vector<Field*>& fields = f.GetFields();
		int num = fields.size()-1;
		for(int i=0; i<num; i++)
		{
			char* sz = f.GetValueAsString(i);
			printf("%s\n",sz);
		}

		printf("------------------------------------------------------------\n");
		//byte* wkb = f.GetBytes(9);
	}

	pSet->Release();
}