#include "GMultiPointWKB.h"
#include "AugeCore.h"
#include "WKTWriter.h"

namespace auge
{
	GMultiPointWKB::GMultiPointWKB():
	m_pWKBMultiPoint(NULL),
		m_attach(false)
	{

	}

	GMultiPointWKB::~GMultiPointWKB()
	{
		if(!m_attach)
		{
			if(m_pWKBMultiPoint!=NULL)
			{
				auge_free(m_pWKBMultiPoint);
				m_pWKBMultiPoint = NULL;
			}
		}

	}

	augeGeometryType GMultiPointWKB::GeometryType()
	{
		return augeGTPoint;
	}

	const GEnvelope* GMultiPointWKB::Envelope() const
	{
		return &m_extent;
	}

	const char*	GMultiPointWKB::AsText(bool multi/*=false*/)
	{
		if(m_wkt.empty())
		{
			WKTWriter writer;
			writer.Write(m_wkt, (g_uchar*)m_pWKBMultiPoint);
		}
		return m_wkt.c_str();
	}

	g_uchar* GMultiPointWKB::AsBinary() const
	{
		return (g_uchar*)m_pWKBMultiPoint;
	}

	void GMultiPointWKB::Release()
	{
		if(!ReleaseRef())
		{
			delete this;
		}
	}

	bool GMultiPointWKB::Create(g_uchar *wkb, bool attach)
	{
		m_pWKBMultiPoint = (WKBMultiPoint*)wkb;
		m_attach = attach;
		return true;
	}
	Geometry* GMultiPointWKB::Clone() const
	{
		return NULL;
	}

	void GMultiPointWKB::Centroid(double &x, double &y)
	{
		x=0.0f, y=0.0f;
		int numPoints = m_pWKBMultiPoint->numPoints;
		WKBPoint* pWKBPoint = (WKBPoint*)(&(m_pWKBMultiPoint->points[0]));
		for(int i=0; i<numPoints; i++, pWKBPoint++)
		{
			x += pWKBPoint->point.x;
			y += pWKBPoint->point.y;
		}
		x = x / numPoints;
		y = y / numPoints;
	}
}