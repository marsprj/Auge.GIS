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
	}

	RailwaySymbolImpl::~RailwaySymbolImpl()
	{
		if(m_pStroke!=NULL)
		{
			m_pStroke->Release();
			m_pStroke = NULL;
		}
	}

	//////////////////////////////////////////////////////////////////////////
	const char*	RailwaySymbolImpl::GetName()
	{
		return "Railway";
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
		double dash_offset = 1;//block_length;

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
		cairo_set_line_cap(canvas_cairo, CAIRO_LINE_CAP_ROUND);
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
		cairo_set_line_cap(canvas_cairo, CAIRO_LINE_CAP_ROUND);
		cairo_stroke(canvas_cairo);

		cairo_restore(canvas_cairo);

		return AG_SUCCESS;
	}
}