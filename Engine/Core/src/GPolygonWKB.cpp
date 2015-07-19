#include "GPolygonWKB.h"
#include "AugeCore.h"
#include "WKTWriter.h"

namespace auge
{
	GPolygonWKB::GPolygonWKB():
	m_pWKBPolygon(NULL),
		m_attach(false)
	{

	}

	GPolygonWKB::~GPolygonWKB()
	{
		if(!m_attach)
		{
			if(m_pWKBPolygon!=NULL)
			{
				auge_free((g_uchar*)m_pWKBPolygon);
				m_pWKBPolygon = NULL;
			}
		}
	}

	augeGeometryType GPolygonWKB::GeometryType()
	{
		return augeGTPolygon;
	}

	const GEnvelope* GPolygonWKB::Envelope() const
	{
		return &m_extent;
	}

	const char*	GPolygonWKB::AsText(bool multi/*=false*/)
	{
		if(m_wkt.empty())
		{
			WKTWriter writer;
			writer.Write(m_wkt, (g_uchar*)m_pWKBPolygon, multi);
		}
		return m_wkt.c_str();
	}

	g_uchar* GPolygonWKB::AsBinary() const
	{
		return (g_uchar*)m_pWKBPolygon;
	}

	void GPolygonWKB::Release()
	{
		if(!ReleaseRef())
		{
			delete this;
		}
	}

	bool GPolygonWKB::Create(g_uchar *wkb, bool attach)
	{
		m_pWKBPolygon = (WKBPolygon*)wkb;
		m_attach = attach;
		return true;
	}

	Geometry* GPolygonWKB::Clone() const
	{
		return NULL;
	}

	void GPolygonWKB::Centroid(double &x, double &y)
	{
		x=0.0f,y=0.0f;
		int point_total = 0;
		Point* pt = NULL;
		int numRings = m_pWKBPolygon->numRings;
		LinearRing* pLinearRing = &(m_pWKBPolygon->rings[0]);
		for(int i=0; i<numRings; i++)
		{	
			int numPoints = pLinearRing->numPoints;
			point_total += numPoints;
			pt = &(pLinearRing->points[0]);
			for(int j=0; j<numPoints; j++, pt++)
			{
				x += pt->x;
				y += pt->y;
			}
			pLinearRing = (LinearRing*)pt;
		}
		x /= point_total;
		y /= point_total;
	}

	double GPolygonWKB::Area() const
	{
		double area = 0.0;
		int i=0, j=0;
		int numPoints = 0;
		int numRings  = m_pWKBPolygon->numRings;
		auge::LinearRing* pLinearRing = NULL;
		auge::Point *pt0 = NULL;
		auge::Point *pt1 = NULL;

		pLinearRing = (auge::LinearRing*)(&(m_pWKBPolygon->rings[0]));

		for(i=0; i<numRings; i++)
		{
			numPoints = pLinearRing->numPoints;
			pt0 = (auge::Point*)(&(pLinearRing->points[0]));
			pt1 = pt0 + 1;

			for(j=1; j<numPoints; j++, pt0++, pt1++)
			{
				double a = (pt1->y + pt0->y) * (pt1->x - pt0->y) / 2.0;
				area += a;
			}
			
			pLinearRing = (auge::LinearRing*)(pt1);
		}

		return area;
	}

	double GPolygonWKB::Perimeter() const
	{
		double length = 0.0;
		int i=0, j=0;
		int numPoints = 0;
		int numRings  = m_pWKBPolygon->numRings;
		auge::LinearRing* pLinearRing = NULL;
		auge::Point *pt0 = NULL;
		auge::Point *pt1 = NULL;

		pLinearRing = (auge::LinearRing*)(&(m_pWKBPolygon->rings[0]));

		for(i=0; i<numRings; i++)
		{
			numPoints = pLinearRing->numPoints;
			pt0 = (auge::Point*)(&(pLinearRing->points[0]));
			pt1 = pt0 + 1;

			for(j=1; j<numPoints; j++, pt0++, pt1++)
			{
				length += sqrt(pow((pt1->x - pt0->x), 2) + pow((pt1->y - pt0->y), 2));
			}

			pLinearRing = (auge::LinearRing*)(pt1);
		}

		return length;
	}
}