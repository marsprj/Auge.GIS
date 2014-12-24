#include "GPointWKB.h"
#include "AugeCore.h"

namespace auge
{
	GPointWKB::GPointWKB():
	m_pWKBPoint(NULL),
	m_attach(false)
	{

	}

	GPointWKB::~GPointWKB()
	{
		if(m_attach)
		{
			if(m_pWKBPoint!=NULL)
			{
				auge_free(m_pWKBPoint);
				m_pWKBPoint = NULL;
			}
		}
		
	}

	augeGeometryType GPointWKB::GeometryType()
	{
		return augeGTPoint;
	}

	const GEnvelope* GPointWKB::Envelope() const
	{
		return &m_extent;
	}

	const char*	GPointWKB::AsText()
	{
		return m_wkt.c_str();
	}

	g_uchar* GPointWKB::AsBinary() const
	{
		return (g_uchar*)m_pWKBPoint;
	}

	void GPointWKB::Release()
	{
		if(!ReleaseRef())
		{
			delete this;
		}
	}

	bool GPointWKB::Create(g_uchar *wkb, bool attach)
	{
		m_pWKBPoint = (WKBPoint*)wkb;
		m_attach = attach;
		return true;
	}
}