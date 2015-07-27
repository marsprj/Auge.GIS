#include "GMultiPolygonWKB.h"
#include "AugeCore.h"
#include "WKTWriter.h"

#include <math.h>

namespace auge
{
	GMultiPolygonWKB::GMultiPolygonWKB():
	m_pWKBMultiPolygon(NULL),
		m_attach(false)
	{

	}

	GMultiPolygonWKB::~GMultiPolygonWKB()
	{
		if(!m_attach)
		{
			if(m_pWKBMultiPolygon!=NULL)
			{
				auge_free(m_pWKBMultiPolygon);
				m_pWKBMultiPolygon = NULL;
			}
		}
	}

	augeGeometryType GMultiPolygonWKB::GeometryType()
	{
		return augeGTMultiPolygon;
	}

	const GEnvelope* GMultiPolygonWKB::Envelope() const
	{
		return &m_extent;
	}

	const char*	GMultiPolygonWKB::AsText(bool multi/*=false*/)
	{
		if(m_wkt.empty())
		{
			WKTWriter writer;
			writer.Write(m_wkt, (g_uchar*)m_pWKBMultiPolygon);
		}
		return m_wkt.c_str();
	}

	g_uchar* GMultiPolygonWKB::AsBinary() const
	{
		return (g_uchar*)m_pWKBMultiPolygon;
	}

	void GMultiPolygonWKB::Release()
	{
		if(!ReleaseRef())
		{
			delete this;
		}
	}

	bool GMultiPolygonWKB::Create(g_uchar *wkb, bool attach)
	{
		m_pWKBMultiPolygon = (WKBMultiPolygon*)wkb;
		m_attach = attach;
		return true;
	}

	Geometry* GMultiPolygonWKB::Clone() const
	{
		return NULL;
	}

	void GMultiPolygonWKB::Centroid(double &x, double &y)
	{
		x=0.0f,y=0.0f;
		int point_total = 0;
		Point* pt = NULL;
		int numPolygons = m_pWKBMultiPolygon->numPolygons;
		WKBPolygon* pWKBPolygon = &(m_pWKBMultiPolygon->polygons[0]);
		for(int i=0; i<numPolygons; i++)
		{
			int numRings = pWKBPolygon->numRings;
			LinearRing* pLinearRing = &(pWKBPolygon->rings[0]);
			for(int j=0; j<numRings; j++)
			{	
				int numPoints = pLinearRing->numPoints;
				point_total += numPoints;
				pt = &(pLinearRing->points[0]);
				for(int k=0; k<numPoints; k++, pt++)
				{
					x += pt->x;
					y += pt->y;
				}
				pLinearRing = (LinearRing*)pt;
			}
			pWKBPolygon = (WKBPolygon*)pt;
		}
		
		x /= point_total;
		y /= point_total;
	}

	double GMultiPolygonWKB::Area() const
	{
		double area=0.0;
		auge::Point* pt0 = NULL;
		auge::Point* pt1 = NULL; 

		int numPolygons = m_pWKBMultiPolygon->numPolygons;
		WKBPolygon* pWKBPolygon = &(m_pWKBMultiPolygon->polygons[0]);
		for(int i=0; i<numPolygons; i++)
		{
			int numRings = pWKBPolygon->numRings;
			LinearRing* pLinearRing = &(pWKBPolygon->rings[0]);
			for(int j=0; j<numRings; j++)
			{	
				int numPoints = pLinearRing->numPoints;
				pt0 = &(pLinearRing->points[0]);
				pt1 = pt0 + 1;
				for(int k=1; k<numPoints; k++, pt0++, pt1++)
				{
					double a = (pt1->y + pt0->y) * (pt1->x - pt0->x) / 2.0;
					area += a;
				}
				pLinearRing = (LinearRing*)pt1;
			}
			pWKBPolygon = (WKBPolygon*)pt1;
		}
		return area;
	}

	double GMultiPolygonWKB::Perimeter() const
	{
		double length=0.0;
		auge::Point* pt0 = NULL;
		auge::Point* pt1 = NULL; 

		int numPolygons = m_pWKBMultiPolygon->numPolygons;
		WKBPolygon* pWKBPolygon = &(m_pWKBMultiPolygon->polygons[0]);
		for(int i=0; i<numPolygons; i++)
		{
			int numRings = pWKBPolygon->numRings;
			LinearRing* pLinearRing = &(pWKBPolygon->rings[0]);
			for(int j=0; j<numRings; j++)
			{	
				int numPoints = pLinearRing->numPoints;
				pt0 = &(pLinearRing->points[0]);
				pt1 = pt0 + 1;
				for(int k=1; k<numPoints; k++, pt0++, pt1++)
				{
					length += sqrt(pow((pt1->x - pt0->x), 2) + pow((pt1->y - pt0->y), 2));
				}
				pLinearRing = (LinearRing*)pt1;
			}
			pWKBPolygon = (WKBPolygon*)pt1;
		}
		return length;
	}
}
