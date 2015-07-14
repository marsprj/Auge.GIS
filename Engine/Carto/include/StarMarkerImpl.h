#ifndef __AUGE_STAR_MARKER_IMPL_H__
#define __AUGE_STAR_MARKER_IMPL_H__

#include "AugeMarkerSymbol.h"

#ifdef WIN32
#	include "cairo/cairo-win32.h"
#else
#	include "cairo/cairo.h"
#endif

namespace auge
{
	class StarMarkerImpl : public StarMarker
	{
	public:
		StarMarkerImpl();
		virtual ~StarMarkerImpl();

	public:
		virtual RESULTCODE		Draw(Geometry* pGeometry, Renderer* pRenderer, Transformation* pTransform);
				
		virtual augeMarkerType	GetMarkType();

		virtual void			SetStroke(Stroke* pStroke);
		virtual Stroke*			GetStroke();

		virtual void			SetFill(Fill* pFill);
		virtual Fill*			GetFill();

		virtual void			SetOpacity(float opacity);
		virtual float			GetOpacity();

		virtual void			SetSize(float size);
		virtual float			GetSize();

		virtual void			SetRotation(float rotation);
		virtual float			GetRotation();	

		virtual augeSymbolizerType	GetType();
		
		virtual void			Release();

	private:
		void		DrawIcon();

	private:
		Stroke		*m_pStroke;
		Fill		*m_pFill;
		float		m_opacity;
		float		m_size;
		float		m_rotation;

		cairo_surface_t *m_icon;
		cairo_t			*m_cairo;

	};
}

#endif //__AUGE_STAR_MARKER_IMPL_H__
