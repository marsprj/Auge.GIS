#ifndef __AUGE_POINT_WKB_H__
#define __AUGE_POINT_WKB_H__

#include "AugeGeometry.h"
#include <string>

namespace auge
{
	class GPointWKB : public GPoint
	{
	public:
		GPointWKB();
		virtual ~GPointWKB();
	public:
		virtual augeGeometryType	GeometryType();
		virtual const GEnvelope*	Envelope() const;
		virtual const char*			AsText(bool multi=false);
		virtual g_uchar*			AsBinary() const;
		virtual Geometry*			Clone() const;
		virtual void				Centroid(double &x, double &y);
		virtual void				Release();

	public:
		bool	Create(g_uchar *wkb, bool attach);

	private:
		WKBPoint*	m_pWKBPoint;
		std::string	m_wkt;
		GEnvelope	m_extent;
		bool		m_attach;
	};
}

#endif //__AUGE_POINT_WKB_H__
