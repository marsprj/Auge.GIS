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
	m_size(10.0f),
	m_rotation(0.0f)
	{
		m_icon_name = "Capital.png";

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

	const char*	CapitalMarkerImpl::GetIcon()
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

	bool CapitalMarkerImpl::IsGraphic()
	{
		return false;
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

	RESULTCODE CapitalMarkerImpl::Draw(WKBPoint* pWKBPoint, Renderer* pRenderer, Transformation* pTransform)
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

	RESULTCODE CapitalMarkerImpl::Draw(WKBMultiPoint* pWKBMultiPoint, Renderer* pRenderer, Transformation* pTransform)
	{
		g_uint numPoints = pWKBMultiPoint->numPoints;
		WKBPoint* pWKBPoint = (WKBPoint*)(&(pWKBMultiPoint->points[0]));
		for(g_uint i=0; i<numPoints; i++, pWKBPoint++)
		{
			Draw(pWKBPoint, pRenderer, pTransform);
		}

		return AG_SUCCESS;
	}

	//RESULTCODE CapitalMarkerImpl::Draw(Geometry* pGeometry, Renderer* pRenderer, Transformation* pTransform)
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
	//		cairo_save(canvas_cairo);
	//		cairo_translate(canvas_cairo, sx-m_size, sy-m_size);
	//		cairo_set_source_surface(canvas_cairo, m_icon, 0,0);
	//		cairo_paint(canvas_cairo);
	//		cairo_surface_flush(canvas_surface);
	//		cairo_restore(canvas_cairo);
	//	}

	//	return AG_SUCCESS;
	//}

	void CapitalMarkerImpl::DrawSymbol()
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

	void CapitalMarkerImpl::DrawIcon()
	{
		m_icon = cairo_image_surface_create(CAIRO_FORMAT_ARGB32, AUGE_ICON_SIZE, AUGE_ICON_SIZE);
		m_cairo = cairo_create(m_icon);

		double cx,cy;
		cx=cy=AUGE_ICON_SIZE / 2.0;
		double radius = AUGE_ICON_SIZE * AUGE_ICON_SCALE / 2.0f;
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

	void CapitalMarkerImpl::SaveIcon(const char* icon_path)
	{
		cairo_surface_write_to_png(m_icon, icon_path);
	}

	Symbol*	CapitalMarkerImpl::Clone()
	{
		return (new CapitalMarkerImpl());
	}
}
