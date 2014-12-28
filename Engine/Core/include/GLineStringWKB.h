#ifndef __AUGE_LINESTRING_WKB_H__
#define __AUGE_LINESTRING_WKB_H__

#include "AugeGeometry.h"
#include <string>

namespace auge
{
	class GLineStringWKB : public GLineString
	{
	public:
		GLineStringWKB();
		virtual ~GLineStringWKB();
	public:
		virtual augeGeometryType	GeometryType();
		virtual const GEnvelope*	Envelope() const;
		virtual const char*			AsText(bool multi=false);
		virtual g_uchar*			AsBinary() const;
		virtual Geometry*			Clone() const;
		virtual void				Release();

	public:
		bool	Create(g_uchar *wkb, bool attach);

	private:
		WKBLineString*	m_pWKBLineString;
		std::string		m_wkt;
		GEnvelope		m_extent;
		bool			m_attach;
	};
}

#endif //__AUGE_LINESTRING_WKB_H__
