#include "GMultiPolygonWKB.h"
#include "AugeCore.h"
#include "WKTWriter.h"

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
		return augeGTPoint;
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
}