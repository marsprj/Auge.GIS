#include "MapBrush.h"

using namespace auge;

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
	MapIO* pMapIO = NULL;
	StyleIO*	pStyleIO = NULL;
	Style*		pStyle = NULL;
	Connection *pConnection = NULL;

	int ret = AG_SUCCESS;
	//const char* constr = "hostaddr=192.168.111.159 port=5432 dbname=gisdb user=postgres password=qwer1234";
	const char* constr = "hostaddr=127.0.0.1 port=5432 dbname=gisdb user=postgres password=qwer1234";

	pConnection = new Connection();
	ret = pConnection->Open(constr);
	if(ret!=AG_SUCCESS)
	{
		pConnection->Release();
		return -1;
	}

	pStyleIO = StyleIO::GetInstance();
	pStyleIO->SetConnection(pConnection);
	pStyleIO->Initialize();

	const char* name = "cities";
	pStyle = CreateStylePoint();	
	//pStyle = CreateStyleLine();
	//pStyle = CreateStylePolygon();
	pStyle->SetName(name);

	pStyleIO->Save(pStyle);
	pStyle->Release();

	pStyle = pStyleIO->Read(name);
	pStyle->Release();

	pConnection->Close();
	pConnection->Release();

	return 0;
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
	pRule->SetTextSymbolizer(CreateTextSymbolizer());
	pStyle->AddRule(pRule);

	pRule	= new Rule();
	pSymbolzier = CreateTextSymbolizer();
	pRule->SetSymbolizer(pSymbolzier);
	
	pStyle->AddRule(pRule);

	pRule	= new Rule();
	

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