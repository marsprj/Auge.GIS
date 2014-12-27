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
	if(m_attach)
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
}