#include "GLineStringWKB.h"
#include "AugeCore.h"

namespace auge
{
	GLineStringWKB::GLineStringWKB():
m_pWKBLineString(NULL),
	m_attach(false)
{

}

GLineStringWKB::~GLineStringWKB()
{
	if(m_attach)
	{
		if(m_pWKBLineString!=NULL)
		{
			auge_free(m_pWKBLineString);
			m_pWKBLineString = NULL;
		}
	}

}

augeGeometryType GLineStringWKB::GeometryType()
{
	return augeGTPoint;
}

const GEnvelope* GLineStringWKB::Envelope() const
{
	return &m_extent;
}

const char*	GLineStringWKB::AsText()
{
	return m_wkt.c_str();
}

g_uchar* GLineStringWKB::AsBinary() const
{
	return (g_uchar*)m_pWKBLineString;
}

void GLineStringWKB::Release()
{
	if(!ReleaseRef())
	{
		delete this;
	}
}

bool GLineStringWKB::Create(g_uchar *wkb, bool attach)
{
	m_pWKBLineString = (WKBLineString*)wkb;
	m_attach = attach;
	return true;
}
}