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
	if(m_attach)
	{
		if(m_pWKBPolygon!=NULL)
		{
			auge_free(m_pWKBPolygon);
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

const char*	GPolygonWKB::AsText()
{
	if(m_wkt.empty())
	{
		WKTWriter writer;
		writer.Write(m_wkt, (g_uchar*)m_pWKBPolygon);
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
}