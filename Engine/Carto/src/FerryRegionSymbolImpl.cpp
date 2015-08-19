#include "FerryRegionSymbolImpl.h"
#include "RendererCairo.h"
#include "FillImpl.h"
#include "StrokeImpl.h"

namespace auge
{
	FerryRegionSymbolImpl::FerryRegionSymbolImpl():
	m_cairo(NULL),
	m_icon(NULL),	
	m_opacity(0.0f),
	m_size(5.0f),
	m_rotation(0.0f)
	{
		m_pStroke = new StrokeImpl();
		GColor color(0,0,0,255);
		m_pStroke->SetColor(color);
		m_pFill = new FillImpl();
		color.Set(190,255,232,255);
		m_pFill->SetColor(color);

		m_icon_name = "FerryRegion.png";
	}

	FerryRegionSymbolImpl::~FerryRegionSymbolImpl()
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
	const char*	FerryRegionSymbolImpl::GetName()
	{
		return "Ferry";
	}

	const char*	FerryRegionSymbolImpl::GetIcon()
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

	bool FerryRegionSymbolImpl::IsGraphic()
	{
		return false;
	}

	augeMarkerType FerryRegionSymbolImpl::GetMarkType()
	{
		return augeMarkerSquare;
	}
	
	void FerryRegionSymbolImpl::SetStroke(Stroke* pStroke)
	{
		if(m_pStroke!=NULL)
		{
			m_pStroke->Release();
			m_pStroke = NULL;
		}
		m_pStroke = pStroke;
	}

	Stroke* FerryRegionSymbolImpl::GetStroke()
	{
		return m_pStroke;
	}

	void FerryRegionSymbolImpl::SetFill(Fill* pFill)
	{
		if(m_pFill!=NULL)
		{
			m_pFill->Release();
			m_pFill = NULL;
		}
		m_pFill = pFill;
	}

	Fill*	FerryRegionSymbolImpl::GetFill()
	{
		return m_pFill;
	}

	void FerryRegionSymbolImpl::SetOpacity(float opacity)
	{
		m_opacity = opacity;
	}

	float FerryRegionSymbolImpl::GetOpacity()
	{
		return m_opacity;
	}

	void FerryRegionSymbolImpl::SetSize(float size)
	{
		m_size = size;
	}

	float FerryRegionSymbolImpl::GetSize()
	{
		return m_size;
	}

	void FerryRegionSymbolImpl::SetRotation(float rotation)
	{
		m_rotation = rotation;
	}

	float FerryRegionSymbolImpl::GetRotation()
	{
		return m_rotation;
	}

	void FerryRegionSymbolImpl::Release()
	{
		if(!ReleaseRef())
		{
			delete this;
		}
	}

	augeSymbolizerType FerryRegionSymbolImpl::GetType()
	{
		return augeSymbolPoint;
	}

	RESULTCODE FerryRegionSymbolImpl::Draw(Geometry* pGeometry, Renderer* pRenderer, Transformation* pTransform)
	{
		if(pGeometry==NULL)
		{
			return AG_FAILURE;
		}
		augeGeometryType geom_type = pGeometry->GeometryType();
		if((geom_type!=augeGTPolygon) &&(geom_type!=augeGTMultiPolygon))
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

		Fill	*pFill = GetFill();
		Stroke	*pStroke = GetStroke();

		g_uchar* wkb = pGeometry->AsBinary();
		cairo_new_path(canvas_cairo);
		switch(geom_type)
		{
		case augeGTPolygon:
			build_path(canvas_cairo, (WKBPolygon*)wkb, pTransform);
			break;
		case augeGTMultiPolygon:
			build_path(canvas_cairo, (WKBMultiPolygon*)wkb, pTransform);
			break;
		}

		if(pFill!=NULL)
		{
			GColor& color = pFill->GetColor();
			cairo_set_source_rgba(canvas_cairo, color.GetRedF(), color.GetGreenF(), color.GetBlueF(), color.GetAlphaF());
			if(pStroke==NULL)
				cairo_fill(canvas_cairo);
			else
				cairo_fill_preserve(canvas_cairo);
		}
		if(pStroke!=NULL)
		{
			set_stroke_style(canvas_cairo, pStroke);
			cairo_stroke(canvas_cairo);
		}

		return AG_SUCCESS;
	}

	void FerryRegionSymbolImpl::DrawIcon()
	{
		m_icon = cairo_image_surface_create(CAIRO_FORMAT_ARGB32, AUGE_ICON_SIZE, AUGE_ICON_SIZE);
		m_cairo = cairo_create(m_icon);

		double size_2 = AUGE_ICON_SIZE / 2.0f;
		double cx = size_2;
		double cy = size_2;
		cairo_save(m_cairo);
		cairo_translate (m_cairo, cx, cy);
		cairo_scale(m_cairo,AUGE_ICON_SCALE,AUGE_ICON_SCALE);
		//cairo_rotate (m_cairo, m_rotation*PI/180.0f);
		cairo_rectangle(m_cairo, -size_2, -size_2, AUGE_ICON_SIZE, AUGE_ICON_SIZE);
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


	void FerryRegionSymbolImpl::SaveIcon(const char* icon_path)
	{
		cairo_surface_write_to_png(m_icon, icon_path);
	}

	Symbol*	FerryRegionSymbolImpl::Clone()
	{
		return (new FerryRegionSymbolImpl());
	}
}