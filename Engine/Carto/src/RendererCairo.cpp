#include "RendererCairo.h"

namespace auge
{
	void	build_path(cairo_t* cairo, WKBLineString		*pWKBLineString,		Transformation* pTransformation);
	void	build_path(cairo_t* cairo, WKBMultiLineString	*pWKBMultiLineString,	Transformation* pTransformation);
	void	build_path(cairo_t* cairo, WKBPolygon			*pWKBPolygon,			Transformation* pTransformation);
	void	build_path(cairo_t* cairo, WKBMultiPolygon		*pWKBPolygon,			Transformation* pTransformation);
	void	set_stroke_style(cairo_t* cairo, Stroke* pStroke);

	RendererCairo::RendererCairo(g_uint width, g_uint height)
	{
		m_cairo_surface = cairo_image_surface_create(CAIRO_FORMAT_ARGB32, width, height);
		m_cairo = cairo_create(m_cairo_surface);
		m_image_render = false;
	}

	RendererCairo::~RendererCairo()
	{
		if(m_cairo!=NULL)
		{
			cairo_destroy(m_cairo);
			if(!m_image_render)
			{
				cairo_surface_destroy(m_cairo_surface);
			}

			m_cairo = NULL;
			m_cairo_surface = NULL;
		}
	}

	void RendererCairo::Relaese()
	{
		delete this;
	}

	void RendererCairo::DrawRectangle(g_uint width, g_uint height, GColor& color, double border)
	{
		DrawRectangle(0,0,width, height,color, border);
	}

	void RendererCairo::FillRectangle(g_uint width, g_uint height, GColor& color)
	{
		FillRectangle(0,0,width, height,color);
	}

	void RendererCairo::DrawRectangle(g_uint x, g_uint y, g_uint width, g_uint height, GColor& color, double border)
	{
		
	}

	void RendererCairo::FillRectangle(g_uint x, g_uint y, g_uint width, g_uint height, GColor& color)
	{
		float r = color.GetRedF();
		cairo_set_source_rgba(m_cairo, color.GetRedF(), color.GetGreenF(), color.GetBlueF(), color.GetAlphaF());
		cairo_rectangle(m_cairo, 0, 0, width, height);
		cairo_fill(m_cairo);
	}

	bool RendererCairo::SaveAsImage(const char* szPath)
	{
		if(szPath==NULL)
		{
			return false;
		}
		cairo_surface_write_to_png(m_cairo_surface, szPath);
		return true;
	}

	void RendererCairo::DrawCircle(int cx, int cy, float radius, Stroke* pStroke)
	{
		if(pStroke==NULL)
		{
			return;
		}

		set_stroke_style(m_cairo, pStroke);

		cairo_arc(m_cairo, cx, cy, radius, 0, 2*PI);
		cairo_stroke(m_cairo);
	}

	void RendererCairo::FillCircle(int cx, int cy, float radius, Fill* pFill)
	{
		if(pFill==NULL)
		{
			return;
		}
		GColor& color = pFill->GetColor();
		cairo_set_source_rgba(m_cairo, color.GetRedF(), color.GetGreenF(), color.GetBlueF(), color.GetAlphaF());
		cairo_arc(m_cairo, cx, cy, radius, 0, 2*PI);
		cairo_fill(m_cairo);
	}

	void RendererCairo::Draw(g_uchar* wkb, Symbolizer* pSymbolizer, Transformation* pTransformation)
	{
		switch(WKBTYPE(wkb))
		{
		case wkbPoint:
			{
				if(pSymbolizer->GetType()==augeSymbolPoint)
				{
					DrawPoint((WKBPoint*)wkb, (PointSymbolizer*)pSymbolizer, pTransformation);
				}
			}
			break;
		case wkbMultiPoint:
			{
				if(pSymbolizer->GetType()==augeSymbolPoint)
				{
					DrawPoint((WKBMultiPoint*)wkb, (PointSymbolizer*)pSymbolizer, pTransformation);
				}
			}
			break;
		case wkbLineString:
			{
				if(pSymbolizer->GetType()==augeSymbolLine)
				{
					DrawLine((WKBLineString*)wkb, (LineSymbolizer*)pSymbolizer, pTransformation);
				}
			}
			break;
		case wkbMultiLineString:
			{
				if(pSymbolizer->GetType()==augeSymbolLine)
				{
					DrawLine((WKBMultiLineString*)wkb, (LineSymbolizer*)pSymbolizer, pTransformation);
				}
			}
			break;
		case wkbPolygon:
			{
				if(pSymbolizer->GetType()==augeSymbolPolygon)
				{
					DrawPolygon((WKBPolygon*)wkb, (PolygonSymbolizer*)pSymbolizer, pTransformation);
				}
			}
			break;
		case wkbMultiPolygon:
			{
				if(pSymbolizer->GetType()==augeSymbolPolygon)
				{
					DrawPolygon((WKBMultiPolygon*)wkb, (PolygonSymbolizer*)pSymbolizer, pTransformation);
				}
			}
			break;
		}
	}

	void RendererCairo::DrawPoint(WKBPoint	*pWKBPoint,	PointSymbolizer* pSymbolizer, Transformation* pTransformation)
	{
		float radius = pSymbolizer->GetSize();
		Fill   *pFill = pSymbolizer->GetFill();
		Stroke *pStroke = pSymbolizer->GetStroke();

		int sx=0, sy=0;
		pTransformation->ToScreenPoint(pWKBPoint->point.x, pWKBPoint->point.y, sx, sy);

		cairo_arc(m_cairo, sx, sy, radius, 0, 2*PI);
		if(pFill!=NULL)
		{
			GColor& color = pFill->GetColor();
			cairo_set_source_rgba(m_cairo, color.GetRedF(), color.GetGreenF(), color.GetBlueF(), color.GetAlphaF());
			if(pStroke==NULL)
			{
				cairo_fill(m_cairo);
			}
			else
			{
				cairo_fill_preserve(m_cairo);
			}
		}
		if(pStroke!=NULL)
		{
			set_stroke_style(m_cairo, pStroke);
			cairo_stroke(m_cairo);
		}
	}

	void RendererCairo::DrawPoint(WKBMultiPoint *pWKBMultiPoint, PointSymbolizer* pPointSymbolizer, Transformation* pTransformation)
	{
		g_uint numPoints = pWKBMultiPoint->numPoints;
		WKBPoint* pWKBPoint = NULL;
		pWKBPoint = (WKBPoint*)(&(pWKBMultiPoint->points[0]));
		for(g_uint i=0; i<numPoints; i++, pWKBPoint++)
		{
			DrawPoint(pWKBPoint, pPointSymbolizer, pTransformation);
		}
	}

	void RendererCairo::DrawLine(WKBLineString	*pWKBLineString, Stroke* pStroke, Transformation* pTransformation)
	{
		build_path(m_cairo, pWKBLineString, pTransformation);
		set_stroke_style(m_cairo, pStroke);
		cairo_stroke(m_cairo);
	}

	void RendererCairo::DrawLine(WKBMultiLineString	*pWKBMultiLineString, Stroke* pStroke, Transformation* pTransformation)
	{
		build_path(m_cairo, pWKBMultiLineString, pTransformation);
		set_stroke_style(m_cairo, pStroke);
		cairo_stroke(m_cairo);
	}

	void RendererCairo::DrawLine(WKBLineString *pWKBLineString,	LineSymbolizer* pSymbolizer, Transformation* pTransformation)
	{
		Stroke* pStroke = pSymbolizer->GetStroke();
		build_path(m_cairo, pWKBLineString, pTransformation);
		set_stroke_style(m_cairo, pStroke);
		cairo_stroke(m_cairo);
	}

	void RendererCairo::DrawLine(WKBMultiLineString	*pWKBMultiLineString, LineSymbolizer* pSymbolizer, Transformation* pTransformation)
	{
		Stroke* pStroke = pSymbolizer->GetStroke();
		build_path(m_cairo, pWKBMultiLineString, pTransformation);
		set_stroke_style(m_cairo, pStroke);
		cairo_stroke(m_cairo);
	}

	void RendererCairo::DrawPolygon(WKBPolygon *pWKBPolygon, PolygonSymbolizer* pPolygonSymbolizer, Transformation* pTransformation)
	{
		Fill	*pFill = pPolygonSymbolizer->GetFill();
		Stroke	*pStroke = pPolygonSymbolizer->GetStroke();

		cairo_new_path(m_cairo);
		build_path(m_cairo, pWKBPolygon,pTransformation);

		if(pFill!=NULL)
		{
			GColor& color = pFill->GetColor();
			cairo_set_source_rgba(m_cairo, color.GetRedF(), color.GetGreenF(), color.GetBlueF(), color.GetAlphaF());
			if(pStroke==NULL)
				cairo_fill(m_cairo);
			else
				cairo_fill_preserve(m_cairo);
		}
		if(pStroke!=NULL)
		{
			set_stroke_style(m_cairo, pStroke);
			cairo_stroke(m_cairo);
		}
	}

	void RendererCairo::DrawPolygon(WKBMultiPolygon	*pWKBMultiPolygon, PolygonSymbolizer* pPolygonSymbolizer, Transformation* pTransformation)
	{
		Fill	*pFill = pPolygonSymbolizer->GetFill();
		Stroke	*pStroke = pPolygonSymbolizer->GetStroke();

		cairo_new_path(m_cairo);
		build_path(m_cairo, pWKBMultiPolygon,pTransformation);

		if(pFill!=NULL)
		{
			GColor& color = pFill->GetColor();
			cairo_set_source_rgba(m_cairo, color.GetRedF(), color.GetGreenF(), color.GetBlueF(), color.GetAlphaF());
			if(pStroke==NULL)
				cairo_fill(m_cairo);
			else
				cairo_fill_preserve(m_cairo);
		}
		if(pStroke!=NULL)
		{
			set_stroke_style(m_cairo, pStroke);
			cairo_stroke(m_cairo);
		}
	}

	void RendererCairo::Label(const g_char* text, WKBPoint *pWKBPoint, TextSymbolizer* pSymbolizer, Transformation* pTransformation)
	{
		int sx=0,sy=0;
		pTransformation->ToScreenPoint(pWKBPoint->point.x,pWKBPoint->point.y,sx,sy);

		const g_char* utext = auge_encoding_convert("GBK","UTF-8",text, strlen(text));

		Font* pFont = pSymbolizer->GetFont();
		//cairo_select_font_face(m_cairo, "Sazanami Gothic", CAIRO_FONT_SLANT_NORMAL, CAIRO_FONT_WEIGHT_BOLD);
		cairo_select_font_face(m_cairo, "Microsoft Yahei", CAIRO_FONT_SLANT_NORMAL, CAIRO_FONT_WEIGHT_BOLD);
		//cairo_select_font_face(m_cairo, pFont->GetFamily(), (cairo_font_slant_t)(pFont->GetStyle()), (cairo_font_weight_t)(pFont->GetWeight()));
		//cairo_set_font_size(m_cairo, 30.0);
		cairo_set_font_size(m_cairo, pFont->GetSize());

		cairo_move_to(m_cairo, sx, sy);
		cairo_show_text(m_cairo, utext);

		Fill	*pFill = pSymbolizer->GetFill();
		Stroke	*pStroke = pSymbolizer->GetStroke();
		if(pFill!=NULL)
		{
			GColor& color = pFill->GetColor();
			cairo_set_source_rgba(m_cairo, color.GetRedF(), color.GetGreenF(), color.GetBlueF(), color.GetAlphaF());
			if(pStroke==NULL)
				cairo_fill(m_cairo);
			else
				cairo_fill_preserve(m_cairo);
		}
		if(pStroke!=NULL)
		{
			set_stroke_style(m_cairo, pStroke);
			cairo_stroke(m_cairo);
		}
	}
}

namespace auge
{
	//////////////////////////////////////////////////////////////////////////
	// Build Path Begin
	//////////////////////////////////////////////////////////////////////////
	void build_path(cairo_t* cairo, WKBLineString* pWKBLineString, Transformation* pTransformation)
	{
		int numPoints = pWKBLineString->numPoints;
		if(numPoints>0)
		{
			int i = 0;
			int sx=0, sy=0;
			auge::Point *pt = (auge::Point*)(&(pWKBLineString->points[0]));

			pTransformation->ToScreenPoint(pt->x, pt->y, sx, sy);
			cairo_move_to(cairo, sx, sy);

			//cairo_move_to(cairo, pt->x, pt->y);
			for(i=1, pt++; i<numPoints; i++, pt++)
			{
				pTransformation->ToScreenPoint(pt->x, pt->y, sx, sy);
				cairo_line_to(cairo, sx, sy);

				//cairo_line_to(cairo, pt->x, pt->y);
			}
		}
	}

	void build_path(cairo_t* cairo, WKBMultiLineString* pWKBMultiLineString, Transformation* pTransformation)
	{
		int i=0,j=0;
		int numPoints = 0;
		int numLineStings = 0;
		auge::Point *pt = NULL;
		WKBLineString* pWKBLineString = NULL;
		numLineStings = pWKBMultiLineString->numLineStrings;

		int sx=0, sy=0;

		pWKBLineString = (WKBLineString*)(&(pWKBMultiLineString->lineStrings[0]));
		for(i=0; i<numLineStings; i++)
		{
			numPoints = pWKBLineString->numPoints;
			pt = (auge::Point*)(&(pWKBLineString->points[0]));

			pTransformation->ToScreenPoint(pt->x, pt->y, sx, sy);
			cairo_move_to(cairo, sx, sy);
			//cairo_move_to(cairo, pt->x, pt->y);

			for(j=1,pt++; j<numPoints; j++, pt++)
			{
				pTransformation->ToScreenPoint(pt->x, pt->y, sx, sy);
				cairo_line_to(cairo, sx, sy);
			}
			pWKBLineString = (WKBLineString*)(pt);
		}
	}

	void build_path(cairo_t* cairo, WKBPolygon* pWKBPolygon, Transformation* pTransformation)
	{
		int i=0, j=0, end=0;
		int sx=0, sy=0;
		int numPoints = 0;
		int numRings  = pWKBPolygon->numRings;
		auge::LinearRing* pLinearRing = NULL;
		auge::Point *pt = NULL;

		pLinearRing = (auge::LinearRing*)(&(pWKBPolygon->rings[0]));

		for(i=0; i<numRings; i++)
		{
			numPoints = pLinearRing->numPoints;
			pt = (auge::Point*)(&(pLinearRing->points[0]));

			pTransformation->ToScreenPoint(pt->x, pt->y, sx, sy);
			cairo_move_to(cairo, sx, sy);
			for(j=1, pt++; j<numPoints; j++, pt++)
			{
				pTransformation->ToScreenPoint(pt->x, pt->y, sx, sy);
				cairo_line_to(cairo, sx, sy);

			}
			cairo_close_path(cairo);
			pLinearRing = (auge::LinearRing*)(pt);
		}
	}

	void build_path(cairo_t* cairo, WKBMultiPolygon* pMultiWKBPolygon, Transformation* pTransformation)
	{
		int i=0,j=0,k=0,end=0;
		int sx=0, sy=0;
		int numPolygons = 0;
		int numRings = 0;
		int numPoints = 0;
		auge::Point *pt = NULL;
		auge::LinearRing *pLinearRing = NULL;
		auge::WKBPolygon *pWKBPolygon = NULL;

		numPolygons = pMultiWKBPolygon->numPolygons;
		pWKBPolygon = (auge::WKBPolygon*)(&(pMultiWKBPolygon->polygons[0]));

		for(i=0; i<numPolygons; i++)
		{
			numRings = pWKBPolygon->numRings;
			pLinearRing = (auge::LinearRing*)(&(pWKBPolygon->rings[0]));
			for(j=0; j<numRings; j++)
			{
				numPoints = pLinearRing->numPoints;
				if(numPoints>0)
				{
					numPoints = pLinearRing->numPoints;
					end = numPoints-1;
					pt = (auge::Point*)(&(pLinearRing->points[0]));

					pTransformation->ToScreenPoint(pt->x, pt->y, sx, sy);
					cairo_move_to(cairo, sx, sy);

					for(k=1, pt++; k<numPoints; k++,pt++)
					{
						pTransformation->ToScreenPoint(pt->x, pt->y, sx, sy);
						cairo_line_to(cairo, sx, sy);
					}
					cairo_close_path(cairo);
					pLinearRing = (auge::LinearRing*)(pt);
				}
			}
			pWKBPolygon = (WKBPolygon*)(pLinearRing);
		}
	}

	void set_stroke_style(cairo_t* cairo, Stroke* pStroke)
	{
		//设置线颜色
		GColor& color = pStroke->GetColor();
		cairo_set_source_rgba(cairo,	color.GetRedF(), color.GetGreenF(), color.GetBlueF(), color.GetAlphaF());
		//设置
		float width = pStroke->GetWidth();
		cairo_set_line_width(cairo, width);
		//设置cap
		cairo_set_line_cap(cairo, CAIRO_LINE_CAP_ROUND);
	}
	//////////////////////////////////////////////////////////////////////////
	// Build Path End
	//////////////////////////////////////////////////////////////////////////
}