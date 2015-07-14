#include "SquareMarkerImpl.h"
#include "RendererCairo.h"
#include "FillImpl.h"
#include "StrokeImpl.h"

namespace auge
{
	static double g_auge_marker_triangle_points[][2] = { 
		{ 0.000000, -1.000000},
		{-0.866025,  0.500000},
		{ 0.866025,  0.500000},
		{ 0.000000, -1.000000}
	};

	SquareMarkerImpl::SquareMarkerImpl():
	m_cairo(NULL),
	m_icon(NULL),	
	m_opacity(0.0f),
	m_size(5.0f),
	m_rotation(0.0f)
	{
		m_pStroke = new StrokeImpl();
		m_pFill = new FillImpl();
	}

	SquareMarkerImpl::~SquareMarkerImpl()
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
	const char*	SquareMarkerImpl::GetName()
	{
		return "Square";
	}

	augeMarkerType SquareMarkerImpl::GetMarkType()
	{
		return augeMarkerSquare;
	}
	
	void SquareMarkerImpl::SetStroke(Stroke* pStroke)
	{
		if(m_pStroke!=NULL)
		{
			m_pStroke->Release();
			m_pStroke = NULL;
		}
		m_pStroke = pStroke;
	}

	Stroke* SquareMarkerImpl::GetStroke()
	{
		return m_pStroke;
	}

	void SquareMarkerImpl::SetFill(Fill* pFill)
	{
		if(m_pFill!=NULL)
		{
			m_pFill->Release();
			m_pFill = NULL;
		}
		m_pFill = pFill;
	}

	Fill*	SquareMarkerImpl::GetFill()
	{
		return m_pFill;
	}

	void SquareMarkerImpl::SetOpacity(float opacity)
	{
		m_opacity = opacity;
	}

	float SquareMarkerImpl::GetOpacity()
	{
		return m_opacity;
	}

	void SquareMarkerImpl::SetSize(float size)
	{
		m_size = size;
	}

	float SquareMarkerImpl::GetSize()
	{
		return m_size;
	}

	void SquareMarkerImpl::SetRotation(float rotation)
	{
		m_rotation = rotation;
	}

	float SquareMarkerImpl::GetRotation()
	{
		return m_rotation;
	}

	void SquareMarkerImpl::Release()
	{
		if(!ReleaseRef())
		{
			delete this;
		}
	}

	augeSymbolizerType SquareMarkerImpl::GetType()
	{
		return augeSymbolPoint;
	}

	RESULTCODE SquareMarkerImpl::Draw(Geometry* pGeometry, Renderer* pRenderer, Transformation* pTransform)
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
			double size_2 = m_size / 2.0;
			cairo_save(canvas_cairo);
			cairo_translate(canvas_cairo, sx-m_size, sy-m_size);
			cairo_set_source_surface(canvas_cairo, m_icon, 0,0);
			cairo_paint(canvas_cairo);
			cairo_surface_flush(canvas_surface);
			cairo_restore(canvas_cairo);
		}

		return AG_SUCCESS;
	}

	void SquareMarkerImpl::DrawIcon()
	{
		m_icon = cairo_image_surface_create(CAIRO_FORMAT_ARGB32, m_size*2, m_size*2);
		m_cairo = cairo_create(m_icon);

		double cx = m_size;
		double cy = m_size;
		double size_2 = m_size / 2.0f;
		cairo_save(m_cairo);
		cairo_translate (m_cairo, cx, cy);
		cairo_rotate (m_cairo, m_rotation*PI/180.0f);
		cairo_rectangle(m_cairo, -size_2, -size_2, m_size, m_size);
		if(m_pFill!=NULL)
		{
			GColor& color = m_pFill->GetColor();
			cairo_set_source_rgba(m_cairo, color.GetRedF(), color.GetGreenF(), color.GetBlueF(), color.GetAlphaF());
			if(m_pStroke==NULL)
			{
				cairo_fill(m_cairo);
			}
			else
			{
				cairo_fill_preserve(m_cairo);
			}
		}
		if(m_pStroke!=NULL)
		{
			set_stroke_style(m_cairo, m_pStroke);
			cairo_stroke(m_cairo);
		}
		cairo_restore(m_cairo);

		//cairo_surface_write_to_png(m_icon, "G:\\temp\\map\\icon.png");
	}
}