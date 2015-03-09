#include "GMultiLineStringWKB.h"
#include "AugeCore.h"
#include "WKTWriter.h"

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
		return augeGTPoint;
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
}