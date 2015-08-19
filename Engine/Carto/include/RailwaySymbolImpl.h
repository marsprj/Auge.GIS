#ifndef __AUGE_RAILWAY_SYMBOL_IMPL_H__
#define __AUGE_RAILWAY_SYMBOL_IMPL_H__

#include "AugeLineSymbol.h"

#ifdef WIN32
#	include "cairo/cairo-win32.h"
#else
#	include "cairo/cairo.h"
#endif

namespace auge
{
	class RailwaySymbolImpl : public RailwaySymbol
	{
	public:
		RailwaySymbolImpl();
		virtual ~RailwaySymbolImpl();

	public:
		virtual const char*		GetName();
		virtual const char*		GetIcon();
		virtual bool			IsGraphic();
		virtual RESULTCODE		Draw(Geometry* pGeometry, Renderer* pRenderer, Transformation* pTransform);
		virtual Symbol*			Clone();

		virtual Stroke*			GetStroke();
		virtual void			SetStroke(Stroke* pStroke);
		virtual augeLineType	GetLineType();
		virtual augeSymbolizerType	GetType();
		
		virtual void			Release();

	private:
		void		DrawIcon();
		void		SaveIcon(const char* icon_path);

	private:
		Stroke		*m_pStroke;
		g_uint		m_block_width;
		g_uint		m_block_length;

		std::string m_icon_name;

		cairo_surface_t *m_icon;
		cairo_t			*m_cairo;
	};
}

#endif //__AUGE_RAILWAY_SYMBOL_IMPL_H__
