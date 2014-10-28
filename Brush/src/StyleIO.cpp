#include "StyleIO.h"
#include "Workspace.h"
#include "Style.h"
#include "Rule.h"
#include "PointSymbolizer.h"
#include "LineSymbolizer.h"
#include "PolygonSymbolizer.h"
#include "TextSymbolizer.h"

#include <sstream>
#include <libpq-fe.h>

#include <libxml/parser.h>
#include <libxml/tree.h>
#include <libxml/xpath.h>

namespace auge
{
	StyleIO* StyleIO::m_pInstance = new StyleIO();
	StyleIO* StyleIO::GetInstance()
	{		
		return m_pInstance;
	}

	StyleIO::StyleIO():
	m_pConnection(NULL)
	{

	}

	StyleIO::~StyleIO()
	{
		if(m_pInstance!=NULL)
		{
			delete m_pInstance;
			m_pInstance = NULL;
		}
	}

	Style* StyleIO::Read(int sid)
	{
		if(sid<0)
		{
			return NULL;
		}

		char sql[PATH_MAX] = {0};
		sprintf(sql, "select id,name,style from ag_style where id=%d", sid);

		PGresult* pgResult = NULL;
		pgResult = PQexec(m_pConnection->m_pgConnection, sql);
		if(PQresultStatus(pgResult)!=PGRES_TUPLES_OK)
		{
			PQclear(pgResult);
			return NULL;
		}

		if(PQntuples(pgResult)==0)
		{
			PQclear(pgResult);
			return NULL;
		}

		int id = atoi(PQgetvalue(pgResult,0,0));
		char* text = PQgetvalue(pgResult,0,2);
		Style* pStyle = NULL;
		pStyle = DeSerialize(text,strlen(text));
		if(pStyle!=NULL)
		{
			pStyle->SetID(id);
		}

		PQclear(pgResult);

		return pStyle;
	}

	Style* StyleIO::Read(const char* szName)
	{
		if(szName==NULL)
		{
			return NULL;
		}
		
		char sql[PATH_MAX] = {0};
		sprintf(sql, "select id,name,style from ag_style where name='%s'", szName);

		PGresult* pgResult = NULL;
		pgResult = PQexec(m_pConnection->m_pgConnection, sql);
		if(PQresultStatus(pgResult)!=PGRES_TUPLES_OK)
		{
			PQclear(pgResult);
			return NULL;
		}

		if(PQntuples(pgResult)==0)
		{
			PQclear(pgResult);
			return NULL;
		}
		int id = atoi(PQgetvalue(pgResult,0,0));
		char* text = PQgetvalue(pgResult,0,2);
		Style* pStyle = NULL;
		pStyle = DeSerialize(text,strlen(text));
		if(pStyle!=NULL)
		{
			pStyle->SetID(id);
		}

		PQclear(pgResult);

		return pStyle;
	}

	int StyleIO::Save(Style* pStyle)
	{
		char*  text = NULL;
		int    size = 0;
		int ret = AG_FAILURE;

		ret = Serialize(pStyle, &text, size);
		if(ret!=AG_SUCCESS)
		{
			return ret;
		}

		std::string sql;
		std::stringstream ss;
		ss<<"insert into ag_style(name, style) values('"<<pStyle->GetName()<<"','"<<text<<"') returning id";
		
		PGresult* pgResult = NULL;
		pgResult = PQexec(m_pConnection->m_pgConnection, ss.str().c_str());
		if(PQresultStatus(pgResult)!=PGRES_TUPLES_OK)
		{
			PQclear(pgResult);
			return AG_FAILURE;
		}
		int id = atoi(PQgetvalue(pgResult,0,0));
		pStyle->SetID(id);
		PQclear(pgResult);
		return AG_SUCCESS;
	}

	int	StyleIO::Remove(int sid)
	{
		return AG_FAILURE;
	}

	int	StyleIO::Remove(const char* szName)
	{
		return AG_FAILURE;
	}

	int	StyleIO::Serialize(Style* pStyle, char** text, int& size)
	{
		
		xmlDocPtr	pxmlDoc = NULL;
		xmlNodePtr	pxmlRoot= NULL;
		xmlNsPtr	pnsR = NULL;
		xmlNsPtr	pnsSLD = NULL;
		xmlNsPtr	pnsOGC = NULL;
		xmlNsPtr	pnsGML = NULL;

		const char* sld_version = "1.0.0";
		const char* encoding = "UTF-8";

		// create namespace
		//pnsR   = xmlNewNs(NULL, BAD_CAST"http://www.opengis.net/sld", NULL);
		//pnsSLD = xmlNewNs(NULL, BAD_CAST"http://www.opengis.net/sld", BAD_CAST"sld");
		//pnsOGC = xmlNewNs(NULL, BAD_CAST"http://www.opengis.net/ogc", BAD_CAST"ogc");
		//pnsGML = xmlNewNs(NULL, BAD_CAST"http://www.opengis.net/gml", BAD_CAST"gml");

		// create xml doc
		pxmlDoc = xmlNewDoc(BAD_CAST"1.0");

		// create xml root element
		pxmlRoot = xmlNewNode(pnsR, BAD_CAST"StyledLayerDescriptor");
		xmlSetNs(pxmlRoot, pnsOGC);
		xmlSetNs(pxmlRoot, pnsGML);
		xmlSetNs(pxmlRoot, pnsSLD);
		xmlSetProp(pxmlRoot, BAD_CAST"version", BAD_CAST sld_version);
		xmlDocSetRootElement(pxmlDoc, pxmlRoot);
		//User Layer
		xmlNodePtr pxmlULayer = NULL;
		pxmlULayer = xmlNewChild(pxmlRoot,pnsSLD,BAD_CAST"UserLayer",NULL);
		xmlNodePtr pxmlContraints = NULL;
		pxmlContraints = xmlNewChild(pxmlULayer,pnsSLD,BAD_CAST"LayerFeatureConstraints",NULL);
		xmlNewChild(pxmlContraints,pnsSLD,BAD_CAST"LayerFeatureConstraint",NULL);
		//User Style
		xmlNodePtr pxmlUStyle = NULL;
		pxmlUStyle = xmlNewChild(pxmlULayer,pnsSLD,BAD_CAST"UserStyle",NULL);
		xmlNewChild(pxmlUStyle,pnsSLD,BAD_CAST"Name",BAD_CAST pStyle->GetName());
		xmlNewChild(pxmlUStyle,pnsSLD,BAD_CAST"Title",BAD_CAST pStyle->GetName());
		//Feature Style
		xmlNodePtr pxmlFtStyle = NULL;
		pxmlFtStyle = xmlNewChild(pxmlUStyle,pnsSLD,BAD_CAST"FeatureTypeStyle",NULL);
		xmlNewChild(pxmlFtStyle,pnsSLD,BAD_CAST"Name",BAD_CAST pStyle->GetName());
		xmlNewChild(pxmlFtStyle,pnsSLD,BAD_CAST"FeatureTypeName",BAD_CAST NULL);
		xmlNewChild(pxmlFtStyle,pnsSLD,BAD_CAST"SemanticTypeIdentifier",BAD_CAST NULL);
		xmlNewChild(pxmlFtStyle,pnsSLD,BAD_CAST"SemanticTypeIdentifier",BAD_CAST NULL);

		xmlNodePtr pxmlRule = NULL;
		xmlNodePtr pxmlFilter = NULL;
		
		Rule *r = NULL;
		Symbolizer *s = NULL;
		std::vector<Rule*>::iterator iter;
		std::vector<Rule*>& rules = pStyle->GetRules();
		for(iter=rules.begin(); iter!=rules.end(); iter++)
		{
			r = *iter;
			pxmlRule = xmlNewChild(pxmlFtStyle, pnsSLD, BAD_CAST"Rule", NULL);
			xmlNewChild(pxmlRule, pnsSLD, BAD_CAST"Name", NULL);
			xmlNewChild(pxmlRule, pnsSLD, BAD_CAST"Title", NULL);

			pxmlFilter = xmlNewChild(pxmlRule, pnsOGC, BAD_CAST"Filter", NULL);
			
			s = r->GetSymbolizer();
			BuildSymbolizer(s, pxmlRule, pnsSLD, pnsOGC);
			s = r->GetTextSymbolizer();
			if(s!=NULL)
			{
				BuildSymbolizer(s, pxmlRule, pnsSLD, pnsOGC);
			}
		}

		xmlDocDumpFormatMemoryEnc(pxmlDoc, (xmlChar **)text, &size, encoding, /*1*/1);
#ifdef _DEBUG
		printf("%s", *text);
#endif
		xmlSaveFile("g:\\temp\\style.xml",pxmlDoc);

		xmlFreeNs(pnsR);
		xmlFreeNs(pnsSLD);
		xmlFreeNs(pnsOGC);
		xmlFreeNs(pnsGML);
		xmlFreeDoc(pxmlDoc);
		xmlMemoryDump();

		return AG_SUCCESS;
	}

	void StyleIO::BuildSymbolizer(Symbolizer* pSymbolizer, xmlNodePtr pxmlRule, xmlNsPtr pnsSLD, xmlNsPtr pnsOGC)
	{
		char str[NAME_MAX];
		switch(pSymbolizer->GetType())
		{
		case augeSymbolPoint:
			{
				xmlNodePtr pxmlSymbol = NULL;
				pxmlSymbol = xmlNewChild(pxmlRule, pnsSLD, BAD_CAST"PointSymbolizer", NULL);
				xmlNodePtr pxmlGraphic = NULL;
				pxmlGraphic = xmlNewChild(pxmlSymbol, pnsSLD, BAD_CAST"Graphic", NULL);
				xmlNodePtr pxmlMark = NULL;
				pxmlMark = xmlNewChild(pxmlGraphic, pnsSLD, BAD_CAST"Mark", NULL);
				
				PointSymbolizer* s = static_cast<PointSymbolizer*>(pSymbolizer);
				if(s->fill)
				{
					xmlNodePtr pxmlFill = NULL;
					pxmlFill = xmlNewChild(pxmlMark, pnsSLD, BAD_CAST"Fill", NULL);
					xmlNodePtr pxmlParam = NULL;
					// color
					BuildFillColor(s->fill_color, pxmlFill, pnsSLD);
				}
				if(s->outline)
				{
					xmlNodePtr pxmlStroke = NULL;
					pxmlStroke = xmlNewChild(pxmlMark, pnsSLD, BAD_CAST"Stroke", NULL);
					xmlNodePtr pxmlParam = NULL;
					// color
					BuildStrokeColor(s->outline_color, pxmlStroke, pnsSLD);
					//width
					ag_snprintf(str,NAME_MAX,"%f",s->outline_width);
					pxmlParam = xmlNewChild(pxmlStroke, pnsSLD, BAD_CAST"CssParameter", BAD_CAST str);
					xmlNewProp(pxmlParam, BAD_CAST"name",BAD_CAST"stroke-width");
				}
				//size
				xmlNewChild(pxmlMark, pnsSLD, BAD_CAST"Size", BAD_CAST"6.0");
			}
			break;
		case augeSymbolLine:
			{
				LineSymbolizer* s = static_cast<LineSymbolizer*>(pSymbolizer);

				xmlNodePtr pxmlSymbol = NULL;
				pxmlSymbol = xmlNewChild(pxmlRule, pnsSLD, BAD_CAST"LineSymbolizer", NULL);

				xmlNodePtr pxmlStroke = NULL;
				pxmlStroke = xmlNewChild(pxmlSymbol, pnsSLD, BAD_CAST"Stroke", NULL);
				xmlNodePtr pxmlParam = NULL;
				// color
				BuildStrokeColor(s->outline_color, pxmlStroke, pnsSLD);
				//width
				ag_snprintf(str,NAME_MAX,"%f",s->outline_width);
				pxmlParam = xmlNewChild(pxmlStroke, pnsSLD, BAD_CAST"CssParameter", BAD_CAST str);
				xmlNewProp(pxmlParam, BAD_CAST"name",BAD_CAST"stroke-width");
			}
			break;
		case augeSymbolPolygon:
			{
				PolygonSymbolizer* s = static_cast<PolygonSymbolizer*>(pSymbolizer);

				xmlNodePtr pxmlSymbol = NULL;
				pxmlSymbol = xmlNewChild(pxmlRule, pnsSLD, BAD_CAST"PolygonSymbolizer", NULL);

				if(s->fill)
				{
					xmlNodePtr pxmlFill = NULL;
					pxmlFill = xmlNewChild(pxmlSymbol, pnsSLD, BAD_CAST"Fill", NULL);
					xmlNodePtr pxmlParam = NULL;
					// color
					BuildFillColor(s->fill_color, pxmlFill, pnsSLD);
					//
				}
				if(s->outline)
				{
					xmlNodePtr pxmlStroke = NULL;
					pxmlStroke = xmlNewChild(pxmlSymbol, pnsSLD, BAD_CAST"Stroke", NULL);
					xmlNodePtr pxmlParam = NULL;
					// color
					BuildStrokeColor(s->outline_color, pxmlStroke, pnsSLD);
					// width
					ag_snprintf(str,NAME_MAX,"%f",s->outline_width);
					pxmlParam = xmlNewChild(pxmlStroke, pnsSLD, BAD_CAST"CssParameter", BAD_CAST str);
					xmlNewProp(pxmlParam, BAD_CAST"name",BAD_CAST"stroke-width");
				}
			}
			break;
		case augeSymbolText:
			{
				TextSymbolizer* s = static_cast<TextSymbolizer*>(pSymbolizer);

				xmlNodePtr pxmlSymbol = NULL;
				xmlNodePtr pxmlParam = NULL;

				pxmlSymbol = xmlNewChild(pxmlRule, pnsSLD, BAD_CAST"TextSymbolizer", NULL);
				// label filed
				xmlNodePtr pxmlLabel = NULL;
				pxmlLabel = xmlNewChild(pxmlSymbol, pnsSLD, BAD_CAST"Label", NULL);
				xmlNewChild(pxmlLabel, pnsOGC, BAD_CAST"PropertyName", BAD_CAST s->field.c_str());
				// font
				xmlNodePtr pxmlFont = NULL;
				pxmlFont = xmlNewChild(pxmlSymbol, pnsSLD, BAD_CAST"Font", NULL);
				// font-family
				pxmlParam = xmlNewChild(pxmlFont, pnsSLD, BAD_CAST"CssParameter", BAD_CAST s->font.c_str());
				xmlNewProp(pxmlParam, BAD_CAST"name",BAD_CAST"font-family");
				// font-size
				ag_snprintf(str,NAME_MAX,"%f",(float)s->size);
				pxmlParam = xmlNewChild(pxmlFont, pnsSLD, BAD_CAST"CssParameter", BAD_CAST str);
				xmlNewProp(pxmlParam, BAD_CAST"name",BAD_CAST"font-size");
				// font-style
				pxmlParam = xmlNewChild(pxmlFont, pnsSLD, BAD_CAST"CssParameter", BAD_CAST EncodeFontStyle(s->slant));
				xmlNewProp(pxmlParam, BAD_CAST"name",BAD_CAST"font-style");
				// font-weight
				pxmlParam = xmlNewChild(pxmlFont, pnsSLD, BAD_CAST"CssParameter", BAD_CAST EncodeFontWeight(s->weight));
				xmlNewProp(pxmlParam, BAD_CAST"name",BAD_CAST"font-weight");

				xmlNodePtr pxmlFill = NULL;
				pxmlFill = xmlNewChild(pxmlSymbol, pnsSLD, BAD_CAST"Fill", NULL);
				if(s->fill)
				{
					BuildFillColor(s->color, pxmlFill, pnsSLD);
				}
				xmlNodePtr pxmlStroke = NULL;
				pxmlStroke = xmlNewChild(pxmlSymbol, pnsSLD, BAD_CAST"Stroke", NULL);
				if(s->outline)
				{	
					// color
					BuildStrokeColor(s->outline_color, pxmlStroke, pnsSLD);
					// width
					ag_snprintf(str,NAME_MAX,"%f",s->outline_width);
					pxmlParam = xmlNewChild(pxmlStroke, pnsSLD, BAD_CAST"CssParameter", BAD_CAST str);
					xmlNewProp(pxmlParam, BAD_CAST"name",BAD_CAST"stroke-width");
				}
			}
			break;
		}
	}


	void StyleIO::BuildFillColor(Color& color, xmlNodePtr pxmlParent, xmlNsPtr pnsSLD)
	{
		char str[NAME_MAX];
		xmlNodePtr pxmlParam = NULL;
		ag_snprintf(str,NAME_MAX,"#%02x%02x%02x",(int)color.GetRed(),(int)color.GetGreen(),(int)color.GetBlue());
		pxmlParam = xmlNewChild(pxmlParent, pnsSLD, BAD_CAST"CssParameter", BAD_CAST str);
		xmlNewProp(pxmlParam, BAD_CAST"name",BAD_CAST"fill");
		// alpha
		ag_snprintf(str,NAME_MAX,"%f",color.GetAlpha()/255.0f);
		pxmlParam = xmlNewChild(pxmlParent, pnsSLD, BAD_CAST"CssParameter", BAD_CAST str);
		xmlNewProp(pxmlParam, BAD_CAST"name",BAD_CAST"fill-opacity");
	}

	void StyleIO::BuildStrokeColor(Color& color, xmlNodePtr pxmlParent, xmlNsPtr pnsSLD)
	{
		char str[NAME_MAX];
		xmlNodePtr pxmlParam = NULL;
		// color
		ag_snprintf(str,NAME_MAX,"#%02x%02x%02x",(int)color.GetRed(),(int)color.GetGreen(),(int)color.GetBlue());
		pxmlParam = xmlNewChild(pxmlParent, pnsSLD, BAD_CAST"CssParameter", BAD_CAST str);
		xmlNewProp(pxmlParam, BAD_CAST"name",BAD_CAST"stroke");
		// alpha
		ag_snprintf(str,NAME_MAX,"%f",color.GetAlpha()/255.0f);
		pxmlParam = xmlNewChild(pxmlParent, pnsSLD, BAD_CAST"CssParameter", BAD_CAST str);
		xmlNewProp(pxmlParam, BAD_CAST"name",BAD_CAST"stroke-opacity");
	}

	Style* StyleIO::DeSerialize(char* text, int size)
	{
		xmlDocPtr pxmlDoc = NULL;

		xmlKeepBlanksDefault(0);
		pxmlDoc = xmlParseMemory(text, size);
		if(pxmlDoc==NULL)
		{
			return NULL;
		}

		xmlChar *xpath = BAD_CAST("//Rule");
		xmlXPathContextPtr	pxmlCtx = NULL;   
		xmlXPathObjectPtr	pxmlXRules = NULL;
		xmlNodeSetPtr		pxmlRules = NULL;
		xmlNodePtr			pxmlRule = NULL;

		pxmlCtx= xmlXPathNewContext(pxmlDoc);
		pxmlXRules = xmlXPathEvalExpression(xpath, pxmlCtx);
		xmlXPathFreeContext(pxmlCtx);
		if(pxmlXRules==NULL)
		{
			xmlFreeDoc(pxmlDoc);
			return NULL;
		}

		Rule* r = NULL;
		Style* s = new Style();
		pxmlRules = pxmlXRules->nodesetval;
		for(int i=0; i<pxmlRules->nodeNr; i++)
		{
			pxmlRule = pxmlRules->nodeTab[i];
			r = ParseRule(pxmlRule, pxmlDoc);
			if(r!=NULL)
			{
				s->AddRule(r);
			}
		}

		xmlXPathFreeObject(pxmlXRules); 
		xmlFreeDoc(pxmlDoc);

		return s;
	}

	Rule* StyleIO::ParseRule(xmlNodePtr pxmlRule, xmlDocPtr pxmlDoc)
	{
		Rule *r = new Rule();
		Symbolizer* s = NULL;
		xmlNodePtr pxmlN = pxmlRule->children;
		while(pxmlN != NULL)
		{
			if(ag_stricmp((char*)pxmlN->name, "PointSymbolizer")==0)
			{
				s = ParsePointSymbolizer(pxmlN, pxmlDoc);
				if(s!=NULL)
					r->SetSymbolizer(s);
			}
			else if(ag_stricmp((char*)pxmlN->name, "LineSymbolizer")==0)
			{
				s = ParseLineSymbolizer(pxmlN, pxmlDoc);
				if(s!=NULL)
					r->SetSymbolizer(s);
			}
			else if(ag_stricmp((char*)pxmlN->name, "PolygonSymbolizer")==0)
			{
				s = ParsePolygonSymbolizer(pxmlN, pxmlDoc);
				if(s!=NULL)
					r->SetSymbolizer(s);
			}
			else if(ag_stricmp((char*)pxmlN->name, "TextSymbolizer")==0)
			{
				s = ParseTextSymbolizer(pxmlN, pxmlDoc);
				if(r->GetSymbolizer()==NULL)
					r->SetSymbolizer(s);
				else
					r->SetTextSymbolizer(static_cast<TextSymbolizer*>(s));
			}

			pxmlN = pxmlN->next;
		}
		return r;
	}

	PointSymbolizer* StyleIO::ParsePointSymbolizer(xmlNodePtr pxmlN, xmlDocPtr pxmlDoc)
	{
		PointSymbolizer* s = new PointSymbolizer();
		ParseFill(s,pxmlN, pxmlDoc);
		ParseStroke(s,pxmlN, pxmlDoc);
		return s;
	}

	LineSymbolizer* StyleIO::ParseLineSymbolizer(xmlNodePtr pxmlN, xmlDocPtr pxmlDoc)
	{
		LineSymbolizer* s = new LineSymbolizer();
		xmlNodePtr nc = pxmlN->children;
		while(nc!=NULL)
		{
			if(ag_stricmp((char*)nc->name, "stroke")==0)
			{
				ParseStroke(s,nc);
			}

			nc = nc->next;
		}
		return s;
	}

	PolygonSymbolizer* StyleIO::ParsePolygonSymbolizer(xmlNodePtr pxmlN, xmlDocPtr pxmlDoc)
	{
		PolygonSymbolizer* s = new PolygonSymbolizer();
		xmlNodePtr nc = pxmlN->children;
		while(nc!=NULL)
		{
			if(ag_stricmp((char*)nc->name, "fill")==0)
			{
				ParseFill(s,nc);
			}
			else if(ag_stricmp((char*)nc->name, "stroke")==0)
			{
				ParseStroke(s,nc);
			}

			nc = nc->next;
		}
		return s;
	}

	TextSymbolizer* StyleIO::ParseTextSymbolizer(xmlNodePtr pxmlN, xmlDocPtr pxmlDoc)
	{
		TextSymbolizer* s = new TextSymbolizer();
		xmlNodePtr nc = pxmlN->children;
		while(nc!=NULL)
		{
			if(ag_stricmp((char*)nc->name, "fill")==0)
			{
				ParseFill(s,nc);
			}
			else if(ag_stricmp((char*)nc->name, "stroke")==0)
			{
				ParseStroke(s,nc);
			}
			else if(ag_stricmp((char*)nc->name, "font")==0)
			{
				ParseFont(s,nc);
			}
			else if(ag_stricmp((char*)nc->name, "Label")==0)
			{
				ParseLabel(s,nc);
			}

			nc = nc->next;
		}
		return s;
	}

	int StyleIO::ParseFill(PointSymbolizer* s,xmlNodePtr pxmlN, xmlDocPtr pxmlDoc)
	{
		xmlXPathContextPtr	pxmlCtx = NULL;   
		xmlXPathObjectPtr	pxmlXRules = NULL;

		char xpath[NAME_MAX];
		sprintf(xpath, "//%s//Fill",pxmlN->name);
		xmlXPathObjectPtr pxmlXObj = XSearch(BAD_CAST xpath,pxmlDoc,pxmlN);
		if(xmlXPathNodeSetIsEmpty(pxmlXObj->nodesetval))
		{
			s->fill = false;
			return AG_FAILURE;
		}

		s->fill = true;
		const char* name  = NULL;
		const char* value = NULL;
		xmlNodePtr nf = pxmlXObj->nodesetval->nodeTab[0];
		xmlNodePtr nc = nf->children;
		int r=0,g=0,b=0,a=255;
		while(nc!=NULL)
		{			
			name  = (const char*)xmlGetProp(nc, BAD_CAST"name");
			if(name!=NULL)
			{
				value = (const char*)xmlNodeGetContent(nc);
				if(ag_stricmp(name, "fill")==0)
				{	
					sscanf(value+1,"%2x%2x%2x",&r,&g,&b);
				}
				else if(ag_stricmp(name, "fill-opacity")==0)
				{
					a = (int)(atof(value) * 255);
				}
			}			
			nc = nc->next;
		}
		s->fill_color = Color((byte)r,(byte)g,(byte)b,(byte)a);
		byte ss = s->fill_color.GetGreen();

		return AG_SUCCESS;
	}

	int StyleIO::ParseStroke(PointSymbolizer* s,xmlNodePtr pxmlN, xmlDocPtr pxmlDoc)
	{
		xmlXPathContextPtr	pxmlCtx = NULL;   
		xmlXPathObjectPtr	pxmlXRules = NULL;


		char xpath[NAME_MAX];
		sprintf(xpath, "//%s//Stroke",pxmlN->name);
		xmlXPathObjectPtr pxmlXObj = XSearch(BAD_CAST xpath,pxmlDoc,pxmlN);
		if(xmlXPathNodeSetIsEmpty(pxmlXObj->nodesetval))
		{
			s->outline = false;
			return AG_FAILURE;
		}
		s->outline = true;
		const char* name  = NULL;
		const char* value = NULL;		
		xmlNodePtr nf = pxmlXObj->nodesetval->nodeTab[0];
		xmlNodePtr nc = nf->children;
		int r=0,g=0,b=0,a=255;
		while(nc!=NULL)
		{
			
			name  = (const char*)xmlGetProp(nc, BAD_CAST"name");
			if(name!=NULL)
			{
				value = (const char*)xmlNodeGetContent(nc);
				if(ag_stricmp(name, "stroke")==0)
				{	
					sscanf(value+1,"%2x%2x%2x",&r,&g,&b);
				}
				else if(ag_stricmp(name, "stroke-opacity")==0)
				{
					a = (int)(atof(value) * 255);
				}
				else if(ag_stricmp(name, "stroke-width")==0)
				{
					double width = atof(value);
					s->outline_width = width;
				}
			}
			nc = nc->next;
		}
		s->outline_color = Color((byte)r,(byte)g,(byte)b,(byte)a);

		return AG_SUCCESS;
	}

	int	StyleIO::ParseStroke(LineSymbolizer* s,xmlNodePtr pxmlN)
	{
		const char* name  = NULL;
		const char* value = NULL;
		xmlNodePtr nc = pxmlN->children;
		int r=0,g=0,b=0,a=255;
		while(nc!=NULL)
		{
			name  = (const char*)xmlGetProp(nc, BAD_CAST"name");
			if(name!=NULL)
			{
				value = (const char*)xmlNodeGetContent(nc);
				if(ag_stricmp(name, "stroke")==0)
				{	
					sscanf(value+1,"%2x%2x%2x",&r,&g,&b);
				}
				else if(ag_stricmp(name, "stroke-opacity")==0)
				{
					a = (int)(atof(value) * 255);
				}
				else if(ag_stricmp(name, "stroke-width")==0)
				{
					double width = atof(value);
					s->outline_width = width;
				}
			}
			nc = nc->next;
		}
		s->outline_color = Color((byte)r,(byte)g,(byte)b,(byte)a);

		return AG_SUCCESS;
	}

	int	StyleIO::ParseFill(PolygonSymbolizer* s,xmlNodePtr pxmlN)
	{
		const char* name  = NULL;
		const char* value = NULL;
		xmlNodePtr nc = pxmlN->children;
		int r=0,g=0,b=0,a=255;
		while(nc!=NULL)
		{	
			name  = (const char*)xmlGetProp(nc, BAD_CAST"name");
			if(name!=NULL)
			{
				value = (const char*)xmlNodeGetContent(nc);
				if(ag_stricmp(name, "fill")==0)
				{	
					sscanf(value+1,"%2x%2x%2x",&r,&g,&b);
				}
				else if(ag_stricmp(name, "fill-opacity")==0)
				{
					a = (int)(atof(value) * 255);
				}
			}
			nc = nc->next;
		}
		s->fill_color = Color((byte)r,(byte)g,(byte)b,(byte)a);
		s->fill = true;
		return AG_SUCCESS;
	}

	int	StyleIO::ParseStroke(PolygonSymbolizer* s,xmlNodePtr pxmlN)
	{
		
		const char* name  = NULL;
		const char* value = NULL;
		xmlNodePtr nc = pxmlN->children;
		int r=0,g=0,b=0,a=255;
		while(nc!=NULL)
		{	
			name  = (const char*)xmlGetProp(nc, BAD_CAST"name");
			if(name!=NULL)
			{
				value = (const char*)xmlNodeGetContent(nc);
				if(ag_stricmp(name, "stroke")==0)
				{	
					sscanf(value+1,"%2x%2x%2x",&r,&g,&b);
				}
				else if(ag_stricmp(name, "stroke-opacity")==0)
				{
					a = (int)(atof(value) * 255);
				}
				else if(ag_stricmp(name, "stroke-width")==0)
				{
					double width = atof(value);
					s->outline_width = width;
				}
			}
			nc = nc->next;
		}
		s->outline_color = Color((byte)r,(byte)g,(byte)b,(byte)a);
		s->outline = true;

		return AG_SUCCESS;
	}

	int	StyleIO::ParseFill(TextSymbolizer* s,xmlNodePtr pxmlN)
	{
		const char* name  = NULL;
		const char* value = NULL;
		xmlNodePtr nc = pxmlN->children;
		int r=0,g=0,b=0,a=255;
		while(nc!=NULL)
		{
			name  = (const char*)xmlGetProp(nc, BAD_CAST"name");
			if(name!=NULL)
			{
				value = (const char*)xmlNodeGetContent(nc);
				if(ag_stricmp(name, "fill")==0)
				{	
					sscanf(value+1,"%2x%2x%2x",&r,&g,&b);
				}
				else if(ag_stricmp(name, "fill-opacity")==0)
				{
					a = (int)(atof(value) * 255);
				}
			}
			
			nc = nc->next;
		}
		s->color = Color((byte)r,(byte)g,(byte)b,(byte)a);
		s->fill = true;

		return AG_SUCCESS;
	}

	int	StyleIO::ParseStroke(TextSymbolizer* s,xmlNodePtr pxmlN)
	{
		s->outline = true;
		const char* name  = NULL;
		const char* value = NULL;
		xmlNodePtr nc = pxmlN->children;
		int r=0,g=0,b=0,a=255;
		while(nc!=NULL)
		{	
			name  = (const char*)xmlGetProp(nc, BAD_CAST"name");
			if(name!=NULL)
			{
				value = (const char*)xmlNodeGetContent(nc);
				if(ag_stricmp(name, "stroke")==0)
				{	
					sscanf(value+1,"%2x%2x%2x",&r,&g,&b);
				}
				else if(ag_stricmp(name, "stroke-opacity")==0)
				{
					a = (int)(atof(value) * 255);
				}
				else if(ag_stricmp(name, "stroke-width")==0)
				{
					double width = atof(value);
					s->outline_width = width;
				}
			}
			nc = nc->next;
		}
		s->outline_color = Color((byte)r,(byte)g,(byte)b,(byte)a);

		return AG_SUCCESS;
	}

	int	StyleIO::ParseFont(TextSymbolizer* s,xmlNodePtr pxmlN)
	{
		const char* name  = NULL;
		const char* value = NULL;
		xmlNodePtr nc = pxmlN->children;
		int r=0,g=0,b=0,a=255;
		while(nc!=NULL)
		{	
			name  = (const char*)xmlGetProp(nc, BAD_CAST"name");
			if(name!=NULL)
			{
				value = (const char*)xmlNodeGetContent(nc);
				if(ag_stricmp(name, "font-family")==0)
				{	
					s->font = value;
				}
				else if(ag_stricmp(name, "font-size")==0)
				{
					s->size = atoi(value);
				}
				else if(ag_stricmp(name, "font-style")==0)
				{
					s->slant = ParseFontStyle(value);
				}
				else if(ag_stricmp(name, "font-weight")==0)
				{
					s->weight = ParseFontWeight(value);
				}
			}
			
			nc = nc->next;
		}
		s->color = Color((byte)r,(byte)g,(byte)b,(byte)a);

		return AG_SUCCESS;
	}

	int	StyleIO::ParseLabel(TextSymbolizer* s,xmlNodePtr pxmlN)
	{
		const char* name  = NULL;
		const char* value = NULL;
		xmlNodePtr nc = pxmlN->children;
		if(nc!=NULL)
		{
			if(ag_stricmp((const char*)nc->name,"PropertyName")==0)
			{
				value = (char*)xmlNodeGetContent(nc);
				if(value!=NULL)
				{
					s->field = value;
				}
			}
		}
		
		return AG_SUCCESS;
	}

	xmlXPathObjectPtr StyleIO::XSearch(xmlChar* xpath,xmlDocPtr pxmlDoc, xmlNodePtr pxmlCurNode)
	{
		xmlXPathContextPtr	pxmlCtx = NULL;   
		xmlXPathObjectPtr	pxmlXObject = NULL;
		xmlNodeSetPtr		pxmlRules = NULL;
		xmlNodePtr			pxmlRule = NULL;

		Rule* r = NULL;

		pxmlCtx= xmlXPathNewContext(pxmlDoc);
		pxmlCtx->node = pxmlCurNode;
		pxmlXObject = xmlXPathEvalExpression(xpath, pxmlCtx);
		xmlXPathFreeContext(pxmlCtx);
		return pxmlXObject;
	}

	int	StyleIO::Initialize()
	{
		PGresult* pgResult = NULL;
		const char* sql_style = "create table ag_style (id serial, name character varying(32), style character varying(8192), constraint sid_uk unique(id), constraint sn_uk unique(name))";

		pgResult = PQexec(m_pConnection->m_pgConnection, sql_style);
		if(PQresultStatus(pgResult)!=PGRES_COMMAND_OK)
		{
			//PQclear(pgResult);
			//return AG_FAILURE;
		}
		PQclear(pgResult);

		return AG_SUCCESS;
	}

	bool StyleIO::Initialized()
	{
		const char* sql = "select oid from pg_class where realname='ag_style'";
		PGresult* pgResult = NULL;
		pgResult = PQexec(m_pConnection->m_pgConnection, sql);
		if(PQresultStatus(pgResult)!=PGRES_TUPLES_OK)
		{
			PQclear(pgResult);
			return false;
		}
		int n = PQntuples(pgResult);
		PQclear(pgResult);
		return (n>0);
	}

	void StyleIO::SetConnection(Workspace* pConnection)
	{
		m_pConnection = pConnection;
	}

	const char* StyleIO::EncodeFontStyle(FontStyle s)
	{
		const char* sz = "";
		switch(s)
		{
		case augeFontStyleNormal:
			sz = "normal";
			break;
		case augeFontStyleItalic:
			sz = "italic";
			break;
		case augeFontStyleOblique:
			sz = "oblique";
			break;
		}
		return sz;
	}

	const char* StyleIO::EncodeFontWeight(FontWeight w)
	{
		const char* sz = "";
		switch(w)
		{
		case augeFontWeightNormal:
			sz = "normal";
			break;
		case augeFontWeightBold:
			sz = "bold";
			break;
		}
		return sz;
	}

	FontStyle StyleIO::ParseFontStyle(const char* s)
	{
		if(s==NULL)
		{
			return augeFontStyleNormal;
		}

		FontStyle ret = augeFontStyleNormal;
		if(ag_stricmp(s,"Italic")==0)
		{
			ret = augeFontStyleItalic;
		}
		else if(ag_stricmp(s, "Oblique")==0)
		{
			ret = augeFontStyleOblique;
		}
		return ret;
	}

	FontWeight StyleIO::ParseFontWeight(const char* w)
	{
		if(w==NULL)
		{
			return augeFontWeightNormal;
		}
		if(ag_stricmp(w,"bold")==0)
		{
			return augeFontWeightBold;
		}
		return augeFontWeightNormal;
	}
}