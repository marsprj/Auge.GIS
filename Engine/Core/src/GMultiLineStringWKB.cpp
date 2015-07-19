#include "GMultiLineStringWKB.h"
#include "AugeCore.h"
#include "WKTWriter.h"

#include <math.h>

namespace auge
{
	GMultiLineStringWKB::GMultiLineStringWKB():
	m_pWKBMultiLineString(NULL),
		m_attach(false)
	{

	}

	GMultiLineStringWKB::~GMultiLineStringWKB()
	{
		if(!m_attach)
		{
			if(m_pWKBMultiLineString!=NULL)
			{
				auge_free(m_pWKBMultiLineString);
				m_pWKBMultiLineString = NULL;
			}
		}
	}

	augeGeometryType GMultiLineStringWKB::GeometryType()
	{
		return augeGTMultiLineString;
	}

	const GEnvelope* GMultiLineStringWKB::Envelope() const
	{
		return &m_extent;
	}

	const char*	GMultiLineStringWKB::AsText(bool multi/*=false*/)
	{
		if(m_wkt.empty())
		{
			WKTWriter writer;
			writer.Write(m_wkt, (g_uchar*)m_pWKBMultiLineString,multi);
		}
		return m_wkt.c_str();
	}

	g_uchar* GMultiLineStringWKB::AsBinary() const
	{
		return (g_uchar*)m_pWKBMultiLineString;
	}

	void GMultiLineStringWKB::Release()
	{
		if(!ReleaseRef())
		{
			delete this;
		}
	}

	bool GMultiLineStringWKB::Create(g_uchar *wkb, bool attach)
	{
		m_pWKBMultiLineString = (WKBMultiLineString*)wkb;
		m_attach = attach;
		return true;
	}

	Geometry* GMultiLineStringWKB::Clone() const
	{
		return NULL;
	}

	void GMultiLineStringWKB::Centroid(double &x, double &y)
	{
		x=0.0f,y=0.0f;
		int point_total = 0;
		Point* pt = NULL;
		int numLineStrings = m_pWKBMultiLineString->numLineStrings;
		WKBLineString* pWKBLineString = &(m_pWKBMultiLineString->lineStrings[0]);
		for(int i=0; i<numLineStrings; i++)
		{	
			int numPoints = pWKBLineString->numPoints;
			point_total += numPoints;
			pt = &(pWKBLineString->points[0]);
			for(int j=0; j<numPoints; j++, pt++)
			{
				x += pt->x;
				y += pt->y;
			}
			pWKBLineString = (WKBLineString*)pt;
		}
		x /= point_total;
		y /= point_total;
	}

	double GMultiLineStringWKB::Length() const
	{
		double length = 0.0;
		Point* pt0 = NULL;
		Point* pt1 = NULL;
		int numLineStrings = m_pWKBMultiLineString->numLineStrings;
		WKBLineString* pWKBLineString = &(m_pWKBMultiLineString->lineStrings[0]);
		for(int i=0; i<numLineStrings; i++)
		{	
			int numPoints = pWKBLineString->numPoints;
			pt0 = &(pWKBLineString->points[0]);
			pt1 = pt0 + 1;
			for(int j=1; j<numPoints; j++, pt0++, pt1++)
			{
				length += sqrt(pow((pt1->x - pt0->x), 2) + pow((pt1->y - pt0->y), 2));
			}
			pWKBLineString = (WKBLineString*)pt1;
		}

		return 0.0;
	}
}