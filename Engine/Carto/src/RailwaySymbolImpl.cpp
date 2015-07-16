#include "RailwaySymbolImpl.h"
#include "RendererCairo.h"
#include "FillImpl.h"
#include "StrokeImpl.h"

namespace auge
{
	RailwaySymbolImpl::RailwaySymbolImpl()
	{
		m_block_width = 5;
		m_block_length= 10;
		m_pStroke = new StrokeImpl();

		m_cairo = NULL;
		m_icon = NULL;

		m_icon_name = "Railway.png";
	}

	RailwaySymbolImpl::~RailwaySymbolImpl()
	{
		if(m_pStroke!=NULL)
		{
			m_pStroke->Release();
			m_pStroke = NULL; 
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
	const char*	RailwaySymbolImpl::GetName()
	{
		return "Railway";
	}

	const char*	RailwaySymbolImpl::GetIcon()
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

	augeLineType RailwaySymbolImpl::GetLineType()
	{
		return augeLineRailway;
	}
	
	void RailwaySymbolImpl::SetStroke(Stroke* pStroke)
	{
		if(m_pStroke!=NULL)
		{
			m_pStroke->Release();
			m_pStroke = NULL;
		}
		m_pStroke = pStroke;
	}

	Stroke* RailwaySymbolImpl::GetStroke()
	{
		return m_pStroke;
	}

	void RailwaySymbolImpl::Release()
	{
		if(!ReleaseRef())
		{
			delete this;
		}
	}

	augeSymbolizerType RailwaySymbolImpl::GetType()
	{
		return augeSymbolPoint;
	}

	RESULTCODE RailwaySymbolImpl::Draw(Geometry* pGeometry, Renderer* pRenderer, Transformation* pTransform)
	{
		if(pGeometry==NULL)
		{
			return AG_FAILURE;
		}

		augeGeometryType geom_type = pGeometry->GeometryType();
		if((geom_type!=augeGTLineString) &&(geom_type!=augeGTMultiLineString))
		{
			return AG_FAILURE;
		}
		g_uchar* wkb = pGeometry->AsBinary();

		double dashes[] = { m_block_length,  /* ink */
							m_block_length,  /* skip */
		};
		int ndash = sizeof(dashes) / sizeof(double);
		double dash_offset = 1;//m_block_length;

		RendererCairo* pRendererCairo = static_cast<RendererCairo*>(pRenderer);
		cairo_t			*canvas_cairo = pRendererCairo->GetCairo();
		cairo_surface_t	*canvas_surface=pRendererCairo->GetCairoSurface();

		cairo_save(canvas_cairo);

		// draw black
		switch(geom_type)
		{
		case augeGTLineString:
			build_path(canvas_cairo, (WKBLineString*)wkb, pTransform);
			break;
		case augeGTMultiLineString:
			build_path(canvas_cairo, (WKBMultiLineString*)wkb, pTransform);
			break;
		}
		cairo_set_source_rgba(canvas_cairo,	0.0f, 0.0f, 0.0f, 1.0f);
		cairo_set_line_width(canvas_cairo, m_block_width);
		cairo_set_line_cap(canvas_cairo, CAIRO_LINE_CAP_BUTT);
		cairo_stroke(canvas_cairo);

		// draw white block
		switch(geom_type)
		{
		case augeGTLineString:
			build_path(canvas_cairo, (WKBLineString*)wkb, pTransform);
			break;
		case augeGTMultiLineString:
			build_path(canvas_cairo, (WKBMultiLineString*)wkb, pTransform);
			break;
		}
		cairo_set_source_rgba(canvas_cairo,	1.0f, 1.0f, 1.0f, 1.0f);
		cairo_set_line_width(canvas_cairo, m_block_width-1);
		cairo_set_dash (canvas_cairo, dashes, ndash, dash_offset);
		cairo_set_line_cap(canvas_cairo, CAIRO_LINE_CAP_BUTT);
		cairo_stroke(canvas_cairo);
		 
		cairo_restore(canvas_cairo);

		return AG_SUCCESS;
	} 
	 
	void RailwaySymbolImpl::DrawIcon()
	{
		int icon_size = AUGE_ICON_SIZE;
		int block_length = 11;
		int block_width  = 8;
		double dashes[] = { block_length,  /* ink */
			block_length,  /* skip */
		};
		int ndash = sizeof(dashes) / sizeof(double); 
		double dash_offset = block_length;

		if(m_icon==NULL)
		{
			m_icon = cairo_image_surface_create(CAIRO_FORMAT_ARGB32, icon_size, icon_size);
			m_cairo = cairo_create(m_icon);
		} 

		double cy = icon_size>>1; 

		cairo_save(m_cairo);
		 
		// build path
		cairo_new_path(m_cairo);
		cairo_move_to(m_cairo, 0,			cy);
		cairo_line_to(m_cairo, icon_size,	cy);

		cairo_set_source_rgba(m_cairo,	0.0f, 0.0f, 0.0f, 1.0f);
		//cairo_set_source_rgba(m_cairo,	1.0f, 1.0f, 1.0f, 1.0f);
		cairo_set_line_width(m_cairo, block_width);
		cairo_set_line_cap(m_cairo, CAIRO_LINE_CAP_BUTT);
		cairo_stroke(m_cairo);

		// draw white block
		cairo_new_path(m_cairo);
		cairo_move_to(m_cairo, 0,			cy);
		cairo_line_to(m_cairo, icon_size,	cy);

		cairo_set_source_rgba(m_cairo,	1.0f, 1.0f, 1.0f, 1.0f);
		//cairo_set_source_rgba(m_cairo,	0.0f, 0.0f, 0.0f, 1.0f);
		cairo_set_line_width(m_cairo, block_width-1);
		cairo_set_dash (m_cairo, dashes, ndash, dash_offset);
		cairo_set_line_cap(m_cairo, CAIRO_LINE_CAP_BUTT);
		cairo_stroke(m_cairo);

		cairo_restore(m_cairo);
	}

	void RailwaySymbolImpl::SaveIcon(const char* icon_path)
	{
		cairo_surface_write_to_png(m_icon, icon_path);
	}
}