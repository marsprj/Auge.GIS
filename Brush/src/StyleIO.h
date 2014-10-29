#ifndef __AUGE_STYLE_IO_H__
#define __AUGE_STYLE_IO_H__

#include "Base.h"
#include "Color.h"
#include "TextSymbolizer.h"

#include <libxml/parser.h>
#include <libxml/tree.h>
#include <libxml/xpath.h>

namespace auge
{
	class Style;
	class Rule;
	class Symbolizer;
	class PointSymbolizer;
	class LineSymbolizer;
	class PolygonSymbolizer;
	class TextSymbolizer;
	class Connection;

	class AUGE_API StyleIO
	{	
	public:
		~StyleIO();
	private:
		StyleIO();
	public:
		static StyleIO*	GetInstance();
	private:
		static StyleIO*	m_pInstance;

	public:
		Style*				Read(int sid);
		Style*				Read(const char* szName);
		int					Save(Style* pStyle);
		int					Remove(int sid);
		int					Remove(const char* szName);

	public:
		int					Serialize(Style* pStyle, char** text, int& size);
		Style*				DeSerialize(char* text, int size);

	public:
		int					Initialize();
		void				SetConnection(Connection* pConnection);

	private:
		void				BuildSymbolizer(Symbolizer* pSymbolizer, xmlNodePtr pxmlRule, xmlNsPtr pnsSLD, xmlNsPtr pnsOGC);
		void				BuildFillColor(Color& color, xmlNodePtr pxmlParent, xmlNsPtr pnsSLD);
		void				BuildStrokeColor(Color& color, xmlNodePtr pxmlParent, xmlNsPtr pnsSLD);

		Rule*				ParseRule(xmlNodePtr pxmlRule, xmlDocPtr pxmlDoc);
		PointSymbolizer*	ParsePointSymbolizer(xmlNodePtr pxmlN, xmlDocPtr pxmlDoc);
		LineSymbolizer*		ParseLineSymbolizer(xmlNodePtr pxmlN, xmlDocPtr pxmlDoc);
		PolygonSymbolizer*	ParsePolygonSymbolizer(xmlNodePtr pxmlN, xmlDocPtr pxmlDoc);
		TextSymbolizer*		ParseTextSymbolizer(xmlNodePtr pxmlN, xmlDocPtr pxmlDoc);

		int					ParseFill(PointSymbolizer* s,xmlNodePtr pxmlN, xmlDocPtr pxmlDoc);
		int					ParseStroke(PointSymbolizer* s,xmlNodePtr pxmlN, xmlDocPtr pxmlDoc);
		int					ParseStroke(LineSymbolizer* s,xmlNodePtr pxmlN);
		int					ParseFill(PolygonSymbolizer* s,xmlNodePtr pxmlN);
		int					ParseStroke(PolygonSymbolizer* s,xmlNodePtr pxmlNc);
		int					ParseFill(TextSymbolizer* s,xmlNodePtr pxmlN);
		int					ParseStroke(TextSymbolizer* s,xmlNodePtr pxmlN);
		int					ParseFont(TextSymbolizer* s,xmlNodePtr pxmlN);
		int					ParseLabel(TextSymbolizer* s,xmlNodePtr pxmlN);

		xmlXPathObjectPtr	XSearch(xmlChar* xpath,xmlDocPtr pxmlDoc,xmlNodePtr pxmlCurNode=NULL);

		const char*			EncodeFontStyle(FontStyle s);
		const char*			EncodeFontWeight(FontWeight w);
		FontStyle			ParseFontStyle(const char* s);
		FontWeight			ParseFontWeight(const char* w);

		bool				Initialized();
		void				Cleanup();

	private:
		Connection* m_pConnection;
	};
}

#endif //__AUGE_STYLE_IO_H__
