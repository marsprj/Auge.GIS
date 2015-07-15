#ifndef __AUGE_SIMPLE_LINE_SYMBOL_IMPL_H__
#define __AUGE_SIMPLE_LINE_SYMBOL_IMPL_H__

#include "AugeLineSymbol.h"
#include <string>

#ifdef WIN32
#	include "cairo/cairo-win32.h"
#else
#	include "cairo/cairo.h"
#endif

namespace auge
{
	class SimpleLineSymbolImpl : public SimpleLineSymbol
	{
	public:
		SimpleLineSymbolImpl();
		virtual ~SimpleLineSymbolImpl();

	public:
		virtual const char*			GetName();
		virtual const char*			GetIcon();
		virtual augeSymbolizerType	GetType();
		virtual RESULTCODE			Draw(Geometry* pGeometry, Renderer* pRenderer, Transformation* pTransform);

		virtual Stroke*				GetStroke();
		virtual void				SetStroke(Stroke* pStroke);
		virtual augeLineType		GetLineType();

		virtual void				Release();

	private:
		void		DrawIcon();
		void		SaveIcon(const char* icon_path);

	private:
		Stroke			*m_pStroke;
		augeLineType	m_line_type;

		std::string		m_icon_name;

		cairo_surface_t *m_icon;
		cairo_t			*m_cairo;
	};
}

#endif //__AUGE_SIMPLE_LINE_SYMBOL_IMPL_H__

