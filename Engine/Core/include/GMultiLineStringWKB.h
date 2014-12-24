#ifndef __AUGE_MULTI_LINESTRING_WKB_H__
#define __AUGE_MULTI_LINESTRING_WKB_H__

#include "AugeGeometry.h"
#include <string>

namespace auge
{
	class GMultiLineStringWKB : public GMultiLineString
	{
	public:
		GMultiLineStringWKB();
		virtual ~GMultiLineStringWKB();
	public:
		virtual augeGeometryType	GeometryType();
		virtual const GEnvelope*	Envelope() const;
		virtual const char*			AsText();
		virtual g_uchar*			AsBinary() const;
		virtual void				Release();

	public:
		bool	Create(g_uchar *wkb, bool attach);

	private:
		WKBMultiLineString*	m_pWKBMultiLineString;
		std::string		m_wkt;
		GEnvelope		m_extent;
		bool			m_attach;
	};
}

#endif //__AUGE_MULTI_LINESTRING_WKB_H__
