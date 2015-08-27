#include "TriangleMarkerImpl.h"
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

	TriangleMarkerImpl::TriangleMarkerImpl():
	m_cairo(NULL),
	m_icon(NULL),	
	m_opacity(0.0f),
	m_size(20.0f),
	m_rotation(0.0f)
	{
		m_pStroke = new StrokeImpl();
		m_pFill = new FillImpl();

		m_icon_name = "Triangle.png";
	}

	TriangleMarkerImpl::~TriangleMarkerImpl()
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
	const char*	TriangleMarkerImpl::GetName()
	{
		return "Triangle";
	}

	const char*	TriangleMarkerImpl::GetIcon()
	{
		char icon_path[AUGE_PATH_MAX];
		memset(icon_path, 0, AUGE_PATH_MAX);
		auge_make_symbol_icon_path(m_icon_name.c_str(), icon_path, AUGE_PATH_MAX);

		if(g_access(icon_path,4))
		{
			DrawIcon();
			SaveIcon(icon_path);
		}

		return m_icon_name.c_str();
	}

	bool TriangleMarkerImpl::IsGraphic()
	{
		return false;
	}

	augeMarkerType TriangleMarkerImpl::GetMarkType()
	{
		return augeMarkerTriangle;
	}
	
	void TriangleMarkerImpl::SetStroke(Stroke* pStroke)
	{
		if(m_pStroke!=NULL)
		{
			m_pStroke->Release();
			m_pStroke = NULL;
		}
		m_pStroke = pStroke;
	}

	Stroke* TriangleMarkerImpl::GetStroke()
	{
		return m_pStroke;
	}

	void TriangleMarkerImpl::SetFill(Fill* pFill)
	{
		if(m_pFill!=NULL)
		{
			m_pFill->Release();
			m_pFill = NULL;
		}
		m_pFill = pFill;
	}

	Fill*	TriangleMarkerImpl::GetFill()
	{
		return m_pFill;
	}

	void TriangleMarkerImpl::SetOpacity(float opacity)
	{
		m_opacity = opacity;
	}

	float TriangleMarkerImpl::GetOpacity()
	{
		return m_opacity;
	}

	void TriangleMarkerImpl::SetSize(float size)
	{
		m_size = size;
	}

	float TriangleMarkerImpl::GetSize()
	{
		return m_size;
	}

	void TriangleMarkerImpl::SetRotation(float rotation)
	{
		m_rotation = rotation;
	}

	float TriangleMarkerImpl::GetRotation()
	{
		return m_rotation;
	}

	void TriangleMarkerImpl::Release()
	{
		if(!ReleaseRef())
		{
			delete this;
		}
	}

	augeSymbolizerType TriangleMarkerImpl::GetType()
	{
		return augeSymbolPoint;
	}

	RESULTCODE TriangleMarkerImpl::Draw(Geometry* pGeometry, Renderer* pRenderer, Transformation* pTransform)
	{
		if(pGeometry==NULL)
		{
			return AG_FAILURE;
		}

		augeGeometryType type = pGeometry->GeometryType();
		switch(type)
		{
		case augeGTPoint:
			{
				WKBPoint* pWKBPoint = (WKBPoint*)pGeometry->AsBinary();
				Draw(pWKBPoint, pRenderer, pTransform);
			}
			break;
		case augeGTMultiPoint:
			{
				WKBMultiPoint* pWKBMultiPoint = (WKBMultiPoint*)pGeometry->AsBinary();
				Draw(pWKBMultiPoint, pRenderer, pTransform);
			}
			break;
		}

		return AG_SUCCESS;
	}

	RESULTCODE TriangleMarkerImpl::Draw(WKBPoint* pWKBPoint, Renderer* pRenderer, Transformation* pTransform)
	{
		if(m_size==0)
		{
			return AG_FAILURE;
		}

		RendererCairo* pRendererCairo = static_cast<RendererCairo*>(pRenderer);
		cairo_t			*canvas_cairo = pRendererCairo->GetCairo();
		cairo_surface_t	*canvas_surface=pRendererCairo->GetCairoSurface();

		int sx=0, sy=0;		
		pTransform->ToScreenPoint(pWKBPoint->point.x, pWKBPoint->point.y, sx, sy);

		//cairo_surface_t *image = cairo_image_surface_create_from_png(resource);
		if(m_cairo==NULL)
		{
			DrawSymbol();
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

	RESULTCODE TriangleMarkerImpl::Draw(WKBMultiPoint* pWKBMultiPoint, Renderer* pRenderer, Transformation* pTransform)
	{
		g_uint numPoints = pWKBMultiPoint->numPoints;
		WKBPoint* pWKBPoint = (WKBPoint*)(&(pWKBMultiPoint->points[0]));
		for(g_uint i=0; i<numPoints; i++, pWKBPoint++)
		{
			Draw(pWKBPoint, pRenderer, pTransform);
		}

		return AG_SUCCESS;
	}

	//RESULTCODE TriangleMarkerImpl::Draw(Geometry* pGeometry, Renderer* pRenderer, Transformation* pTransform)
	//{
	//	if(pGeometry==NULL)
	//	{
	//		return AG_FAILURE;
	//	}
	//	if(pGeometry->GeometryType()!=augeGTPoint)
	//	{
	//		return AG_FAILURE;
	//	}
	//	if(m_size==0)
	//	{
	//		return AG_FAILURE;
	//	}

	//	RendererCairo* pRendererCairo = static_cast<RendererCairo*>(pRenderer);
	//	cairo_t			*canvas_cairo = pRendererCairo->GetCairo();
	//	cairo_surface_t	*canvas_surface=pRendererCairo->GetCairoSurface();

	//	int sx=0, sy=0;
	//	WKBPoint* pWKBPoint = (WKBPoint*)pGeometry->AsBinary();
	//	pTransform->ToScreenPoint(pWKBPoint->point.x, pWKBPoint->point.y, sx, sy);
	//	
	//	//cairo_surface_t *image = cairo_image_surface_create_from_png(resource);
	//	if(m_cairo==NULL)
	//	{
	//		DrawSymbol();
	//	}
	//	
	//	if(m_cairo!=NULL)
	//	{
	//		double size_2 = m_size / 2.0;
	//		cairo_save(canvas_cairo);
	//		cairo_translate(canvas_cairo, sx-m_size, sy-m_size);
	//		cairo_set_source_surface(canvas_cairo, m_icon, 0,0);
	//		cairo_paint(canvas_cairo);
	//		cairo_surface_flush(canvas_surface);
	//		cairo_restore(canvas_cairo);
	//	}

	//	return AG_SUCCESS;
	//}

	void TriangleMarkerImpl::DrawSymbol()
	{
		m_icon = cairo_image_surface_create(CAIRO_FORMAT_ARGB32, m_size*2, m_size*2);
		m_cairo = cairo_create(m_icon);

		size_t count = sizeof(g_auge_marker_triangle_points) / sizeof(double) / 2;
		double offset_x=0, offset_y=0;		
		double cx=m_size;
		double cy=m_size;
		double x, y;
		double size_2 = m_size / 2.0f;

		cairo_new_path(m_cairo);

		cairo_translate (m_cairo, cx, cy);
		//cairo_scale(m_cairo,10,10);
		cairo_rotate (m_cairo, m_rotation*PI/180.0f);
		
		x = g_auge_marker_triangle_points[0][0];
		y = g_auge_marker_triangle_points[0][1];
		cairo_move_to(m_cairo, x*size_2, y*size_2);
		for(size_t i=1; i<count; i++)
		{
			x = g_auge_marker_triangle_points[i][0];
			y = g_auge_marker_triangle_points[i][1];
			cairo_line_to(m_cairo, x*size_2, y*size_2);
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
		//cairo_surface_write_to_png(m_icon, "G:\\temp\\map\\icon.png");
	}

	void TriangleMarkerImpl::DrawIcon()
	{
		m_icon = cairo_image_surface_create(CAIRO_FORMAT_ARGB32, AUGE_ICON_SIZE, AUGE_ICON_SIZE);
		m_cairo = cairo_create(m_icon);

		size_t count = sizeof(g_auge_marker_triangle_points) / sizeof(double) / 2;
		double offset_x=0, offset_y=0;		
		double size_2 = AUGE_ICON_SIZE / 2.0f;
		double cx=size_2;
		double cy=size_2;
		double x, y;

		cairo_new_path(m_cairo);

		cairo_translate (m_cairo, cx, cy);
		cairo_scale(m_cairo,AUGE_ICON_SCALE,AUGE_ICON_SCALE);
		//cairo_rotate (m_cairo, m_rotation*PI/180.0f);
		
		x = g_auge_marker_triangle_points[0][0];
		y = g_auge_marker_triangle_points[0][1];
		cairo_move_to(m_cairo, x*size_2, y*size_2);
		for(size_t i=1; i<count; i++)
		{
			x = g_auge_marker_triangle_points[i][0];
			y = g_auge_marker_triangle_points[i][1];
			cairo_line_to(m_cairo, x*size_2, y*size_2);
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
		//cairo_surface_write_to_png(m_icon, "G:\\temp\\map\\icon.png");
	}

	void TriangleMarkerImpl::SaveIcon(const char* icon_path)
	{
		cairo_surface_write_to_png(m_icon, icon_path);
	}

	Symbol*	TriangleMarkerImpl::Clone()
	{
		return (new TriangleMarkerImpl());
	}
}
