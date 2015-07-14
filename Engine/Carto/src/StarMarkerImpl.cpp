#include "StarMarkerImpl.h"
#include "RendererCairo.h"
#include "FillImpl.h"
#include "StrokeImpl.h"

namespace auge
{
	static double g_auge_marker_star_points[][2] = { 
		{ 0.000000, -1.000000},
		{-0.866025,  0.500000},
		{ 0.866025,  0.500000},
		{ 0.000000, -1.000000}
	};

	StarMarkerImpl::StarMarkerImpl():
	m_cairo(NULL),
	m_icon(NULL),	
	m_opacity(0.0f),
	m_size(50.0f),
	m_rotation(0.0f)
	{
		m_pStroke = new StrokeImpl();
		m_pFill = new FillImpl();
	}

	StarMarkerImpl::~StarMarkerImpl()
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
	augeMarkerType StarMarkerImpl::GetMarkType()
	{
		return augeMarkerStar;
	}
	
	void StarMarkerImpl::SetStroke(Stroke* pStroke)
	{
		if(m_pStroke!=NULL)
		{
			m_pStroke->Release();
			m_pStroke = NULL;
		}
		m_pStroke = pStroke;
	}

	Stroke* StarMarkerImpl::GetStroke()
	{
		return m_pStroke;
	}

	void StarMarkerImpl::SetFill(Fill* pFill)
	{
		if(m_pFill!=NULL)
		{
			m_pFill->Release();
			m_pFill = NULL;
		}
		m_pFill = pFill;
	}

	Fill*	StarMarkerImpl::GetFill()
	{
		return m_pFill;
	}

	void StarMarkerImpl::SetOpacity(float opacity)
	{
		m_opacity = opacity;
	}

	float StarMarkerImpl::GetOpacity()
	{
		return m_opacity;
	}

	void StarMarkerImpl::SetSize(float size)
	{
		m_size = size;
	}

	float StarMarkerImpl::GetSize()
	{
		return m_size;
	}

	void StarMarkerImpl::SetRotation(float rotation)
	{
		m_rotation = rotation;
	}

	float StarMarkerImpl::GetRotation()
	{
		return m_rotation;
	}

	void StarMarkerImpl::Release()
	{
		if(!ReleaseRef())
		{
			delete this;
		}
	}

	augeSymbolizerType StarMarkerImpl::GetType()
	{
		return augeSymbolPoint;
	}

	RESULTCODE StarMarkerImpl::Draw(Geometry* pGeometry, Renderer* pRenderer, Transformation* pTransform)
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
			double width_2 = m_size / 2.0;
			double height_2= m_size / 2.0;
			cairo_translate(canvas_cairo, sx-width_2, sy-height_2);
			cairo_set_source_surface(canvas_cairo, m_icon, 0,0);
			cairo_paint(canvas_cairo);
			cairo_surface_flush(canvas_surface);
		}

		return AG_SUCCESS;
	}

	void StarMarkerImpl::DrawIcon()
	{
		m_icon = cairo_image_surface_create(CAIRO_FORMAT_ARGB32, m_size*2, m_size*2);
		m_cairo = cairo_create(m_icon);

		size_t count = sizeof(g_auge_marker_star_points) / sizeof(double) / 2;
		
		cairo_new_path(m_cairo);	

		double offset_x=0, offset_y=0;
		double size_2 = m_size / 2.0;
		double x, y;
		x = g_auge_marker_star_points[0][0];
		y = g_auge_marker_star_points[0][1];
		cairo_move_to(m_cairo, x*m_size+m_size, y*m_size+m_size);
		for(size_t i=1; i<count; i++)
		{
			x = g_auge_marker_star_points[i][0];
			y = g_auge_marker_star_points[i][1];
			cairo_line_to(m_cairo, x*m_size+m_size, y*m_size+m_size);
		}
		cairo_close_path(m_cairo);

		cairo_rotate (m_cairo, m_rotation*PI/180.0f);
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

		cairo_surface_write_to_png(m_icon, "g:\\temp\\map\\icon.png");
	}
}