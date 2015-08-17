#ifndef __AUGE_GRAPHIC_FILL_SYMBOL_IMPL_H__
#define __AUGE_GRAPHIC_FILL_SYMBOL_IMPL_H__

#include "AugeRegionSymbol.h"
#include <string>

#ifdef WIN32
#	include "cairo/cairo-win32.h"
#else
#	include "cairo/cairo.h"
#endif

namespace auge
{
	class GraphicFillSymbolImpl : public GraphicFillSymbol
	{
	public:
		GraphicFillSymbolImpl();
		virtual ~GraphicFillSymbolImpl();
	public:
		virtual const char*		GetName();
		virtual const char*		GetPath();
		virtual bool				IsGraphic();
		virtual void			SetOpacity(float opacity);
		virtual float			GetOpacity();

	private:
		float	m_opacity;
		std::string m_name;
		std::string m_path;
	};
}

#endif //__AUGE_GRAPHIC_FILL_SYMBOL_IMPL_H__

