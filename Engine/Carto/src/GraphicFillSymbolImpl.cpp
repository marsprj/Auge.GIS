#include "GraphicFillSymbolImpl.h"
#include "RendererCairo.h"

namespace auge
{
	GraphicFillSymbolImpl::GraphicFillSymbolImpl():
	m_icon_surface(NULL),
	m_icon_cairo(NULL),	
	m_opacity(0.0f),
	m_pStroke(NULL)
	{

	}

	GraphicFillSymbolImpl::~GraphicFillSymbolImpl()
	{
		if(m_icon_cairo!=NULL)
		{
			cairo_destroy(m_icon_cairo);
			m_icon_cairo = NULL;
		}
		if(m_icon_surface!=NULL)
		{
			cairo_surface_destroy(m_icon_surface);
			m_icon_surface = NULL;
		}
		if(m_pStroke!=NULL)
		{
			AUGE_SAFE_RELEASE(m_pStroke);
		}
	}

	const char*	GraphicFillSymbolImpl::GetName()
	{
		return m_name.c_str();
	}

	const char*	GraphicFillSymbolImpl::GetPath()
	{
		return m_icon_path.c_str();
	}

	bool GraphicFillSymbolImpl::IsGraphic()
	{
		return true;
	}

	void GraphicFillSymbolImpl::SetOpacity(float opacity)
	{
		m_opacity = opacity;
	}

	float GraphicFillSymbolImpl::GetOpacity()
	{
		return m_opacity;
	}

	void GraphicFillSymbolImpl::SetName(const char* name)
	{
		if(name==NULL)
		{
			m_name.clear();
		}
		else
		{
			m_name = name;
		}
	}

	void GraphicFillSymbolImpl::SetPath(const char* icon_path)
	{
		if(icon_path==NULL)
		{
			m_icon_path.clear();
		}
		else
		{
			m_icon_path = icon_path;
		}
	}

	void GraphicFillSymbolImpl::SetFilePath(const char* file_path)
	{
		if(file_path==NULL)
		{
			m_file_path.clear();
		}
		else
		{
			m_file_path = file_path;
		}
	}


	void GraphicFillSymbolImpl::SetStroke(Stroke* pStroke)
	{
		if(m_pStroke!=NULL)
		{
			m_pStroke->Release();
			m_pStroke = NULL;
		}
		m_pStroke = pStroke;
	}

	Stroke* GraphicFillSymbolImpl::GetStroke()
	{
		return m_pStroke;
	}

	const char*	GraphicFillSymbolImpl::GetIcon()
	{
		return m_icon_path.c_str();
	}

	augeSymbolizerType GraphicFillSymbolImpl::GetType()
	{
		return augeSymbolPolygon;
	}

	RESULTCODE GraphicFillSymbolImpl::Draw(Geometry* pGeometry, Renderer* pRenderer, Transformation* pTransform)
	{
		if(pGeometry==NULL)
		{
			return AG_FAILURE;
		}
		augeGeometryType geom_type = pGeometry->GeometryType();
		if(geom_type!=augeGTPolygon && geom_type!=augeGTMultiPolygon)
		{
			return AG_FAILURE;
		}

		RendererCairo* pRendererCairo = static_cast<RendererCairo*>(pRenderer);
		cairo_t			*canvas_cairo = pRendererCairo->GetCairo();
		cairo_surface_t	*canvas_surface=pRendererCairo->GetCairoSurface();

		if(m_icon_cairo==NULL)
		{
			LoadSymbol();
		}

		// Create Base Surface
		double canvas_w = pTransform->GetCanvasWidth();
		double canvas_h = pTransform->GetCanvasHeight();
		
		//------------------------------------------------------------------------
		// Generate base by cairo matrix
		//------------------------------------------------------------------------
		cairo_surface_t	*base_surface = CreateBaseSurface(canvas_w, canvas_h);

		//------------------------------------------------------------------------
		// Clip base surface by mask
		//------------------------------------------------------------------------
		cairo_surface_t	*mask_surface = CreateClipedSurface(pGeometry, pTransform, base_surface, canvas_w, canvas_h);
		if(m_pStroke!=NULL)
		{
			DrawStroke(pGeometry, pTransform, m_pStroke, mask_surface);
		}

		//------------------------------------------------------------------------
		// paint mask surfact to canvas
		//------------------------------------------------------------------------
		cairo_set_source_surface(canvas_cairo, mask_surface, 0, 0);
		cairo_paint(canvas_cairo);

		cairo_surface_destroy(mask_surface);
		cairo_surface_destroy(base_surface);

		return AG_SUCCESS;
	}

	void GraphicFillSymbolImpl::LoadSymbol()
	{
		m_icon_surface = cairo_image_surface_create_from_png(m_file_path.c_str());
		if(m_icon_surface)
		{
			m_icon_cairo = cairo_create(m_icon_surface);
		}
	}

	cairo_surface_t* GraphicFillSymbolImpl::CreateBaseSurface(int width, int height)
	{
		if(m_icon_cairo==NULL)
		{
			LoadSymbol();
		}

		int icon_w = cairo_image_surface_get_width(m_icon_surface);
		int icon_h = cairo_image_surface_get_height(m_icon_surface);

		cairo_pattern_t *pattern;
		pattern = cairo_pattern_create_for_surface (m_icon_surface);
		cairo_pattern_set_extend (pattern, CAIRO_EXTEND_REPEAT);

		cairo_matrix_t   matrix;
		cairo_matrix_init_scale (&matrix, 1.0, 1.0);
		cairo_pattern_set_matrix (pattern, &matrix);

		cairo_surface_t	*base_surface=cairo_image_surface_create(CAIRO_FORMAT_ARGB32, width, height);
		cairo_t			*base_cairo = cairo_create(base_surface);

		cairo_set_source (base_cairo, pattern);
		cairo_rectangle (base_cairo, 0, 0, width, height);
		cairo_fill (base_cairo);
		cairo_destroy(base_cairo);

		return base_surface;
	}

	cairo_surface_t* GraphicFillSymbolImpl::CreateClipedSurface(Geometry* pGeometry, Transformation* pTransform, cairo_surface_t* base_surface, int width, int height)
	{
		cairo_surface_t	*mask_surface = cairo_image_surface_create(CAIRO_FORMAT_ARGB32, width, height);
		cairo_t *mask_cairo = cairo_create(mask_surface);

		// Clip Base Surface
		g_uchar* wkb = pGeometry->AsBinary();
		cairo_new_path(mask_cairo);
		switch(pGeometry->GeometryType())
		{
		case augeGTPolygon:
			build_path(mask_cairo, (WKBPolygon*)wkb, pTransform);
			break;
		case augeGTMultiPolygon:
			build_path(mask_cairo, (WKBMultiPolygon*)wkb, pTransform);
			break;
		}

		//------------------------------------------------------------------------
		// clip base surface by mask
		//------------------------------------------------------------------------
		cairo_clip (mask_cairo);
		cairo_scale (mask_cairo, 1.0, 1.0);
		
		cairo_set_source_surface(mask_cairo, base_surface, 0, 0);
		cairo_paint(mask_cairo);
		cairo_destroy(mask_cairo);

		return mask_surface;
	}

	void GraphicFillSymbolImpl::DrawStroke(Geometry* pGeometry, Transformation* pTransform, Stroke* pStroke, cairo_surface_t* mask_surface)
	{
		cairo_t *mask_cairo = cairo_create(mask_surface);

		g_uchar* wkb = pGeometry->AsBinary();
		cairo_new_path(mask_cairo);
		switch(pGeometry->GeometryType())
		{
		case augeGTPolygon:
			build_path(mask_cairo, (WKBPolygon*)wkb, pTransform);
			break;
		case augeGTMultiPolygon:
			build_path(mask_cairo, (WKBMultiPolygon*)wkb, pTransform);
			break;
		}
		set_stroke_style(mask_cairo, m_pStroke);
		cairo_stroke(mask_cairo);

		cairo_destroy(mask_cairo);
	}

	Symbol*	GraphicFillSymbolImpl::Clone()
	{
		GraphicFillSymbolImpl* pSymbol = new GraphicFillSymbolImpl();
		pSymbol->SetName(m_name.c_str());
		pSymbol->SetPath(m_icon_path.c_str());
		pSymbol->SetFilePath(m_file_path.c_str());
		pSymbol->SetOpacity(m_opacity);
		pSymbol->SetStroke(m_pStroke);
		if(m_pStroke)
		{
			m_pStroke->AddRef();
		}
		return pSymbol;
	}
}