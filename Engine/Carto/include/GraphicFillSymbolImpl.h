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
		virtual const char*			GetName();
		virtual const char*			GetPath();

		virtual bool				IsGraphic();
		virtual const char*			GetIcon();
		virtual augeSymbolizerType	GetType();
		virtual RESULTCODE			Draw(Geometry* pGeometry, Renderer* pRenderer, Transformation* pTransform);
		virtual Symbol*				Clone();

		virtual Stroke*				GetStroke();
		virtual void				SetStroke(Stroke* pStroke);

		virtual void				SetOpacity(float opacity);
		virtual float				GetOpacity();

	public:
		void	SetName(const char* name);
		void	SetPath(const char* icon_path);
		void	SetFilePath(const char* file_path);

	private:
		void	LoadSymbol();

		cairo_surface_t*			CreateBaseSurface(int width, int height);
		cairo_surface_t*			CreateClipedSurface(Geometry* pGeometry, Transformation* pTransform, cairo_surface_t* base_surface, int width, int height);
		void						DrawStroke(Geometry* pGeometry, Transformation* pTransform, Stroke* pStroke, cairo_surface_t* mask_surface);

	private:
		float		m_opacity;
		Stroke*		m_pStroke;

		std::string m_name;		
		std::string m_icon_path;
		std::string m_file_path;

		cairo_surface_t *m_icon_surface;
		cairo_t			*m_icon_cairo;
	};
}

#endif //__AUGE_GRAPHIC_FILL_SYMBOL_IMPL_H__

