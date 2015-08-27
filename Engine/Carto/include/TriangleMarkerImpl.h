#ifndef __AUGE_TRIANGLE_MARKER_IMPL_H__
#define __AUGE_TRIANGLE_MARKER_IMPL_H__

#include "AugeMarkerSymbol.h"

#ifdef WIN32
#	include "cairo/cairo-win32.h"
#else
#	include "cairo/cairo.h"
#endif

namespace auge
{
	class TriangleMarkerImpl : public TriangleMarker
	{
	public:
		TriangleMarkerImpl();
		virtual ~TriangleMarkerImpl();

	public:
		virtual const char*		GetName();
		virtual const char*		GetIcon();
		virtual bool			IsGraphic();
		virtual RESULTCODE		Draw(Geometry* pGeometry, Renderer* pRenderer, Transformation* pTransform);
		virtual Symbol*			Clone();
				
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
		void		DrawSymbol();
		RESULTCODE	Draw(WKBPoint* pWKBPoint, Renderer* pRenderer, Transformation* pTransform);
		RESULTCODE	Draw(WKBMultiPoint* pWKBMultiPoint, Renderer* pRenderer, Transformation* pTransform);


		void		DrawIcon();
		void		SaveIcon(const char* icon_path);

	private:
		Stroke		*m_pStroke;
		Fill		*m_pFill;
		float		m_opacity;
		float		m_size;
		float		m_rotation;

		cairo_surface_t *m_icon;
		cairo_t			*m_cairo;

		std::string	m_icon_name;
	};
}

#endif //__AUGE_TRIANGLE_MARKER_IMPL_H__
