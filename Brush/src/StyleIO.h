#ifndef __AUGE_STYLE_IO_H__
#define __AUGE_STYLE_IO_H__

#include "Base.h"
#include "Color.h"
#include "TextSymbolizer.h"

#include <libxml/parser.h>
#include <libxml/tree.h>

namespace auge
{
	class Style;
	class Symbolizer;
	class Workspace;

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
		void				SetConnection(Workspace* pConnection);

	private:
		void				BuildSymbolizer(Symbolizer* pSymbolizer, xmlNodePtr pxmlRule, xmlNsPtr pnsSLD, xmlNsPtr pnsOGC);
		void				BuildFillColor(Color& color, xmlNodePtr pxmlParent, xmlNsPtr pnsSLD);
		void				BuildStrokeColor(Color& colorr, xmlNodePtr pxmlParent, xmlNsPtr pnsSLD);

		const char*			EncodeFontStyle(FontStyle s);
		const char*			EncodeFontWeight(FontWeight w);

		bool				Initialized();
		void				Cleanup();

	private:
		Workspace* m_pConnection;
	};
}

#endif //__AUGE_STYLE_IO_H__
