#include "GMultiPointWKB.h"
#include "AugeCore.h"

namespace auge
{
	GMultiPointWKB::GMultiPointWKB():
m_pWKBMultiPoint(NULL),
	m_attach(false)
{

}

GMultiPointWKB::~GMultiPointWKB()
{
	if(m_attach)
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

const char*	GMultiPointWKB::AsText()
{
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
}