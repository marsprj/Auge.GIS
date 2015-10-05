#include "RendererCairo.h"
#include "AugeRaster.h"

namespace auge
{
	const g_uint g_cairo_bands = 4;
	
	//void	build_path(cairo_t* cairo, WKBLineString		*pWKBLineString,		Transformation* pTransformation);
	//void	build_path(cairo_t* cairo, WKBMultiLineString	*pWKBMultiLineString,	Transformation* pTransformation);
	//void	build_path(cairo_t* cairo, WKBPolygon			*pWKBPolygon,			Transformation* pTransformation);
	//void	build_path(cairo_t* cairo, WKBMultiPolygon		*pWKBPolygon,			Transformation* pTransformation);
	//void	set_stroke_style(cairo_t* cairo, Stroke* pStroke);

	RendererCairo::RendererCairo(g_uint width, g_uint height)
	{
		m_cairo_surface = cairo_image_surface_create(CAIRO_FORMAT_ARGB32, width, height);
		m_cairo = cairo_create(m_cairo_surface);
		m_image_render = false;
		m_width = width;
		m_height = height;
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

	cairo_t* RendererCairo::GetCairo()
	{
		return m_cairo;
	}

	cairo_surface_t* RendererCairo::GetCairoSurface()
	{
		return m_cairo_surface;
	}

	void RendererCairo::Save()
	{
		cairo_save(m_cairo);
	}

	void RendererCairo::Resotre()
	{
		cairo_restore(m_cairo);
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
		float g = color.GetGreenF();
		float b = color.GetBlueF();
		float a = color.GetAlphaF();
		//cairo_set_source_rgba(m_cairo, color.GetRedF(), color.GetGreenF(), color.GetBlueF(), color.GetAlphaF());
		//cairo_set_source_rgba(m_cairo, r, g, b, 1.0f);
		cairo_set_source_rgba(m_cairo, r, g, b, a);
		cairo_rectangle(m_cairo, x, y, width, height);
		cairo_fill(m_cairo);
	}

	RESULTCODE RendererCairo::SaveAsImage(const char* szPath)
	{
		if(szPath==NULL)
		{
			return AG_FAILURE;
		}
		cairo_status_t status = cairo_surface_write_to_png(m_cairo_surface, szPath);
		char msg[AUGE_MSG_MAX];
		sprintf(msg,"%d",int(status));
		if(status!=CAIRO_STATUS_SUCCESS)
		{
			return AG_FAILURE;
		}
		return AG_SUCCESS;
	}

	RESULTCODE RendererCairo::SaveAsImage(g_uint x, g_uint y, g_uint width, g_uint height, const char* path)
	{


		return AG_SUCCESS;
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
		Graphic* pGraphic = pSymbolizer->GetGraphic();
		if(pGraphic)
		{
			DrawGraphicPoint(pWKBPoint, pSymbolizer, pTransformation);
		}
		else
		{
			DrawGeometryPoint(pWKBPoint, pSymbolizer, pTransformation);
		}
	}

	void RendererCairo::DrawGraphicPoint(WKBPoint	*pWKBPoint,	PointSymbolizer* pSymbolizer, Transformation* pTransformation)
	{
		//Graphic* pGraphic = pSymbolizer->GetGraphic(); 
		//ExternalGraphic* pExternalGraphic = pGraphic->GetExternalGraphic();
		//if(pExternalGraphic!=NULL)
		//{
		//	const char* resource = pExternalGraphic->GetResource();
		//	if(resource)
		//	{
		//		cairo_surface_t *image = cairo_image_surface_create_from_png(resource);
		//		if(image!=NULL)
		//		{
		//			int sx=0, sy=0;
		//			float size = pSymbolizer->GetSize();					
		//			pTransformation->ToScreenPoint(pWKBPoint->point.x, pWKBPoint->point.y, sx, sy);

		//			int w = cairo_image_surface_get_width(image);
		//			int h = cairo_image_surface_get_height(image);

		//			cairo_save(m_cairo);
		//			cairo_translate(m_cairo, sx, sy);
		//			cairo_scale  (m_cairo, size/w, size/h);
		//			cairo_set_source_surface(m_cairo, image, 0,0);
		//			cairo_paint(m_cairo);
		//			cairo_surface_flush(m_cairo_surface);
		//			cairo_surface_destroy(image);
		//			cairo_restore(m_cairo);
		//		}
		//	}
		//}
	}

	// @ deprecated
	void RendererCairo::DrawGeometryPoint(WKBPoint	*pWKBPoint,	PointSymbolizer* pSymbolizer, Transformation* pTransformation)
	{
		//float size = pSymbolizer->GetSize();
		//float rotation = pSymbolizer->GetRotation();
		//Fill   *pFill  = pSymbolizer->GetFill();
		//Stroke *pStroke= pSymbolizer->GetStroke();

		//int sx=0, sy=0;
		//pTransformation->ToScreenPoint(pWKBPoint->point.x, pWKBPoint->point.y, sx, sy);

		//augeMarkerType type = pSymbolizer->GetMarkType();
		//switch(type)
		//{
		//case augeMarkerSquare:
		//	DrawSquare(sx, sy, size, rotation, pFill, pStroke);
		//	break;
		//case augeMarkerCircle:
		//	DrawCircle(sx, sy, size/2, pFill, pStroke);
		//	break;
		//case augeMarkerTriangle:
		//	DrawsTriangle(sx,sy,size, rotation, pFill, pStroke);
		//	break;
		//case augeMarkerStar:
		//	DrawStar(sx, sy, size, rotation, pFill, pStroke);
		//	break;
		//case augeMarkerCross:
		//	break;
		//case augeMarkerX:
		//	break;
		//case augeMarkerPentagon:
		//	DrawPentagon(sx, sy, size, rotation, pFill, pStroke);
		//	break;
		//case augeMarkerCapital:
		//	DrawCapital(sx, sy, size, pFill, pStroke);
		//}
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

	//void RendererCairo::DrawLine(WKBLineString *pWKBLineString,	LineSymbolizer* pSymbolizer, Transformation* pTransformation)
	//{
	//	Stroke* pStroke = pSymbolizer->GetStroke();
	//	build_path(m_cairo, pWKBLineString, pTransformation);
	//	set_stroke_style(m_cairo, pStroke);
	//	cairo_stroke(m_cairo);
	//}

	// @ deprecated
	void RendererCairo::DrawLine(WKBLineString *pWKBLineString,	LineSymbolizer* pSymbolizer, Transformation* pTransformation)
	{
		//augeLineType line_type = pSymbolizer->GetLineType();
		//switch(line_type)
		//{
		//case augeLineRailway:
		//	{
		//		DrawRailway(pWKBLineString, pTransformation);
		//	}
		//	break;
		//default:
		//	{
		//		Stroke* pStroke = pSymbolizer->GetStroke();
		//		build_path(m_cairo, pWKBLineString, pTransformation);
		//		set_stroke_style(m_cairo, pStroke);
		//		cairo_stroke(m_cairo);
		//	}
		//}
	}

	//void RendererCairo::DrawLine(WKBMultiLineString	*pWKBMultiLineString, LineSymbolizer* pSymbolizer, Transformation* pTransformation)
	//{
	//	Stroke* pStroke = pSymbolizer->GetStroke();
	//	build_path(m_cairo, pWKBMultiLineString, pTransformation);
	//	set_stroke_style(m_cairo, pStroke);
	//	cairo_stroke(m_cairo);
	//}

	// @ deprecated
	void RendererCairo::DrawLine(WKBMultiLineString	*pWKBMultiLineString, LineSymbolizer* pSymbolizer, Transformation* pTransformation)
	{
		//augeLineType line_type = pSymbolizer->GetLineType();
		//switch(line_type)
		//{
		//case augeLineRailway:
		//	{
		//		DrawRailway(pWKBMultiLineString, pTransformation);
		//	}
		//	break;
		//default:
		//	{
		//		Stroke* pStroke = pSymbolizer->GetStroke();
		//		build_path(m_cairo, pWKBMultiLineString, pTransformation);
		//		set_stroke_style(m_cairo, pStroke);
		//		cairo_stroke(m_cairo);
		//	}
		//}
	}

	// @deprecated
	void RendererCairo::DrawPolygon(WKBPolygon *pWKBPolygon, PolygonSymbolizer* pPolygonSymbolizer, Transformation* pTransformation)
	{
		//Fill	*pFill = pPolygonSymbolizer->GetFill();
		//Stroke	*pStroke = pPolygonSymbolizer->GetStroke();

		//cairo_new_path(m_cairo);
		//build_path(m_cairo, pWKBPolygon,pTransformation);

		//if(pFill!=NULL)
		//{
		//	GColor& color = pFill->GetColor();
		//	cairo_set_source_rgba(m_cairo, color.GetRedF(), color.GetGreenF(), color.GetBlueF(), color.GetAlphaF());
		//	if(pStroke==NULL)
		//		cairo_fill(m_cairo);
		//	else
		//		cairo_fill_preserve(m_cairo);
		//}
		//if(pStroke!=NULL)
		//{
		//	set_stroke_style(m_cairo, pStroke);
		//	cairo_stroke(m_cairo);
		//}
	}

	// @deprecated
	void RendererCairo::DrawPolygon(WKBMultiPolygon	*pWKBMultiPolygon, PolygonSymbolizer* pPolygonSymbolizer, Transformation* pTransformation)
	{
		//Fill	*pFill = pPolygonSymbolizer->GetFill();
		//Stroke	*pStroke = pPolygonSymbolizer->GetStroke();

		//cairo_new_path(m_cairo);
		//build_path(m_cairo, pWKBMultiPolygon,pTransformation);

		//if(pFill!=NULL)
		//{
		//	GColor& color = pFill->GetColor();
		//	cairo_set_source_rgba(m_cairo, color.GetRedF(), color.GetGreenF(), color.GetBlueF(), color.GetAlphaF());
		//	if(pStroke==NULL)
		//		cairo_fill(m_cairo);
		//	else
		//		cairo_fill_preserve(m_cairo);
		//}
		//if(pStroke!=NULL)
		//{
		//	set_stroke_style(m_cairo, pStroke);
		//	cairo_stroke(m_cairo);
		//}
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

		//modify anchor
		cairo_text_extents_t t_extents;
		cairo_text_extents (m_cairo, utext, &t_extents);
		double ax = pSymbolizer->GetAnchorX();
		double ay = pSymbolizer->GetAnchorY();
		sx -= ax *			t_extents.width;
		sy += ay * t_extents.height;

		//// modify displacement
		//sx += pSymbolizer->GetDisplacementX();
		//sy += pSymbolizer->GetDisplacementY();

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

	void RendererCairo::DrawText(const char* text, int x, int y, TextSymbolizer* pSymbolizer)
	{
		const g_char* utext = auge_encoding_convert("GBK","UTF-8",text, strlen(text));
		DrawText((g_uchar*)utext, x, y, pSymbolizer);
	}

	void RendererCairo::DrawText(const g_uchar* text, int x, int y, TextSymbolizer* pSymbolizer)
	{
		g_char* utext = (g_char*)text;
		Font* pFont = pSymbolizer->GetFont();
		//cairo_select_font_face(m_cairo, "Sazanami Gothic", CAIRO_FONT_SLANT_NORMAL, CAIRO_FONT_WEIGHT_BOLD);
		//cairo_select_font_face(m_cairo, "Microsoft Yahei", CAIRO_FONT_SLANT_NORMAL, CAIRO_FONT_WEIGHT_BOLD);

		const char* family = pFont->GetFamily();
		cairo_font_slant_t slant = (cairo_font_slant_t)pFont->GetSlant();
		cairo_font_weight_t weight = (cairo_font_weight_t)pFont->GetWeight();
		cairo_select_font_face(m_cairo, family, slant, weight);
		cairo_set_font_size(m_cairo, pFont->GetSize());

		cairo_move_to(m_cairo, x, y);
		cairo_text_path(m_cairo, utext);

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

	void RendererCairo::SetFont(const char* family, float size, augeFontSlant slant, augeFontWeight weight)
	{
		cairo_select_font_face(m_cairo, family, (cairo_font_slant_t)slant, (cairo_font_weight_t)weight);
		//cairo_select_font_face(m_cairo, "Microsoft Yahei", CAIRO_FONT_SLANT_NORMAL, CAIRO_FONT_WEIGHT_BOLD);
		cairo_set_font_size(m_cairo, size);
	}

	bool RendererCairo::GetTextExtent(const char* text, GEnvelope& extent)
	{
		cairo_text_extents_t caro_extents;
		cairo_text_extents (m_cairo, text, &caro_extents);
		extent.Set(0, 0, caro_extents.width, caro_extents.height);
		return true;
	}

	bool RendererCairo::DrawColorMap(ColorMap* pColorMap)
	{
		if(pColorMap==NULL)
		{
			return false;
		}

		GColor& start = pColorMap->GetStartColor();
		GColor& end = pColorMap->GetEndColor();

		g_byte r=0, g=0, b=0;
		float h=0.0f, s=0.0f, v=0.0f;
		float h_s=0.0f, s_s=0.0f, v_s=0.0f;
		float h_e=0.0f, s_e=0.0f, v_e=0.0f;

		auge_rgb_2_hsv(start.GetRed(), start.GetGreen(), start.GetBlue(), h_s, s_s, v_s);
		auge_rgb_2_hsv(end.GetRed(), end.GetGreen(), end.GetBlue(), h_e, s_e, v_e);

		float h_span = (h_e - h_s) / (m_width-1);
		float s_span = (s_e - s_s) / (m_width-1);
		float v_span = (v_e - v_s) / (m_width-1);

		for(int i=0; i<m_width-1; i++)
		{
			auge_hsv_2_rgb(h_s+h_span*i, s_s+s_span*i, v_s+v_span*i, r, g, b);
			GColor color(r,g,b,255);
			FillRectangle(i, 0, 1, m_height, color);
		}

		return true;
	}

	//bool RendererCairo::DrawColorMap(ColorMap* pColorMap)
	//{
	//	if(pColorMap==NULL)
	//	{
	//		return false;
	//	}

	//	int count = pColorMap->GetCount();
	//	if(count==0)
	//	{
	//		return false;
	//	}
	//	float span = (float)(m_width) / (float)(count);
	//	for(int i=0; i<count; i++)
	//	{
	//		GColor* color = pColorMap->GetColor(i);
	//		FillRectangle(span*i, 0, span, m_height, *color);
	//	}
	//}

	//////////////////////////////////////////////////////////////////////////
	// Draw Marker Begin
	//////////////////////////////////////////////////////////////////////////
	void RendererCairo::DrawCircle(int cx, int cy, float radius, Fill* pFill, Stroke* pStroke)
	{
		cairo_save(m_cairo);
		cairo_arc(m_cairo, cx, cy, radius, 0, 2*PI);
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
		cairo_restore(m_cairo);
	}

	void RendererCairo::DrawSquare(int cx, int cy, float size, float rotation, Fill* pFill, Stroke* pStroke)
	{
		float size_2 = size / 2;

		cairo_save(m_cairo);
		cairo_translate (m_cairo, cx, cy);
		cairo_rotate (m_cairo, rotation*PI/180.0f);
		cairo_rectangle(m_cairo, -size_2, -size_2, size, size);
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
		cairo_restore(m_cairo);
	}

	void RendererCairo::DrawStar(int cx, int cy, float size, float rotation, Fill* pFill, Stroke* pStroke)
	{
		static double g_auge_marker_stat_points[][2] = { 
			{ 0.000000, -1.000000},
			{ 0.224514, -0.309017},	//inner
			{ 0.951057, -0.309017},
			{ 0.363271,  0.118034},	//inner
			{ 0.587785,  0.809017},
			{ 0.000000,  0.381966},	//inner
			{-0.587785,  0.809017},
			{-0.363271,  0.118034},	//inner
			{-0.951057, -0.309017},
			{-0.224514, -0.309017},	//inner
			{ 0.000000, -1.000000}
		};

		size_t count = sizeof(g_auge_marker_stat_points) / sizeof(double) / 2;
		cairo_save(m_cairo);
		cairo_new_path(m_cairo);

		cairo_translate (m_cairo, cx, cy);
		//cairo_scale(m_cairo,10,10);
		cairo_rotate (m_cairo, rotation*PI/180.0f);
		
		double offset_x=0, offset_y=0;
		double x, y;
		x = g_auge_marker_stat_points[0][0];
		y = g_auge_marker_stat_points[0][1];
		cairo_move_to(m_cairo, x*size, y*size);
		for(size_t i=1; i<count; i++)
		{
			x = g_auge_marker_stat_points[i][0];
			y = g_auge_marker_stat_points[i][1];
			cairo_line_to(m_cairo, x*size, y*size);
		}
		cairo_close_path(m_cairo);

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
		cairo_restore(m_cairo);
	}

	void RendererCairo::DrawPentagon(int cx, int cy, float size, float rotation, Fill* pFill, Stroke* pStroke)
	{
		static double g_auge_marker_stat_points[][2] = { 
			{ 0.000000, -1.000000},
			{ 0.951057, -0.309017},
			{ 0.587785,  0.809017},
			{-0.587785,  0.809017},
			{-0.951057, -0.309017},
			{ 0.000000, -1.000000}
		};

		size_t count = sizeof(g_auge_marker_stat_points) / sizeof(double) / 2;
		cairo_save(m_cairo);
		cairo_new_path(m_cairo);

		cairo_translate (m_cairo, cx, cy);
		//cairo_scale(m_cairo,10,10);
		cairo_rotate (m_cairo, rotation*PI/180.0f);

		double offset_x=0, offset_y=0;
		double x, y;
		x = g_auge_marker_stat_points[0][0];
		y = g_auge_marker_stat_points[0][1];
		cairo_move_to(m_cairo, x*size, y*size);
		for(size_t i=1; i<count; i++)
		{
			x = g_auge_marker_stat_points[i][0];
			y = g_auge_marker_stat_points[i][1];
			cairo_line_to(m_cairo, x*size, y*size);
		}
		cairo_close_path(m_cairo);

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
		cairo_restore(m_cairo);
	}

	void RendererCairo::DrawCapital(int cx, int cy, float size, Fill* pFill, Stroke* pStroke)
	{
		float radius = size / 2;
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
	}

	void RendererCairo::DrawsTriangle(int cx, int cy, float size, float rotation, Fill* pFill, Stroke* pStroke)
	{
		static double g_auge_marker_stat_points[][2] = { 
			{ 0.000000, -1.000000},
			{-0.866025,  0.500000},
			{ 0.866025,  0.500000},
			{ 0.000000, -1.000000}
		};

		size_t count = sizeof(g_auge_marker_stat_points) / sizeof(double) / 2;
		cairo_save(m_cairo);
		cairo_new_path(m_cairo);

		cairo_translate (m_cairo, cx, cy);
		//cairo_scale(m_cairo,10,10);
		cairo_rotate (m_cairo, rotation*PI/180.0f);

		double offset_x=0, offset_y=0;
		double x, y;
		x = g_auge_marker_stat_points[0][0];
		y = g_auge_marker_stat_points[0][1];
		cairo_move_to(m_cairo, x*size, y*size);
		for(size_t i=1; i<count; i++)
		{
			x = g_auge_marker_stat_points[i][0];
			y = g_auge_marker_stat_points[i][1];
			cairo_line_to(m_cairo, x*size, y*size);
		}
		cairo_close_path(m_cairo);

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
		cairo_restore(m_cairo);
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

	void RendererCairo::FillCircle(int cx, int cy, float size, Fill* pFill)
	{
		if(pFill==NULL)
		{
			return;
		}
		float size_2 = size / 2;
		GColor& color = pFill->GetColor();
		cairo_set_source_rgba(m_cairo, color.GetRedF(), color.GetGreenF(), color.GetBlueF(), color.GetAlphaF());
		cairo_rectangle(m_cairo, cx-size_2, cy-size_2, size, size);
		cairo_fill(m_cairo);
	}

	void RendererCairo::DrawSquare(int cx, int cy, float size, Stroke* pStroke)
	{
		if(pStroke==NULL)
		{
			return;
		}
		float size_2 = size / 2;
		set_stroke_style(m_cairo, pStroke);
		cairo_rectangle(m_cairo, cx-size_2, cy-size_2, size, size);
		cairo_stroke(m_cairo);
	}

	void RendererCairo::FillSquare(int cx, int cy, float radius, Fill* pFill)
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
	//////////////////////////////////////////////////////////////////////////
	// Draw Marker End
	//////////////////////////////////////////////////////////////////////////

	//////////////////////////////////////////////////////////////////////////
	// Draw Line End
	//////////////////////////////////////////////////////////////////////////
	void RendererCairo::DrawRailway(WKBLineString *pWKBLineString, Transformation* pTransformation)
	{
		int block_width = 5;
		int block_length= 10;

		double dashes[] = { block_length,  /* ink */
			block_length,  /* skip */
		};
		int ndash = sizeof(dashes) / sizeof(double);
		double dash_offset = 1;//block_length;

		cairo_save(m_cairo);
		
		// draw black
		build_path(m_cairo, pWKBLineString, pTransformation);
		cairo_set_line_cap(m_cairo, CAIRO_LINE_CAP_BUTT);
		cairo_set_source_rgba(m_cairo,	0.0f, 0.0f, 0.0f, 1.0f);
		cairo_set_line_width(m_cairo, block_width);
		cairo_stroke(m_cairo);

		// draw white block
		build_path(m_cairo, pWKBLineString, pTransformation);
		cairo_set_source_rgba(m_cairo,	1.0f, 1.0f, 1.0f, 1.0f);
		cairo_set_line_width(m_cairo, block_width-1);
		cairo_set_dash (m_cairo, dashes, ndash, dash_offset);
		cairo_stroke(m_cairo);

		cairo_restore(m_cairo);
	}

	void RendererCairo::DrawRailway(WKBMultiLineString *pWKBMultiLineString, Transformation* pTransformation)
	{
		int block_width = 5;
		int block_length= 10;

		double dashes[] = { block_length,  /* ink */
			block_length,  /* skip */
		};
		int ndash = sizeof(dashes) / sizeof(double);
		double dash_offset = 1;//block_length;

		cairo_save(m_cairo);

		// draw black
		build_path(m_cairo, pWKBMultiLineString, pTransformation);
		cairo_set_line_cap(m_cairo, CAIRO_LINE_CAP_BUTT);
		cairo_set_source_rgba(m_cairo,	0.0f, 0.0f, 0.0f, 1.0f);
		cairo_set_line_width(m_cairo, block_width);
		cairo_stroke(m_cairo);

		// draw white block
		build_path(m_cairo, pWKBMultiLineString, pTransformation);
		cairo_set_source_rgba(m_cairo,	1.0f, 1.0f, 1.0f, 1.0f);
		cairo_set_line_width(m_cairo, block_width-1);
		cairo_set_dash (m_cairo, dashes, ndash, dash_offset);
		cairo_stroke(m_cairo);

		cairo_restore(m_cairo);
	}

	//void RendererCairo::DrawRailway(WKBMultiLineString *pWKBMultiLineString, Transformation* pTransformation)
	//{
	//	g_uchar* ptr = NULL;
	//	WKBLineString* pWKBLineString = NULL;
	//		
	//	pWKBMultiLineString = (WKBLineString*)(&(pWKBMultiLineString->lineStrings[0]));
	//	ptr = pWKBMultiLineString;

	//	int numLineStrings = pWKBMultiLineString->numLineStrings;		
	//	for(int i=0; i<numLineStrings; i++)
	//	{

	//	}
	//}
	//////////////////////////////////////////////////////////////////////////
	// Draw Line End
	//////////////////////////////////////////////////////////////////////////
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
			cairo_new_path(cairo);

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

		cairo_new_path(cairo);
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

	void build_path(cairo_t* cairo, WKBMultiPolygon* pWKBMultiPolygon, Transformation* pTransformation)
	{
		int i=0,j=0,k=0,end=0;
		int sx=0, sy=0;
		int numPolygons = 0;
		int numRings = 0;
		int numPoints = 0;
		auge::Point *pt = NULL;
		auge::LinearRing *pLinearRing = NULL;
		auge::WKBPolygon *pWKBPolygon = NULL;

		numPolygons = pWKBMultiPolygon->numPolygons;
		pWKBPolygon = (auge::WKBPolygon*)(&(pWKBMultiPolygon->polygons[0]));

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

	//////////////////////////////////////////////////////////////////////////
	// DrawRaster Begin
	//////////////////////////////////////////////////////////////////////////
	bool RendererCairo::DrawRaster(Raster* pRaster, Transformation* pTransformation)
	{
		bool ret = true;
		const char* format = pRaster->GetFormat();

		GEnvelope& raster_extent = pRaster->GetExtent();
		GEnvelope& viewer_extent = pTransformation->GetViewer();

		if(!raster_extent.Intersects(viewer_extent))
		{
			return false;
		}

		GEnvelope visible_extent = raster_extent.Intersect(viewer_extent);
		if(!visible_extent.IsValid())
		{
			return false;
		}

		g_int r_xmin=0, r_ymin=0, r_xmax=0, r_ymax=0;
		pRaster->GetRasterPosition(visible_extent.m_xmin, visible_extent.m_ymax, r_xmin, r_ymin);
		pRaster->GetRasterPosition(visible_extent.m_xmax, visible_extent.m_ymin, r_xmax, r_ymax);

		g_uint r_w = r_xmax - r_xmin;
		g_uint r_h= r_ymax - r_ymin;

		g_uint r_stride = r_w * g_cairo_bands;
		g_uint r_data_size = r_stride * r_h;
		g_byte* r_data = (g_byte*)calloc(r_data_size, sizeof(g_byte));

		augePixelType pixelType = pRaster->GetPixelType();
		switch(pixelType)
		{
		case augePixelByte:
			ReadRasterSubArea_Byte(pRaster, r_data, r_xmin, r_ymin, r_w, r_h);
			break;
		case augePixelInt16:
		case augePixelUInt16:
			ReadRasterSubArea_Short(pRaster, r_data, r_xmin, r_ymin, r_w, r_h);
			break;
		case augePixelFloat32:
			ReadRasterSubArea_Float(pRaster, r_data, r_xmin, r_ymin, r_w, r_h);
			break;
		}
		
		cairo_surface_t *raster_surface = NULL;	
		raster_surface = cairo_image_surface_create_for_data(r_data,
			CAIRO_FORMAT_ARGB32,//(cairo_format_t)pRaster->GetPixelType(),
			r_w,
			r_h,
			r_stride);

		int s_x0=0, s_y0=0;
		int s_x1=0, s_y1=0;
		GEnvelope& extent = pRaster->GetExtent();
		pTransformation->ToScreenPoint(visible_extent.m_xmin, visible_extent.m_ymin, s_x0, s_y0);
		pTransformation->ToScreenPoint(visible_extent.m_xmax, visible_extent.m_ymax, s_x1, s_y1);
		int s_w = abs(s_x1 - s_x0);
		int s_h = abs(s_y1 - s_y0);
		int o_x = g_min(s_x0, s_x1);
		int o_y = g_min(s_y0, s_y1);
		double cairo_scale_x = (double)s_w/(double)r_w;
		double cairo_scale_y = (double)s_h/(double)r_h;
		//double cairo_scale_x = 1.0f;
		//double cairo_scale_y = 1.0f;

		cairo_save(m_cairo);
		cairo_translate(m_cairo, o_x, o_y);
		cairo_scale  (m_cairo, cairo_scale_x, cairo_scale_y);
		cairo_set_source_surface(m_cairo, raster_surface, 0,0);

		cairo_paint(m_cairo);
		cairo_surface_flush(m_cairo_surface);
		cairo_surface_destroy(raster_surface);
		cairo_restore(m_cairo);

		free(r_data);

		return true;
	}

	//bool RendererCairo::DrawRaster(Raster* pRaster, Transformation* pTransformation)
	//{
	//	GEnvelope& raster_extent = pRaster->GetExtent();
	//	GEnvelope& viewer_extent = pTransformation->GetViewer();

	//	if(!raster_extent.Intersects(viewer_extent))
	//	{
	//		return false;
	//	}

	//	GEnvelope visible_extent = raster_extent.Intersect(viewer_extent);
	//	if(!visible_extent.IsValid())
	//	{
	//		return false;
	//	}

	//	g_int r_xmin=0, r_ymin=0, r_xmax=0, r_ymax=0;
	//	pRaster->GetRasterPosition(visible_extent.m_xmin, visible_extent.m_ymax, r_xmin, r_ymin);
	//	pRaster->GetRasterPosition(visible_extent.m_xmax, visible_extent.m_ymin, r_xmax, r_ymax);

	//	g_uint r_w = r_xmax - r_xmin;
	//	g_uint r_h= r_ymax - r_ymin;

	//	g_uint r_stride = r_w * g_cairo_bands;
	//	g_uint r_data_size = r_stride * r_h;
	//	g_byte* r_data = (g_byte*)calloc(r_data_size, sizeof(g_byte));

	//	ReadRasterSubArea(pRaster, r_data, r_xmin, r_ymin, r_w, r_h);

	//	cairo_surface_t *raster_surface = NULL;	
	//	raster_surface = cairo_image_surface_create_for_data(r_data,
	//								CAIRO_FORMAT_ARGB32,//(cairo_format_t)pRaster->GetPixelType(),
	//								r_w,
	//								r_h,
	//								r_stride);

	//	int s_x0=0, s_y0=0;
	//	int s_x1=0, s_y1=0;
	//	GEnvelope& extent = pRaster->GetExtent();
	//	pTransformation->ToScreenPoint(visible_extent.m_xmin, visible_extent.m_ymin, s_x0, s_y0);
	//	pTransformation->ToScreenPoint(visible_extent.m_xmax, visible_extent.m_ymax, s_x1, s_y1);
	//	int s_w = abs(s_x1 - s_x0);
	//	int s_h = abs(s_y1 - s_y0);
	//	int o_x = g_min(s_x0, s_x1);
	//	int o_y = g_min(s_y0, s_y1);
	//	double cairo_scale_x = (double)s_w/(double)r_w;
	//	double cairo_scale_y = (double)s_h/(double)r_h;
	//	//double cairo_scale_x = 1.0f;
	//	//double cairo_scale_y = 1.0f;
	//	
	//	cairo_save(m_cairo);
	//	cairo_translate(m_cairo, o_x, o_y);
	//	cairo_scale  (m_cairo, cairo_scale_x, cairo_scale_y);
	//	cairo_set_source_surface(m_cairo, raster_surface, 0,0);

	//	cairo_paint(m_cairo);
	//	cairo_surface_flush(m_cairo_surface);
	//	cairo_surface_destroy(raster_surface);
	//	cairo_restore(m_cairo);

	//	return true;
	//}

	//bool RendererCairo::DrawRaster(Raster* pRaster, Transformation* pTransformation)
	//{
	//	const char* path = pRaster->GetPath();
	//	cairo_surface_t *image = cairo_image_surface_create_from_png(path);
	//	if(image!=NULL)
	//	{
	//		int s_x0=0, s_y0=0;
	//		int s_x1=0, s_y1=0;
	//		GEnvelope& extent = pRaster->GetExtent();
	//		pTransformation->ToScreenPoint(extent.m_xmin, extent.m_ymin, s_x0, s_y0);
	//		pTransformation->ToScreenPoint(extent.m_xmax, extent.m_ymax, s_x1, s_y1);
	//		int s_w = abs(s_x1 - s_x0);
	//		int s_h = abs(s_y1 - s_y0);
	//		int o_x = g_min(s_x0, s_x1);
	//		int o_y = g_min(s_y0, s_y1);

	//		int i_w = cairo_image_surface_get_width(image);
	//		int i_h = cairo_image_surface_get_height(image);

	//		cairo_save(m_cairo);
	//		cairo_translate(m_cairo, o_x, o_y);
	//		cairo_scale  (m_cairo, (double)s_w/(double)i_w, (double)s_h/(double)i_h);
	//		cairo_set_source_surface(m_cairo, image, 0,0);

	//		cairo_paint(m_cairo);
	//		cairo_surface_flush(m_cairo_surface);
	//		cairo_surface_destroy(image);
	//		cairo_restore(m_cairo);
	//	}

	//	return true;
	//}

	bool RendererCairo::ReadRasterSubArea_Byte(Raster* pRaster, g_byte* pdata, int x, int y, int width, int height)
	{
		bool ret = false;
		g_uint nBands = pRaster->GetBandCount();

		switch(nBands)
		{
		case 1:
			ret = ReadRasterSubAreaBand_Byte_1(pRaster, pdata, x, y, width, height);
			break;
		case 2:
			break;
		case 3:
			ret = ReadRasterSubAreaBand_Byte_3(pRaster, pdata, x, y, width, height);
			break;
		case 4:
			ret = ReadRasterSubAreaBand_Byte_4(pRaster, pdata, x, y, width, height);
			break;
		}

		return ret;
	}

	bool RendererCairo::ReadRasterSubAreaBand_Byte_1(Raster* pRaster, g_byte* pdata, int x, int y, int width, int height)
	{
		g_uint stride = m_width * g_cairo_bands * sizeof(char);

		auge::RasterBand* pBand = NULL;
		// red
		pBand = pRaster->GetBand(0);
		g_byte* pb = (g_byte*)pBand->GetData(x, y);
		CopyMatrix_Byte(pb, pRaster->GetWidth(), pRaster->GetHeight(), pdata+2, width, height, g_cairo_bands);
		CopyMatrix_Byte(pb, pRaster->GetWidth(), pRaster->GetHeight(), pdata+1, width, height, g_cairo_bands);
		CopyMatrix_Byte(pb, pRaster->GetWidth(), pRaster->GetHeight(), pdata+0, width, height, g_cairo_bands);
		// alpha
		SetMatrix(pdata+3, width, height, g_cairo_bands, 255);

		return true;
	}

	bool RendererCairo::ReadRasterSubAreaBand_Byte_3(Raster* pRaster, g_byte* pdata, int x, int y, int width, int height)
	{
		g_uint stride = m_width * g_cairo_bands * sizeof(char);

		auge::RasterBand* pBand = NULL;
		// red
		pBand = pRaster->GetBand(0);
		g_byte* pb = (g_byte*)pBand->GetData(x, y);
		CopyMatrix_Byte(pb, pRaster->GetWidth(), pRaster->GetHeight(), pdata+2, width, height, g_cairo_bands);
		// green
		pBand = pRaster->GetBand(1);
		pb = (g_byte*)pBand->GetData(x, y);
		CopyMatrix_Byte(pb, pRaster->GetWidth(), pRaster->GetHeight(), pdata+1, width, height, g_cairo_bands);
		// blue
		pBand = pRaster->GetBand(2);
		pb = (g_byte*)pBand->GetData(x, y);
		CopyMatrix_Byte(pb, pRaster->GetWidth(), pRaster->GetHeight(), pdata+0, width, height, g_cairo_bands);
		// alpha
		SetMatrix(pdata+3, width, height, g_cairo_bands, 255);

		return true;
	}

	bool RendererCairo::ReadRasterSubAreaBand_Byte_4(Raster* pRaster, g_byte* pdata, int x, int y, int width, int height)
	{
		g_uint stride = m_width * g_cairo_bands * sizeof(char);

		auge::RasterBand* pBand = NULL;
		// red
		pBand = pRaster->GetBand(0);
		g_byte* pb = (g_byte*)pBand->GetData(x, y);
		CopyMatrix_Byte(pb, pRaster->GetWidth(), pRaster->GetHeight(), pdata+2, width, height, g_cairo_bands);
		// green
		pBand = pRaster->GetBand(1);
		pb = (g_byte*)pBand->GetData(x, y);
		CopyMatrix_Byte(pb,pRaster->GetWidth(), pRaster->GetHeight(),  pdata+1, width, height, g_cairo_bands);
		// blue
		pBand = pRaster->GetBand(2);
		pb = (g_byte*)pBand->GetData(x, y);
		CopyMatrix_Byte(pb, pRaster->GetWidth(), pRaster->GetHeight(), pdata+0, width, height, g_cairo_bands);
		// alpha
		pBand = pRaster->GetBand(3);
		pb = (g_byte*)pBand->GetData(x, y);
		CopyMatrix_Byte(pb, pRaster->GetWidth(), pRaster->GetHeight(), pdata+3, width, height, g_cairo_bands);
		// alpha
		//SetMatrix(pdata+3, width, height, g_cairo_bands, 255);

		return true;
	}

	bool RendererCairo::ReadRasterSubArea_Short(Raster* pRaster, unsigned char* pdata, int x, int y, int width, int height)
	{
		bool ret = false;
		g_uint nBands = pRaster->GetBandCount();

		switch(nBands)
		{
		case 1:
			ret = ReadRasterSubAreaBand_Short_1(pRaster, pdata, x, y, width, height);
			break;
		case 2:
			break;
		case 3:
			ret = ReadRasterSubAreaBand_Byte_3(pRaster, pdata, x, y, width, height);
			break;
		}

		return ret;
	}

	bool RendererCairo::ReadRasterSubAreaBand_Short_1(Raster* pRaster, g_byte* pdata, int x, int y, int width, int height)
	{
		g_uint stride = m_width * g_cairo_bands * sizeof(char);

		auge::RasterBand* pBand = NULL;
		// red
		pBand = pRaster->GetBand(0);
		g_int16* pb = (g_int16*)pBand->GetData(x, y);
		CopyMatrix_Short(pb, pRaster->GetWidth(), pRaster->GetHeight(), pdata+2, width, height, g_cairo_bands);
		CopyMatrix_Short(pb, pRaster->GetWidth(), pRaster->GetHeight(), pdata+1, width, height, g_cairo_bands);
		CopyMatrix_Short(pb, pRaster->GetWidth(), pRaster->GetHeight(), pdata+0, width, height, g_cairo_bands);
		// alpha
		SetMatrix(pdata+3, width, height, g_cairo_bands, 255);

		return true;
	}

	bool RendererCairo::ReadRasterSubAreaBand_Short_3(Raster* pRaster, g_byte* pdata, int x, int y, int width, int height)
	{
		g_uint stride = m_width * g_cairo_bands * sizeof(char);

		auge::RasterBand* pBand = NULL;
		// red
		pBand = pRaster->GetBand(0);
		g_int16* pb = (g_int16*)pBand->GetData(x, y);
		CopyMatrix_Short(pb, pRaster->GetWidth(), pRaster->GetHeight(), pdata+2, width, height, g_cairo_bands);
		pBand = pRaster->GetBand(1);
		pb = (g_int16*)pBand->GetData(x, y);
		CopyMatrix_Short(pb, pRaster->GetWidth(), pRaster->GetHeight(), pdata+1, width, height, g_cairo_bands);
		pBand = pRaster->GetBand(2);
		pb = (g_int16*)pBand->GetData(x, y);
		CopyMatrix_Short(pb, pRaster->GetWidth(), pRaster->GetHeight(), pdata+0, width, height, g_cairo_bands);
		// alpha
		SetMatrix(pdata+3, width, height, g_cairo_bands, 255);

		return true;
	}

	bool RendererCairo::ReadRasterSubArea_Float(Raster* pRaster, unsigned char* pdata, int x, int y, int width, int height)
	{
		bool ret = false;
		g_uint nBands = pRaster->GetBandCount();

		switch(nBands)
		{
		case 1:
			ret = ReadRasterSubAreaBand_Float_1(pRaster, pdata, x, y, width, height);
			break;
		case 2:
			break;
		case 3:
			ret = ReadRasterSubAreaBand_Byte_3(pRaster, pdata, x, y, width, height);
			break;
		}

		return ret;
	}

	bool RendererCairo::ReadRasterSubAreaBand_Float_1(Raster* pRaster, g_byte* pdata, int x, int y, int width, int height)
	{
		g_uint stride = m_width * g_cairo_bands * sizeof(char);

		auge::RasterBand* pBand = NULL;
		// red
		pBand = pRaster->GetBand(0);
		g_int16* pb = (g_int16*)pBand->GetData(x, y);
		CopyMatrix_Float(pb, pRaster->GetWidth(), pRaster->GetHeight(), pdata+2, width, height, g_cairo_bands);
		CopyMatrix_Float(pb, pRaster->GetWidth(), pRaster->GetHeight(), pdata+1, width, height, g_cairo_bands);
		CopyMatrix_Float(pb, pRaster->GetWidth(), pRaster->GetHeight(), pdata+0, width, height, g_cairo_bands);
		// alpha
		SetMatrix(pdata+3, width, height, g_cairo_bands, 255);

		return true;
	}

	bool RendererCairo::ReadRasterSubAreaBand_Float_3(Raster* pRaster, g_byte* pdata, int x, int y, int width, int height)
	{
		g_uint stride = m_width * g_cairo_bands * sizeof(char);

		auge::RasterBand* pBand = NULL;
		// red
		pBand = pRaster->GetBand(0);
		g_int16* pb = (g_int16*)pBand->GetData(x, y);
		CopyMatrix_Float(pb, pRaster->GetWidth(), pRaster->GetHeight(), pdata+2, width, height, g_cairo_bands);
		pBand = pRaster->GetBand(1);
		pb = (g_int16*)pBand->GetData(x, y);
		CopyMatrix_Float(pb, pRaster->GetWidth(), pRaster->GetHeight(), pdata+1, width, height, g_cairo_bands);
		pBand = pRaster->GetBand(2);
		pb = (g_int16*)pBand->GetData(x, y);
		CopyMatrix_Float(pb, pRaster->GetWidth(), pRaster->GetHeight(), pdata+0, width, height, g_cairo_bands);
		// alpha
		SetMatrix(pdata+3, width, height, g_cairo_bands, 255);

		return true;
	}

	void RendererCairo::CopyMatrix_Float(g_int16* src, int src_width, int src_height, g_byte* obj, int obj_width, int obj_height, int obj_step)
	{
		g_int16* sp = src;
		g_byte* op = obj;
		int i,j;
		for(i=0; i<obj_height; i++)
		{
			g_int16* ptr = sp;
			for(j=0; j<obj_width; j++,op+=obj_step)
			{
				//*op = *(ptr++);
				*op = auge_pixel_value_normalize_float(*(ptr++));
			}
			sp += src_width;
		}
	}

	void RendererCairo::CopyMatrix_Byte(g_byte* src, int src_width, int src_height, g_byte* obj, int obj_width, int obj_height, int obj_step)
	{
		g_byte* sp = src;
		g_byte* op = obj;
		int i,j;
		for(i=0; i<obj_height; i++)
		{
			g_byte* ptr = sp;
			for(j=0; j<obj_width; j++,op+=obj_step)
			{
				*op = *(ptr++);
			}
			sp += src_width;
		}
	}

	void RendererCairo::CopyMatrix_Short(g_int16* src, int src_width, int src_height, g_byte* obj, int obj_width, int obj_height, int obj_step)
	{
		g_int16* sp = src;
		g_byte* op = obj;
		int i,j;
		for(i=0; i<obj_height; i++)
		{
			g_int16* ptr = sp;
			for(j=0; j<obj_width; j++,op+=obj_step)
			{
				//*op = *(ptr++);
				*op = auge_pixel_value_normalize_short(*(ptr++));
			}
			sp += src_width;
		}
	}

	void RendererCairo::SetMatrix(g_byte* obj, int width, int height, int stride, g_byte value)
	{
		g_byte* op = obj;
		for(int i=0; i<height; i++)
		{
			for(int j=0; j<width; j++,op+=stride)
			{
				*op = value;
			}
		}
	}
}
