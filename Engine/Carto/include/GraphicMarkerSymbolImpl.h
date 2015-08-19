#ifndef __AUGE_GRAPHIC_MARKER_SYMBOL_IMPL_H__
#define __AUGE_GRAPHIC_MARKER_SYMBOL_IMPL_H__

#include "AugeRegionSymbol.h"
#include <string>

#ifdef WIN32
#	include "cairo/cairo-win32.h"
#else
#	include "cairo/cairo.h"
#endif

namespace auge
{
	class GraphicMarkerSymbolImpl : public GraphicMarkerSymbol
	{
	public:
		GraphicMarkerSymbolImpl();
		virtual ~GraphicMarkerSymbolImpl();
	public:
		virtual const char*			GetName();
		virtual const char*			GetPath();

		virtual bool				IsGraphic();
		virtual const char*			GetIcon();
		virtual augeSymbolizerType	GetType();
		virtual RESULTCODE			Draw(Geometry* pGeometry, Renderer* pRenderer, Transformation* pTransform);
		virtual Symbol*				Clone();

		virtual void				SetOpacity(float opacity);
		virtual float				GetOpacity();

		virtual void				SetSize(float size);
		virtual float				GetSize();

		virtual void				SetRotation(float rotation);
		virtual float				GetRotation();	

	public:
		void	SetName(const char* name);
		void	SetPath(const char* icon_path);
		void	SetFilePath(const char* file_path);

	private:
		void	LoadSymbol();

	private:
		float		m_size;
		float		m_rotation;
		float		m_opacity;

		std::string m_name;		
		std::string m_icon_path;
		std::string m_file_path;

		cairo_surface_t *m_icon;
		cairo_t			*m_cairo;
	};
}

#endif //__AUGE_GRAPHIC_MARKER_SYMBOL_IMPL_H__

