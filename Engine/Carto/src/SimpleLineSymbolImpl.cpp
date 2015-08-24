#include "AugeStyle.h"
#include "StrokeImpl.h"
#include "SimpleLineSymbolImpl.h"
#include "RendererCairo.h"

#include "string.h"

namespace auge
{

	SimpleLineSymbolImpl::SimpleLineSymbolImpl()
	{
		m_line_type = augeLineSimple;
		//m_pStroke = new StrokeImpl();
		m_pStroke = NULL;

		m_cairo = NULL;
		m_icon = NULL;

		m_icon_name = "SimpleLine.png";
	}

	SimpleLineSymbolImpl::~SimpleLineSymbolImpl()
	{	
		if(m_pStroke!=NULL)
		{
		 	AUGE_SAFE_RELEASE(m_pStroke);
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

	const char*	SimpleLineSymbolImpl::GetName()
	{
		return "SimpleLine";
	}

	const char*	SimpleLineSymbolImpl::GetIcon()
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

	bool SimpleLineSymbolImpl::IsGraphic()
	{
		return false;
	}

	augeSymbolizerType SimpleLineSymbolImpl::GetType()
	{
		return augeSymbolLine;
	}

	Stroke* SimpleLineSymbolImpl::GetStroke()
	{
		return m_pStroke;
	}
	
	void SimpleLineSymbolImpl::SetStroke(Stroke* pStroke)
	{
		if(m_pStroke)
		{
			AUGE_SAFE_RELEASE(m_pStroke);
		}
		m_pStroke = pStroke;
	}

	void SimpleLineSymbolImpl::Release()
	{
		if(!ReleaseRef())
		{
			delete this;
		}
	}

	//void SimpleLineSymbolImpl::SetLineType(augeLineType type)
	//{
	//	m_line_type = type;
	//}

	augeLineType SimpleLineSymbolImpl::GetLineType()
	{
		return m_line_type;
	}

	RESULTCODE SimpleLineSymbolImpl::Draw(Geometry* pGeometry, Renderer* pRenderer, Transformation* pTransform)
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
		
		RendererCairo* pRendererCairo = static_cast<RendererCairo*>(pRenderer);
		cairo_t			*canvas_cairo = pRendererCairo->GetCairo();
		cairo_surface_t	*canvas_surface=pRendererCairo->GetCairoSurface();

		GColor& color = m_pStroke->GetColor();
		double width  = m_pStroke->GetWidth();

		cairo_save(canvas_cairo);

		// draw black
		g_uchar* wkb = pGeometry->AsBinary();
		switch(geom_type)
		{
		case augeGTLineString:
			build_path(canvas_cairo, (WKBLineString*)wkb, pTransform);
			break;
		case augeGTMultiLineString:
			build_path(canvas_cairo, (WKBMultiLineString*)wkb, pTransform);
			break;
		}
		cairo_set_source_rgba(canvas_cairo,	color.GetRedF(), color.GetGreenF(), color.GetBlueF(), color.GetAlphaF());
		cairo_set_line_width(canvas_cairo, width);
		cairo_set_line_cap(canvas_cairo, CAIRO_LINE_CAP_ROUND);
		cairo_stroke(canvas_cairo);

		// draw white block
		cairo_restore(canvas_cairo);

		return AG_SUCCESS;
	} 

	void SimpleLineSymbolImpl::DrawIcon()
	{
		int icon_size = AUGE_ICON_SIZE;

		if(m_icon==NULL)
		{
			m_icon = cairo_image_surface_create(CAIRO_FORMAT_ARGB32, icon_size, icon_size);
			m_cairo = cairo_create(m_icon);
		} 

		double cy = icon_size>>1; 
		GColor& color = m_pStroke->GetColor();

		cairo_save(m_cairo);

		cairo_new_path(m_cairo);
		cairo_move_to(m_cairo, 0,			cy);
		cairo_line_to(m_cairo, icon_size,	cy);

		cairo_set_source_rgba(m_cairo,	color.GetRedF(), color.GetGreenF(), color.GetBlueF(), color.GetAlphaF());
		cairo_set_line_width(m_cairo, m_pStroke->GetWidth());
		cairo_set_line_cap(m_cairo, CAIRO_LINE_CAP_ROUND);
		cairo_set_line_join(m_cairo, CAIRO_LINE_JOIN_ROUND);
		cairo_stroke(m_cairo);

		cairo_restore(m_cairo);
	}

	void SimpleLineSymbolImpl::SaveIcon(const char* icon_path)
	{
		cairo_surface_write_to_png(m_icon, icon_path);
	}

	Symbol*	SimpleLineSymbolImpl::Clone()
	{
		return (new SimpleLineSymbolImpl());
	}
}
