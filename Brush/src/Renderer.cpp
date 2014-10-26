#include "Renderer.h"
#include "Map.h"
#include "Transformation.h"
#include "FeatureLayer.h"
#include "FeatureType.h"
#include "FeatureSet.h"
#include "Style.h"
#include "Rule.h"
#include "PointSymbolizer.h"
#include "LineSymbolizer.h"
#include "PolygonSymbolizer.h"
#include "TextSymbolizer.h"
#include "Filter.h"
#include "Color.h"

namespace auge
{
	#define NORMAILZE(v) ((v)/(255.0f))

	Renderer::Renderer():
	m_surface(NULL),
	m_cairo(NULL)
	{

	}

	Renderer::~Renderer()
	{
		Cleanup();
	}

	bool Renderer::Create(uint width, uint height)
	{
		m_surface = cairo_image_surface_create(CAIRO_FORMAT_ARGB32, width, height);
		m_cairo = cairo_create(m_surface);
		return true;
	}

	void Renderer::Cleanup()
	{
		if(m_cairo!=NULL)
		{
			cairo_destroy(m_cairo);
			cairo_surface_destroy(m_surface);
			m_cairo = NULL;
			m_surface = NULL;
		}
	}

	void Renderer::DrawMap(Map* pMap, Transformation* pTransformation)
	{	
		std::vector<Layer*>& layers = pMap->GetLayers();
		std::vector<Layer*>::iterator iter;

		Layer* l = NULL;
		int n = layers.size();
		for(int i=n-1; i>=0; i--)
		{
			l = layers[i];
			switch(l->GetType())
			{
				case augeLayerFeature:
					DrawLayer(static_cast<FeatureLayer*>(l), pTransformation);
					break;
				case augeLayerRaster:
					break;
			}
		}

	}

	void Renderer::DrawLayer(FeatureLayer* pLayer, Transformation* pTransformation)
	{
		if(pLayer->GetFeatureType()==NULL)
		{
			return;
		}

		Style* s = NULL;
		s = pLayer->GetStyle();
		if(s==NULL)
		{
			return;
		}

		Rule *r = NULL;
		std::vector<Rule*>::iterator iter;
		std::vector<Rule*>& rules = s->GetRules();

		for(iter=rules.begin(); iter!=rules.end(); iter++)
		{
			r = *iter;

			Save();
			//SetSymbolizer(r->GetSymbolizer());
			DrawLayer(pLayer, r, pTransformation);
			Restore();
		}
	}

	void Renderer::DrawLayer(FeatureLayer* pLayer, Rule *pRule, Transformation* pTransformation)
	{
		FeatureType	*ft = NULL;
		FeatureSet	*fs = NULL;
		Filter		*filter = pRule->GetFilter();
		Symbolizer	*symbol = pRule->GetSymbolizer();

		ft = pLayer->GetFeatureType();
		//symbol = pRule->GetSymbolizer();
		//filter = pRule->GetFilter();

		fs = ft->Query();
		switch(symbol->GetType())
		{
		case augeSymbolText:
			LabelFeatures(fs, static_cast<TextSymbolizer*>(symbol), pTransformation);
			break;
		default:
			DrawFeatures(fs, symbol, pTransformation);
		}
		
		fs->Release();
	}
	
	void Renderer::DrawFeatures(FeatureSet* pFeatureSet, Symbolizer* pSymbolizer, Transformation* pTransformation)
	{
		int gi = -1;		
		FeatureType* ft = NULL;

		ft = pFeatureSet->GetFeatureType();
		gi = ft->GetGeometryFieldIndex();
		if(gi<0)
		{
			return;
		}

		pFeatureSet->Reset();		
		while(!pFeatureSet->IsEOF())
		{
			Feature& f = pFeatureSet->Next();
			uchar* wkb = f.GetBytes(gi);
			DrawGeomtry(wkb, pSymbolizer, pTransformation);
		}
	}
	
	void Renderer::DrawGeomtry(uchar* wkb, Symbolizer* pSymbolizer, Transformation* pTransformation)
	{
		switch(WKBTYPE(wkb))
		{
		case wkbPoint:
			DrawPoint((WKBPoint*)wkb, (PointSymbolizer*)pSymbolizer, pTransformation);
			break;
		case wkbLineString:
			DrawLine((WKBLineString*)wkb, (LineSymbolizer*)pSymbolizer, pTransformation);
			break;
		case wkbPolygon:
			DrawPolygon((WKBPolygon*)wkb, (PolygonSymbolizer*)pSymbolizer, pTransformation);
			break;
		case wkbMultiPoint:
			DrawMultiPoint((WKBMultiPoint*)wkb, (PointSymbolizer*)pSymbolizer, pTransformation);
			break;
		case wkbMultiLineString:
			DrawMultiLine((WKBMultiLineString*)wkb, (LineSymbolizer*)pSymbolizer, pTransformation);
			break;
		case wkbMultiPolygon:
			DrawMultiPolygon((WKBMultiPolygon*)wkb, (PolygonSymbolizer*)pSymbolizer, pTransformation);
			break;
		}
	}

	void Renderer::DrawPoint(WKBPoint *pWKBPoint, PointSymbolizer *pSymbolizer, Transformation* pTransformation)
	{
		int sx=0, sy=0;
		pTransformation->ToScreenPoint(pWKBPoint->point.x, pWKBPoint->point.y, sx, sy);

		cairo_arc(m_cairo, sx, sy, pSymbolizer->radius, 0, 2*PI);
		
		if(pSymbolizer->fill)
		{
			Color& c = pSymbolizer->fill_color;
			cairo_set_source_rgba(m_cairo,NORMAILZE(c.GetRed()), NORMAILZE(c.GetGreen()), NORMAILZE(c.GetBlue()), NORMAILZE(c.GetAlpha()));
			pSymbolizer->outline ? cairo_fill_preserve(m_cairo) : cairo_fill(m_cairo);
		}
		if(pSymbolizer->outline)
		{
			PointSymbolizer* s = pSymbolizer;
			Color& c = s->outline_color;
			cairo_set_source_rgba(m_cairo,NORMAILZE(c.GetRed()), NORMAILZE(c.GetGreen()), NORMAILZE(c.GetBlue()), NORMAILZE(c.GetAlpha()));			
			cairo_set_line_cap(m_cairo, (cairo_line_cap_t)(s->outline_cap));
			cairo_set_line_join(m_cairo, (cairo_line_join_t)(s->outline_join));
			cairo_set_line_width(m_cairo, s->outline_width);
			cairo_stroke(m_cairo);
		}
	}

	void Renderer::DrawLine(WKBLineString *pWKBLineString,LineSymbolizer *pSymbolizer, Transformation* pTransformation)
	{
		cairo_set_source_rgba(m_cairo,1.0f,0,0,0.5f);

		int numPoints = pWKBLineString->numPoints;
		if(numPoints>0)
		{
			int i = 0;
			int sx=0, sy=0;
			auge::Point *pt = (auge::Point*)(&(pWKBLineString->points[0]));

			pTransformation->ToScreenPoint(pt->x, pt->y, sx, sy);
			cairo_move_to(m_cairo, sx, sy);

			for(i=1, pt++; i<numPoints; i++, pt++)
			{
				pTransformation->ToScreenPoint(pt->x, pt->y, sx, sy);
				cairo_line_to(m_cairo, sx, sy);
			}
		}
		cairo_stroke(m_cairo);
	}

	void Renderer::DrawPolygon(WKBPolygon *pWKBPolygon, PolygonSymbolizer *pSymbolizer, Transformation* pTransformation)
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
			cairo_move_to(m_cairo, sx, sy);
			for(j=1, pt++; j<numPoints; j++, pt++)
			{
				pTransformation->ToScreenPoint(pt->x, pt->y, sx, sy);
				cairo_line_to(m_cairo, sx, sy);

			}
			cairo_close_path(m_cairo);
			pLinearRing = (auge::LinearRing*)(pt);
		}

		if(pSymbolizer->fill)
		{
			Color& c = pSymbolizer->fill_color;
			cairo_set_source_rgba(m_cairo,NORMAILZE(c.GetRed()), NORMAILZE(c.GetGreen()), NORMAILZE(c.GetBlue()), NORMAILZE(c.GetAlpha()));
			pSymbolizer->outline ? cairo_fill_preserve(m_cairo) : cairo_fill(m_cairo);
		}
		if(pSymbolizer->outline)
		{
			PolygonSymbolizer* s = pSymbolizer;
			Color& c = s->outline_color;
			cairo_set_source_rgba(m_cairo,NORMAILZE(c.GetRed()), NORMAILZE(c.GetGreen()), NORMAILZE(c.GetBlue()), NORMAILZE(c.GetAlpha()));			
			cairo_set_line_cap(m_cairo, (cairo_line_cap_t)(s->outline_cap));
			cairo_set_line_join(m_cairo, (cairo_line_join_t)(s->outline_join));
			cairo_set_line_width(m_cairo, s->outline_width);
			cairo_stroke(m_cairo);
		}
	}

	void Renderer::DrawMultiPoint(WKBMultiPoint *pWKBMultiPoint, PointSymbolizer *pSymbolizer, Transformation* pTransformation)
	{
		uint numPoints = pWKBMultiPoint->numPoints;
		WKBPoint* pWKBPoint = NULL;
		pWKBPoint = (WKBPoint*)(&(pWKBMultiPoint->points[0]));
		for(uint i=0; i<numPoints; i++, pWKBPoint++)
		{
			DrawPoint(pWKBPoint, pSymbolizer, pTransformation);
		}
	}

	void Renderer::DrawMultiLine( WKBMultiLineString *pWKBMultiLineString, LineSymbolizer *pSymbolizer, Transformation* pTransformation)
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
			cairo_move_to(m_cairo, sx, sy);

			for(j=1,pt++; j<numPoints; j++, pt++)
			{
				pTransformation->ToScreenPoint(pt->x, pt->y, sx, sy);
				cairo_line_to(m_cairo, sx, sy);
			}
			pWKBLineString = (WKBLineString*)(pt);
		}
		cairo_stroke(m_cairo);
	}

	void Renderer::DrawMultiPolygon(WKBMultiPolygon	*pWKBMultiPolygon, PolygonSymbolizer *pSymbolizer, Transformation* pTransformation)
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
					cairo_move_to(m_cairo, sx, sy);

					for(k=1, pt++; k<numPoints; k++,pt++)
					{
						pTransformation->ToScreenPoint(pt->x, pt->y, sx, sy);
						cairo_line_to(m_cairo, sx, sy);
					}
					cairo_close_path(m_cairo);
					pLinearRing = (auge::LinearRing*)(pt);
				}
			}
			pWKBPolygon = (WKBPolygon*)(pLinearRing);
		}

		if(pSymbolizer->fill)
		{
			Color& c = pSymbolizer->fill_color;
			cairo_set_source_rgba(m_cairo,NORMAILZE(c.GetRed()), NORMAILZE(c.GetGreen()), NORMAILZE(c.GetBlue()), NORMAILZE(c.GetAlpha()));
			pSymbolizer->outline ? cairo_fill_preserve(m_cairo) : cairo_fill(m_cairo);
		}
		if(pSymbolizer->outline)
		{
			PolygonSymbolizer* s = pSymbolizer;
			Color& c = s->outline_color;
			cairo_set_source_rgba(m_cairo,NORMAILZE(c.GetRed()), NORMAILZE(c.GetGreen()), NORMAILZE(c.GetBlue()), NORMAILZE(c.GetAlpha()));			
			cairo_set_line_cap(m_cairo, (cairo_line_cap_t)(s->outline_cap));
			cairo_set_line_join(m_cairo, (cairo_line_join_t)(s->outline_join));
			cairo_set_line_width(m_cairo, s->outline_width);
			cairo_stroke(m_cairo);
		}
	}

	void Renderer::LabelFeatures(FeatureSet* pFeatureSet, TextSymbolizer* pSymbolizer, Transformation* pTransformation)
	{
		int gi = -1;
		int li = -1;
		char* text = NULL;
		FeatureType* ft = NULL;

		ft = pFeatureSet->GetFeatureType();
		gi = ft->GetGeometryFieldIndex();
		li = ft->GetFieldIndex(pSymbolizer->field.c_str());
		if((gi<0)||(li<0))
		{
			return;
		}
		
		pFeatureSet->Reset();		
		while(!pFeatureSet->IsEOF())
		{
			Feature& f = pFeatureSet->Next();
			uchar* wkb = f.GetBytes(gi);
			text = f.GetValueAsString(li);
			if(text!=NULL)
			{
				LabelGeomtry(wkb, text, pSymbolizer, pTransformation);
			}
		}
	}

	void Renderer::LabelGeomtry(uchar* wkb, char* text, TextSymbolizer* pSymbolizer, Transformation* pTransformation)
	{
		switch(WKBTYPE(wkb))
		{
		case wkbPoint:
			LabelPoint((WKBPoint*)wkb, text, pSymbolizer, pTransformation);
			break;
		case wkbLineString:
			LabelLine((WKBLineString*)wkb, text, pSymbolizer, pTransformation);
			break;
		case wkbPolygon:
			LabelPolygon((WKBPolygon*)wkb, text, pSymbolizer, pTransformation);
			break;
		case wkbMultiPoint:
			LabelMultiPoint((WKBMultiPoint*)wkb, text, pSymbolizer, pTransformation);
			break;
		case wkbMultiLineString:
			LabelMultiLine((WKBMultiLineString*)wkb, text, pSymbolizer, pTransformation);
			break;
		case wkbMultiPolygon:
			LabelMultiPolygon((WKBMultiPolygon*)wkb, text,  pSymbolizer, pTransformation);
			break;
		}
	}

	void Renderer::LabelPoint(	WKBPoint	  *pWKBPoint,		char* text, TextSymbolizer	*pSymbolizer, Transformation* pTransformation)
	{
		int sx=0, sy=0;
		pTransformation->ToScreenPoint(pWKBPoint->point.x, pWKBPoint->point.y, sx, sy);
		LabelText(sx, sy, text, pSymbolizer);
	}

	void Renderer::LabelLine(	WKBLineString *pWKBLineString,	char* text, TextSymbolizer	*pSymbolizer, Transformation* pTransformation)
	{

	}

	void Renderer::LabelPolygon(WKBPolygon	  *pWKBPolygon,		char* text, TextSymbolizer	*pSymbolizer, Transformation* pTransformation)
	{

	}

	void Renderer::LabelMultiPoint( WKBMultiPoint		*pWKBMultiPoint,	char* text, TextSymbolizer	*pSymbolizer, Transformation* pTransformation)
	{

	}

	void Renderer::LabelMultiLine(	 WKBMultiLineString *pWKBMultiLineString,	char* text, TextSymbolizer	*pSymbolizer, Transformation* pTransformation)
	{

	}

	void Renderer::LabelMultiPolygon(WKBMultiPolygon	*pWKBMultiPolygon,		char* text, TextSymbolizer	*pSymbolizer, Transformation* pTransformation)
	{

	}

	void Renderer::LabelText(double x, double y, char* text, TextSymbolizer	*pSymbolizer)
	{
		TextSymbolizer* s=pSymbolizer;
		
		cairo_move_to(m_cairo,x+s->offset_x,y+s->offset_y);
		cairo_text_path (m_cairo, text);
		cairo_select_font_face(m_cairo, s->font.c_str(), (cairo_font_slant_t)s->slant, (cairo_font_weight_t)s->weight);
		cairo_set_font_size(m_cairo, s->size);

		if(s->fill)
		{
			Color& c = s->color;			
			cairo_set_source_rgba(m_cairo,NORMAILZE(c.GetRed()), NORMAILZE(c.GetGreen()), NORMAILZE(c.GetBlue()), NORMAILZE(c.GetAlpha()));
			s->outline ? cairo_fill_preserve (m_cairo) : cairo_fill(m_cairo);
		}
		if(s->outline)
		{
			Color& c = s->outline_color;
			cairo_set_source_rgba(m_cairo,NORMAILZE(c.GetRed()), NORMAILZE(c.GetGreen()), NORMAILZE(c.GetBlue()), NORMAILZE(c.GetAlpha()));
			cairo_set_line_width (m_cairo, s->outline_width);
			cairo_stroke (m_cairo);
		}	
	}

	bool Renderer::Save(const char* path)
	{
		if(path==NULL)
		{
			return false;
		}

		cairo_surface_write_to_png(m_surface, path);
		return true;
	}

	void Renderer::Save()
	{
		cairo_save(m_cairo);
	}

	void Renderer::Restore()
	{
		cairo_restore(m_cairo);
	}
}