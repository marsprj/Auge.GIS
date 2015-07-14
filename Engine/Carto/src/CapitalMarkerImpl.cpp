#include "CapitalMarkerImpl.h"
#include "RendererCairo.h"
#include "FillImpl.h"
#include "StrokeImpl.h"

namespace auge
{
	CapitalMarkerImpl::CapitalMarkerImpl():
	m_cairo(NULL),
	m_icon(NULL),	
	m_opacity(0.0f),
	m_size(20.0f),
	m_rotation(0.0f)
	{
		m_pStroke = new StrokeImpl();
		m_pFill = new FillImpl();
	}

	CapitalMarkerImpl::~CapitalMarkerImpl()
	{
		if(m_pStroke!=NULL)
		{
			m_pStroke->Release();
			m_pStroke = NULL;
		}
		if(m_pFill!=NULL)
		{
			m_pFill->Release();
			m_pFill = NULL;
		}
		if(m_cairo!=NULL)
		{
			cairo_destroy(m_cairo);
			m_cairo = NULL;
		}
		if(m_icon!=NULL)
		{
			cairo_surface_destroy(m_icon);
			m_cairo = NULL;
		}
	}

	//////////////////////////////////////////////////////////////////////////
	const char*	CapitalMarkerImpl::GetName()
	{
		return "Capital";
	}

	augeMarkerType CapitalMarkerImpl::GetMarkType()
	{
		return augeMarkerCapital;
	}
	
	void CapitalMarkerImpl::SetStroke(Stroke* pStroke)
	{
		if(m_pStroke!=NULL)
		{
			m_pStroke->Release();
			m_pStroke = NULL;
		}
		m_pStroke = pStroke;
	}

	Stroke* CapitalMarkerImpl::GetStroke()
	{
		return m_pStroke;
	}

	void CapitalMarkerImpl::SetFill(Fill* pFill)
	{
		if(m_pFill!=NULL)
		{
			m_pFill->Release();
			m_pFill = NULL;
		}
		m_pFill = pFill;
	}

	Fill*	CapitalMarkerImpl::GetFill()
	{
		return m_pFill;
	}

	void CapitalMarkerImpl::SetOpacity(float opacity)
	{
		m_opacity = opacity;
	}

	float CapitalMarkerImpl::GetOpacity()
	{
		return m_opacity;
	}

	void CapitalMarkerImpl::SetSize(float size)
	{
		m_size = size;
	}

	float CapitalMarkerImpl::GetSize()
	{
		return m_size;
	}

	void CapitalMarkerImpl::SetRotation(float rotation)
	{
		m_rotation = rotation;
	}

	float CapitalMarkerImpl::GetRotation()
	{
		return m_rotation;
	}

	void CapitalMarkerImpl::Release()
	{
		if(!ReleaseRef())
		{
			delete this;
		}
	}

	augeSymbolizerType CapitalMarkerImpl::GetType()
	{
		return augeSymbolPoint;
	}

	RESULTCODE CapitalMarkerImpl::Draw(Geometry* pGeometry, Renderer* pRenderer, Transformation* pTransform)
	{
		if(pGeometry==NULL)
		{
			return AG_FAILURE;
		}
		if(pGeometry->GeometryType()!=augeGTPoint)
		{
			return AG_FAILURE;
		}
		if(m_size==0)
		{
			return AG_FAILURE;
		}

		RendererCairo* pRendererCairo = static_cast<RendererCairo*>(pRenderer);
		cairo_t			*canvas_cairo = pRendererCairo->GetCairo();
		cairo_surface_t	*canvas_surface=pRendererCairo->GetCairoSurface();

		int sx=0, sy=0;
		WKBPoint* pWKBPoint = (WKBPoint*)pGeometry->AsBinary();
		pTransform->ToScreenPoint(pWKBPoint->point.x, pWKBPoint->point.y, sx, sy);
		
		//cairo_surface_t *image = cairo_image_surface_create_from_png(resource);
		if(m_cairo==NULL)
		{
			DrawIcon();
		}
		
		if(m_cairo!=NULL)
		{
			cairo_save(canvas_cairo);
			cairo_translate(canvas_cairo, sx-m_size, sy-m_size);
			cairo_set_source_surface(canvas_cairo, m_icon, 0,0);
			cairo_paint(canvas_cairo);
			cairo_surface_flush(canvas_surface);
			cairo_restore(canvas_cairo);
		}

		return AG_SUCCESS;
	}

	void CapitalMarkerImpl::DrawIcon()
	{
		m_icon = cairo_image_surface_create(CAIRO_FORMAT_ARGB32, m_size*2, m_size*2);
		m_cairo = cairo_create(m_icon);

		double cx = m_size;
		double cy = m_size;
		double radius = m_size / 2.0f;
		cairo_save(m_cairo);
		cairo_arc(m_cairo, cx, cy, radius, 0, 2*PI);

		cairo_save(m_cairo);
		cairo_arc(m_cairo, cx, cy, radius, 0, 2*PI);
		cairo_set_source_rgba(m_cairo, 1.0f, 1.0f, 1.0f, 1.0f);
		cairo_fill_preserve(m_cairo);
		cairo_set_source_rgba(m_cairo, 0.0f, 0.0f, 0.0f, 1.0f);
		cairo_set_line_width(m_cairo, 1.0f);
		cairo_stroke(m_cairo);

		cairo_arc(m_cairo, cx, cy, radius*0.2, 0, 2*PI);
		cairo_set_source_rgba(m_cairo, 1.0f, 0.0f, 0.0f, 1.0f);
		cairo_fill_preserve(m_cairo);
		cairo_set_line_width(m_cairo, 1.0f);
		cairo_stroke(m_cairo);
		cairo_restore(m_cairo);

		//cairo_surface_write_to_png(m_icon, "G:\\temp\\map\\icon.png");
	}
}